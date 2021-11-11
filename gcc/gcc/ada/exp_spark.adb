------------------------------------------------------------------------------
--                                                                          --
--                         GNAT COMPILER COMPONENTS                         --
--                                                                          --
--                            E X P _ S P A R K                             --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
--          Copyright (C) 1992-2021, Free Software Foundation, Inc.         --
--                                                                          --
-- GNAT is free software;  you can  redistribute it  and/or modify it under --
-- terms of the  GNU General Public License as published  by the Free Soft- --
-- ware  Foundation;  either version 3,  or (at your option) any later ver- --
-- sion.  GNAT is distributed in the hope that it will be useful, but WITH- --
-- OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY --
-- or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License --
-- for  more details.  You should have  received  a copy of the GNU General --
-- Public License  distributed with GNAT; see file COPYING3.  If not, go to --
-- http://www.gnu.org/licenses for a complete copy of the license.          --
--                                                                          --
-- GNAT was originally developed  by the GNAT team at  New York University. --
-- Extensive contributions were provided by Ada Core Technologies Inc.      --
--                                                                          --
------------------------------------------------------------------------------

with Atree;          use Atree;
with Checks;         use Checks;
with Einfo;          use Einfo;
with Einfo.Entities; use Einfo.Entities;
with Einfo.Utils;    use Einfo.Utils;
with Exp_Attr;
with Exp_Ch4;
with Exp_Ch5;        use Exp_Ch5;
with Exp_Dbug;       use Exp_Dbug;
with Exp_Util;       use Exp_Util;
with Namet;          use Namet;
with Nlists;         use Nlists;
with Nmake;          use Nmake;
with Rtsfind;        use Rtsfind;
with Sem;            use Sem;
with Sem_Ch8;        use Sem_Ch8;
with Sem_Prag;       use Sem_Prag;
with Sem_Res;        use Sem_Res;
with Sem_Util;       use Sem_Util;
with Sinfo;          use Sinfo;
with Sinfo.Nodes;    use Sinfo.Nodes;
with Sinfo.Utils;    use Sinfo.Utils;
with Snames;         use Snames;
with Stand;          use Stand;
with Tbuild;         use Tbuild;
with Uintp;          use Uintp;

package body Exp_SPARK is

   -----------------------
   -- Local Subprograms --
   -----------------------

   procedure Expand_SPARK_N_Attribute_Reference (N : Node_Id);
   --  Perform attribute-reference-specific expansion

   procedure Expand_SPARK_N_Delta_Aggregate (N : Node_Id);
   --  Perform delta-aggregate-specific expansion

   procedure Expand_SPARK_N_Freeze_Type (E : Entity_Id);
   --  Build the DIC procedure of a type when needed, if not already done

   procedure Expand_SPARK_N_Loop_Statement (N : Node_Id);
   --  Perform loop-statement-specific expansion

   procedure Expand_SPARK_N_Object_Declaration (N : Node_Id);
   --  Perform object-declaration-specific expansion

   procedure Expand_SPARK_N_Object_Renaming_Declaration (N : Node_Id);
   --  Perform name evaluation for a renamed object

   procedure Expand_SPARK_N_Op_Ne (N : Node_Id);
   --  Rewrite operator /= based on operator = when defined explicitly

   procedure Expand_SPARK_Delta_Or_Update (Typ : Entity_Id; Aggr : Node_Id);
   --  Common expansion for attribute Update and delta aggregates

   ------------------
   -- Expand_SPARK --
   ------------------

   procedure Expand_SPARK (N : Node_Id) is
   begin
      case Nkind (N) is

         --  Qualification of entity names in formal verification mode
         --  is limited to the addition of a suffix for homonyms (see
         --  Exp_Dbug.Qualify_Entity_Name). We used to qualify entity names
         --  as full expansion does, but this was removed as this prevents the
         --  verification back-end from using a short name for debugging and
         --  user interaction. The verification back-end already takes care
         --  of qualifying names when needed.

         when N_Block_Statement
            | N_Entry_Declaration
            | N_Package_Body
            | N_Package_Declaration
            | N_Protected_Type_Declaration
            | N_Subprogram_Body
            | N_Task_Type_Declaration
         =>
            Qualify_Entity_Names (N);

         --  Replace occurrences of System'To_Address by calls to
         --  System.Storage_Elements.To_Address.

         when N_Attribute_Reference =>
            Expand_SPARK_N_Attribute_Reference (N);

         when N_Delta_Aggregate =>
            Expand_SPARK_N_Delta_Aggregate (N);

         when N_Expanded_Name
            | N_Identifier
         =>
            Expand_SPARK_Potential_Renaming (N);

         --  Loop iterations over arrays need to be expanded, to avoid getting
         --  two names referring to the same object in memory (the array and
         --  the iterator) in GNATprove, especially since both can be written
         --  (thus possibly leading to interferences due to aliasing). No such
         --  problem arises with quantified expressions over arrays, which are
         --  dealt with specially in GNATprove.

         when N_Loop_Statement =>
            Expand_SPARK_N_Loop_Statement (N);

         when N_Object_Declaration =>
            Expand_SPARK_N_Object_Declaration (N);

         when N_Object_Renaming_Declaration =>
            Expand_SPARK_N_Object_Renaming_Declaration (N);

         when N_Op_Ne =>
            Expand_SPARK_N_Op_Ne (N);

         when N_Freeze_Entity =>
            if Is_Type (Entity (N)) then
               Expand_SPARK_N_Freeze_Type (Entity (N));
            end if;

         --  In SPARK mode, no other constructs require expansion

         when others =>
            null;
      end case;
   end Expand_SPARK;

   ----------------------------------
   -- Expand_SPARK_Delta_Or_Update --
   ----------------------------------

   procedure Expand_SPARK_Delta_Or_Update
     (Typ  : Entity_Id;
      Aggr : Node_Id)
   is
      Assoc     : Node_Id;
      Comp      : Node_Id;
      Comp_Id   : Entity_Id;
      Comp_Type : Entity_Id;
      Expr      : Node_Id;
      Index     : Node_Id;
      Index_Typ : Entity_Id;
      New_Assoc : Node_Id;

   begin
      --  Apply scalar range checks on the updated components, if needed

      if Is_Array_Type (Typ) then

         --  Multidimensional arrays

         if Present (Next_Index (First_Index (Typ))) then
            Assoc := First (Component_Associations (Aggr));

            while Present (Assoc) loop
               Expr      := Expression (Assoc);
               Comp_Type := Component_Type (Typ);

               if Is_Scalar_Type (Comp_Type) then
                  Apply_Scalar_Range_Check (Expr, Comp_Type);
               end if;

               --  The current association contains a sequence of indexes
               --  denoting an element of a multidimensional array:
               --
               --    (Index_1, ..., Index_N)

               Expr := First (Choices (Assoc));

               pragma Assert (Nkind (Aggr) = N_Aggregate);

               while Present (Expr) loop
                  Index     := First (Expressions (Expr));
                  Index_Typ := First_Index (Typ);

                  while Present (Index_Typ) loop
                     Apply_Scalar_Range_Check (Index, Etype (Index_Typ));
                     Next (Index);
                     Next_Index (Index_Typ);
                  end loop;

                  Next (Expr);
               end loop;

               Next (Assoc);
            end loop;

         --  One-dimensional arrays

         else
            Assoc := First (Component_Associations (Aggr));

            while Present (Assoc) loop
               Expr      := Expression (Assoc);
               Comp_Type := Component_Type (Typ);

               --  Analyze expression of the iterated_component_association
               --  with its index parameter in scope.

               if Nkind (Assoc) = N_Iterated_Component_Association then
                  Push_Scope (Scope (Defining_Identifier (Assoc)));
                  Enter_Name (Defining_Identifier (Assoc));
                  Analyze_And_Resolve (Expr, Comp_Type);
               end if;

               if Is_Scalar_Type (Comp_Type) then
                  Apply_Scalar_Range_Check (Expr, Comp_Type);
               end if;

               --  Restore scope of the iterated_component_association

               if Nkind (Assoc) = N_Iterated_Component_Association then
                  End_Scope;
               end if;

               Index     := First (Choice_List (Assoc));
               Index_Typ := First_Index (Typ);

               while Present (Index) loop
                  --  If the index denotes a range of elements or a constrained
                  --  subtype indication, then their low and high bounds
                  --  already have range checks applied.

                  if Nkind (Index) in N_Range | N_Subtype_Indication then
                     null;

                  --  Otherwise the index denotes a single expression where
                  --  range checks need to be applied or a subtype name
                  --  (without range constraints) where applying checks is
                  --  harmless.
                  --
                  --  In delta_aggregate and Update attribute on array the
                  --  others_choice is not allowed.

                  else pragma Assert (Nkind (Index) in N_Subexpr);
                     Apply_Scalar_Range_Check (Index, Etype (Index_Typ));
                  end if;

                  Next (Index);
               end loop;

               Next (Assoc);
            end loop;
         end if;

      else pragma Assert (Is_Record_Type (Typ));

         --  If the aggregate has multiple component choices, e.g.:
         --
         --    X'Update (A | B | C => 123)
         --
         --  then each component might be of a different type and might or
         --  might not require a range check. We first rewrite associations
         --  into single-component choices, e.g.:
         --
         --    X'Update (A => 123, B => 123, C => 123)
         --
         --  and then apply range checks to individual copies of the
         --  expressions. We do the same for delta aggregates, accordingly.

         --  Iterate over associations of the original aggregate

         Assoc := First (Component_Associations (Aggr));

         --  Rewrite into a new aggregate and decorate

         case Nkind (Aggr) is
            when N_Aggregate =>
               Rewrite
                 (Aggr,
                  Make_Aggregate
                    (Sloc                   => Sloc (Aggr),
                     Component_Associations => New_List));

            when N_Delta_Aggregate =>
               Rewrite
                 (Aggr,
                  Make_Delta_Aggregate
                    (Sloc                   => Sloc (Aggr),
                     Expression             => Expression (Aggr),
                     Component_Associations => New_List));

            when others =>
               raise Program_Error;
         end case;

         Set_Etype (Aggr, Typ);

         --  Populate the new aggregate with component associations

         while Present (Assoc) loop
            Expr := Expression (Assoc);
            Comp := First (Choices (Assoc));

            while Present (Comp) loop
               Comp_Id   := Entity (Comp);
               Comp_Type := Etype (Comp_Id);

               New_Assoc :=
                 Make_Component_Association
                   (Sloc       => Sloc (Assoc),
                    Choices    =>
                      New_List
                        (New_Occurrence_Of (Comp_Id, Sloc (Comp))),
                    Expression => New_Copy_Tree (Expr));

               --  New association must be attached to the aggregate before we
               --  analyze it.

               Append (New_Assoc, Component_Associations (Aggr));

               Analyze_And_Resolve (Expression (New_Assoc), Comp_Type);

               if Is_Scalar_Type (Comp_Type) then
                  Apply_Scalar_Range_Check
                    (Expression (New_Assoc), Comp_Type);
               end if;

               Next (Comp);
            end loop;

            Next (Assoc);
         end loop;
      end if;
   end Expand_SPARK_Delta_Or_Update;

   --------------------------------
   -- Expand_SPARK_N_Freeze_Type --
   --------------------------------

   procedure Expand_SPARK_N_Freeze_Type (E : Entity_Id) is
   begin
      --  When a DIC is inherited by a tagged type, it may need to be
      --  specialized to the descendant type, hence build a separate DIC
      --  procedure for it as done during regular expansion for compilation.

      if Has_DIC (E) and then Is_Tagged_Type (E) then
         --  Why is this needed for DIC, but not for other aspects (such as
         --  Type_Invariant)???

         Build_DIC_Procedure_Body (E);
      end if;
   end Expand_SPARK_N_Freeze_Type;

   ----------------------------------------
   -- Expand_SPARK_N_Attribute_Reference --
   ----------------------------------------

   procedure Expand_SPARK_N_Attribute_Reference (N : Node_Id) is
      Aname   : constant Name_Id      := Attribute_Name (N);
      Attr_Id : constant Attribute_Id := Get_Attribute_Id (Aname);
      Loc     : constant Source_Ptr   := Sloc (N);
      Pref    : constant Node_Id      := Prefix (N);
      Typ     : constant Entity_Id    := Etype (N);
      Expr    : Node_Id;

   begin
      case Attr_Id is
         when Attribute_To_Address =>

            --  Extract and convert argument to expected type for call

            Expr :=
              Make_Type_Conversion (Loc,
                Subtype_Mark =>
                  New_Occurrence_Of (RTE (RE_Integer_Address), Loc),
                Expression   => Relocate_Node (First (Expressions (N))));

            --  Replace attribute reference with call

            Rewrite
              (N,
               Make_Function_Call (Loc,
                 Name                   =>
                   New_Occurrence_Of (RTE (RE_To_Address), Loc),
                 Parameter_Associations => New_List (Expr)));
            Analyze_And_Resolve (N, Typ);

         when Attribute_Object_Size
            | Attribute_Size
            | Attribute_Value_Size
            | Attribute_VADS_Size
         =>
            Exp_Attr.Expand_Size_Attribute (N);

         --  For attributes which return Universal_Integer, introduce a
         --  conversion to the expected type with the appropriate check flags
         --  set.

         when Attribute_Aft
            | Attribute_Alignment
            | Attribute_Bit
            | Attribute_Bit_Position
            | Attribute_Descriptor_Size
            | Attribute_First_Bit
            | Attribute_Last_Bit
            | Attribute_Length
            | Attribute_Max_Alignment_For_Allocation
            | Attribute_Max_Size_In_Storage_Elements
            | Attribute_Pos
            | Attribute_Position
            | Attribute_Range_Length
         =>
            --  If the expected type is Long_Long_Integer, there will be no
            --  check flag as the compiler assumes attributes always fit in
            --  this type. Since in SPARK_Mode we do not take Storage_Error
            --  into account, we cannot make this assumption and need to
            --  produce a check. ??? It should be enough to add this check for
            --  attributes 'Length, 'Range_Length and 'Pos when the type is as
            --  big as Long_Long_Integer.

            declare
               Typ : Entity_Id;
            begin
               if Attr_Id in Attribute_Pos | Attribute_Range_Length then
                  Typ := Etype (Prefix (N));

               elsif Attr_Id = Attribute_Length then
                  Typ := Get_Index_Subtype (N);

               else
                  Typ := Empty;
               end if;

               Apply_Universal_Integer_Attribute_Checks (N);

               if Present (Typ)
                 and then Known_RM_Size (Typ)
                 and then RM_Size (Typ) = RM_Size (Standard_Long_Long_Integer)
               then
                  --  ??? This should rather be a range check, but this would
                  --  crash GNATprove which somehow recovers the proper kind
                  --  of check anyway.
                  Set_Do_Overflow_Check (N);
               end if;
            end;

         when Attribute_Constrained =>

            --  If the prefix is an access to object, the attribute applies to
            --  the designated object, so rewrite with an explicit dereference.

            if Is_Access_Type (Etype (Pref))
              and then
              (not Is_Entity_Name (Pref) or else Is_Object (Entity (Pref)))
            then
               Rewrite (Pref,
                        Make_Explicit_Dereference (Loc, Relocate_Node (Pref)));
               Analyze_And_Resolve (N, Standard_Boolean);
            end if;

         when Attribute_Update =>
            Expand_SPARK_Delta_Or_Update (Typ, First (Expressions (N)));

         when others =>
            null;
      end case;
   end Expand_SPARK_N_Attribute_Reference;

   ------------------------------------
   -- Expand_SPARK_N_Delta_Aggregate --
   ------------------------------------

   procedure Expand_SPARK_N_Delta_Aggregate (N : Node_Id) is
   begin
      Expand_SPARK_Delta_Or_Update (Etype (N), N);
   end Expand_SPARK_N_Delta_Aggregate;

   -----------------------------------
   -- Expand_SPARK_N_Loop_Statement --
   -----------------------------------

   procedure Expand_SPARK_N_Loop_Statement (N : Node_Id) is
      Scheme : constant Node_Id := Iteration_Scheme (N);

   begin
      --  Loop iterations over arrays need to be expanded, to avoid getting
      --  two names referring to the same object in memory (the array and the
      --  iterator) in GNATprove, especially since both can be written (thus
      --  possibly leading to interferences due to aliasing). No such problem
      --  arises with quantified expressions over arrays, which are dealt with
      --  specially in GNATprove.

      if Present (Scheme)
        and then Present (Iterator_Specification (Scheme))
        and then Is_Iterator_Over_Array (Iterator_Specification (Scheme))
      then
         Expand_Iterator_Loop_Over_Array (N);
      end if;
   end Expand_SPARK_N_Loop_Statement;

   ---------------------------------------
   -- Expand_SPARK_N_Object_Declaration --
   ---------------------------------------

   procedure Expand_SPARK_N_Object_Declaration (N : Node_Id) is
      Loc    : constant Source_Ptr := Sloc (N);
      Obj_Id : constant Entity_Id  := Defining_Identifier (N);
      Typ    : constant Entity_Id  := Etype (Obj_Id);

      Call : Node_Id;

   begin
      --  If the object declaration denotes a variable without initialization
      --  whose type is subject to pragma Default_Initial_Condition, create
      --  and analyze a dummy call to the DIC procedure of the type in order
      --  to detect potential elaboration issues.

      if Comes_From_Source (Obj_Id)
        and then Ekind (Obj_Id) = E_Variable
        and then Has_DIC (Typ)
        and then Present (DIC_Procedure (Typ))
        and then not Has_Init_Expression (N)
      then
         Call := Build_DIC_Call (Loc, New_Occurrence_Of (Obj_Id, Loc), Typ);

         --  Partially insert the call into the tree by setting its parent
         --  pointer.

         Set_Parent (Call, N);
         Analyze (Call);
      end if;
   end Expand_SPARK_N_Object_Declaration;

   ------------------------------------------------
   -- Expand_SPARK_N_Object_Renaming_Declaration --
   ------------------------------------------------

   procedure Expand_SPARK_N_Object_Renaming_Declaration (N : Node_Id) is
      CFS    : constant Boolean    := Comes_From_Source (N);
      Loc    : constant Source_Ptr := Sloc (N);
      Obj_Id : constant Entity_Id  := Defining_Entity (N);
      Nam    : constant Node_Id    := Name (N);
      Typ    : constant Entity_Id  := Etype (Obj_Id);

   begin
      --  Transform a renaming of the form

      --    Obj_Id : <subtype mark> renames <function call>;

      --  into

      --    Obj_Id : constant <subtype mark> := <function call>;

      --  Invoking Evaluate_Name and ultimately Remove_Side_Effects introduces
      --  a temporary to capture the function result. Once potential renamings
      --  are rewritten for SPARK, the temporary may be leaked out into source
      --  constructs and lead to confusing error diagnostics. Using an object
      --  declaration prevents this unwanted side effect.

      if Nkind (Nam) = N_Function_Call then
         Rewrite (N,
           Make_Object_Declaration (Loc,
             Defining_Identifier => Obj_Id,
             Constant_Present    => True,
             Object_Definition   => New_Occurrence_Of (Typ, Loc),
             Expression          => Nam));

         --  Inherit the original Comes_From_Source status of the renaming

         Set_Comes_From_Source (N, CFS);

         --  Sever the link to the renamed function result because the entity
         --  will no longer alias anything.

         Set_Renamed_Object (Obj_Id, Empty);

         --  Remove the entity of the renaming declaration from visibility as
         --  the analysis of the object declaration will reintroduce it again.

         Remove_Entity_And_Homonym (Obj_Id);
         Analyze (N);

      --  Otherwise unconditionally remove all side effects from the name

      else
         Evaluate_Name (Nam);
      end if;
   end Expand_SPARK_N_Object_Renaming_Declaration;

   --------------------------
   -- Expand_SPARK_N_Op_Ne --
   --------------------------

   procedure Expand_SPARK_N_Op_Ne (N : Node_Id) is
      Typ : constant Entity_Id := Etype (Left_Opnd (N));

   begin
      --  Case of elementary type with standard operator

      if Is_Elementary_Type (Typ)
        and then Sloc (Entity (N)) = Standard_Location
      then
         null;

      else
         Exp_Ch4.Expand_N_Op_Ne (N);
      end if;
   end Expand_SPARK_N_Op_Ne;

   -------------------------------------
   -- Expand_SPARK_Potential_Renaming --
   -------------------------------------

   procedure Expand_SPARK_Potential_Renaming (N : Node_Id) is
      function In_Insignificant_Pragma (Nod : Node_Id) return Boolean;
      --  Determine whether arbitrary node Nod appears within a significant
      --  pragma for SPARK.

      -----------------------------
      -- In_Insignificant_Pragma --
      -----------------------------

      function In_Insignificant_Pragma (Nod : Node_Id) return Boolean is
         Par : Node_Id;

      begin
         --  Climb the parent chain looking for an enclosing pragma

         Par := Nod;
         while Present (Par) loop
            if Nkind (Par) = N_Pragma then
               return not Pragma_Significant_In_SPARK (Get_Pragma_Id (Par));

            --  Prevent the search from going too far

            elsif Is_Body_Or_Package_Declaration (Par) then
               exit;
            end if;

            Par := Parent (Par);
         end loop;

         return False;
      end In_Insignificant_Pragma;

      --  Local variables

      Loc    : constant Source_Ptr := Sloc (N);
      Obj_Id : constant Entity_Id  := Entity (N);
      Typ    : constant Entity_Id  := Etype (N);
      Ren    : Node_Id;

   --  Start of processing for Expand_SPARK_Potential_Renaming

   begin
      --  Replace a reference to a renaming with the actual renamed object

      if Is_Object (Obj_Id) then
         Ren := Renamed_Object (Obj_Id);

         if Present (Ren) then

            --  Do not process a reference when it appears within a pragma of
            --  no significance to SPARK. It is assumed that the replacement
            --  will violate the semantics of the pragma and cause a spurious
            --  error.

            if In_Insignificant_Pragma (N) then
               return;

            --  Instantiations and inlining of subprograms employ "prologues"
            --  which map actual to formal parameters by means of renamings.
            --  Replace a reference to a formal by the corresponding actual
            --  parameter.

            elsif Nkind (Ren) in N_Entity then
               Rewrite (N, New_Occurrence_Of (Ren, Loc));

            --  Otherwise the renamed object denotes a name

            else
               Rewrite (N, New_Copy_Tree (Ren, New_Sloc => Loc));
               Reset_Analyzed_Flags (N);
            end if;

            Analyze_And_Resolve (N, Typ);
         end if;
      end if;
   end Expand_SPARK_Potential_Renaming;

end Exp_SPARK;