// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * fs/cifs/winucase.c
 *
 * Copyright (c) Jeffrey Layton <jlayton@redhat.com>, 2013
 *
 * The const tables in this file were converted from the following info
 * provided by Microsoft:
 *
 * 3.1.5.3 Mapping UTF-16 Strings to Upper Case:
 *
 * https://msdn.microsoft.com/en-us/library/hh877830.aspx
 * http://www.microsoft.com/en-us/download/details.aspx?displaylang=en&id=10921
 *
 * In particular, the table in "Windows 8 Upper Case Mapping Table.txt" was
 * post-processed using the winucase_convert.pl script.
 */

#include <linux/nls.h>

wchar_t cifs_toupper(wchar_t in);  /* quiet sparse */

static const wchar_t t2_00[256] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047,
	0x0048, 0x0049, 0x004a, 0x004b, 0x004c, 0x004d, 0x004e, 0x004f,
	0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057,
	0x0058, 0x0059, 0x005a, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7,
	0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,
	0x00d0, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x0000,
	0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x00de, 0x0178,
};

static const wchar_t t2_01[256] = {
	0x0000, 0x0100, 0x0000, 0x0102, 0x0000, 0x0104, 0x0000, 0x0106,
	0x0000, 0x0108, 0x0000, 0x010a, 0x0000, 0x010c, 0x0000, 0x010e,
	0x0000, 0x0110, 0x0000, 0x0112, 0x0000, 0x0114, 0x0000, 0x0116,
	0x0000, 0x0118, 0x0000, 0x011a, 0x0000, 0x011c, 0x0000, 0x011e,
	0x0000, 0x0120, 0x0000, 0x0122, 0x0000, 0x0124, 0x0000, 0x0126,
	0x0000, 0x0128, 0x0000, 0x012a, 0x0000, 0x012c, 0x0000, 0x012e,
	0x0000, 0x0000, 0x0000, 0x0132, 0x0000, 0x0134, 0x0000, 0x0136,
	0x0000, 0x0000, 0x0139, 0x0000, 0x013b, 0x0000, 0x013d, 0x0000,
	0x013f, 0x0000, 0x0141, 0x0000, 0x0143, 0x0000, 0x0145, 0x0000,
	0x0147, 0x0000, 0x0000, 0x014a, 0x0000, 0x014c, 0x0000, 0x014e,
	0x0000, 0x0150, 0x0000, 0x0152, 0x0000, 0x0154, 0x0000, 0x0156,
	0x0000, 0x0158, 0x0000, 0x015a, 0x0000, 0x015c, 0x0000, 0x015e,
	0x0000, 0x0160, 0x0000, 0x0162, 0x0000, 0x0164, 0x0000, 0x0166,
	0x0000, 0x0168, 0x0000, 0x016a, 0x0000, 0x016c, 0x0000, 0x016e,
	0x0000, 0x0170, 0x0000, 0x0172, 0x0000, 0x0174, 0x0000, 0x0176,
	0x0000, 0x0000, 0x0179, 0x0000, 0x017b, 0x0000, 0x017d, 0x0000,
	0x0243, 0x0000, 0x0000, 0x0182, 0x0000, 0x0184, 0x0000, 0x0000,
	0x0187, 0x0000, 0x0000, 0x0000, 0x018b, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0191, 0x0000, 0x0000, 0x01f6, 0x0000, 0x0000,
	0x0000, 0x0198, 0x023d, 0x0000, 0x0000, 0x0000, 0x0220, 0x0000,
	0x0000, 0x01a0, 0x0000, 0x01a2, 0x0000, 0x01a4, 0x0000, 0x0000,
	0x01a7, 0x0000, 0x0000, 0x0000, 0x0000, 0x01ac, 0x0000, 0x0000,
	0x01af, 0x0000, 0x0000, 0x0000, 0x01b3, 0x0000, 0x01b5, 0x0000,
	0x0000, 0x01b8, 0x0000, 0x0000, 0x0000, 0x01bc, 0x0000, 0x01f7,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x01c4, 0x0000,
	0x0000, 0x01c7, 0x0000, 0x0000, 0x01ca, 0x0000, 0x01cd, 0x0000,
	0x01cf, 0x0000, 0x01d1, 0x0000, 0x01d3, 0x0000, 0x01d5, 0x0000,
	0x01d7, 0x0000, 0x01d9, 0x0000, 0x01db, 0x018e, 0x0000, 0x01de,
	0x0000, 0x01e0, 0x0000, 0x01e2, 0x0000, 0x01e4, 0x0000, 0x01e6,
	0x0000, 0x01e8, 0x0000, 0x01ea, 0x0000, 0x01ec, 0x0000, 0x01ee,
	0x0000, 0x0000, 0x0000, 0x01f1, 0x0000, 0x01f4, 0x0000, 0x0000,
	0x0000, 0x01f8, 0x0000, 0x01fa, 0x0000, 0x01fc, 0x0000, 0x01fe,
};

static const wchar_t t2_02[256] = {
	0x0000, 0x0200, 0x0000, 0x0202, 0x0000, 0x0204, 0x0000, 0x0206,
	0x0000, 0x0208, 0x0000, 0x020a, 0x0000, 0x020c, 0x0000, 0x020e,
	0x0000, 0x0210, 0x0000, 0x0212, 0x0000, 0x0214, 0x0000, 0x0216,
	0x0000, 0x0218, 0x0000, 0x021a, 0x0000, 0x021c, 0x0000, 0x021e,
	0x0000, 0x0000, 0x0000, 0x0222, 0x0000, 0x0224, 0x0000, 0x0226,
	0x0000, 0x0228, 0x0000, 0x022a, 0x0000, 0x022c, 0x0000, 0x022e,
	0x0000, 0x0230, 0x0000, 0x0232, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x023b, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0241, 0x0000, 0x0000, 0x0000, 0x0000, 0x0246,
	0x0000, 0x0248, 0x0000, 0x024a, 0x0000, 0x024c, 0x0000, 0x024e,
	0x2c6f, 0x2c6d, 0x0000, 0x0181, 0x0186, 0x0000, 0x0189, 0x018a,
	0x0000, 0x018f, 0x0000, 0x0190, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0193, 0x0000, 0x0000, 0x0194, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0197, 0x0196, 0x0000, 0x2c62, 0x0000, 0x0000, 0x0000, 0x019c,
	0x0000, 0x2c6e, 0x019d, 0x0000, 0x0000, 0x019f, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2c64, 0x0000, 0x0000,
	0x01a6, 0x0000, 0x0000, 0x01a9, 0x0000, 0x0000, 0x0000, 0x0000,
	0x01ae, 0x0244, 0x01b1, 0x01b2, 0x0245, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x01b7, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const wchar_t t2_03[256] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0370, 0x0000, 0x0372, 0x0000, 0x0000, 0x0000, 0x0376,
	0x0000, 0x0000, 0x0000, 0x03fd, 0x03fe, 0x03ff, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0386, 0x0388, 0x0389, 0x038a,
	0x0000, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397,
	0x0398, 0x0399, 0x039a, 0x039b, 0x039c, 0x039d, 0x039e, 0x039f,
	0x03a0, 0x03a1, 0x0000, 0x03a3, 0x03a4, 0x03a5, 0x03a6, 0x03a7,
	0x03a8, 0x03a9, 0x03aa, 0x03ab, 0x038c, 0x038e, 0x038f, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x03cf,
	0x0000, 0x03d8, 0x0000, 0x03da, 0x0000, 0x03dc, 0x0000, 0x03de,
	0x0000, 0x03e0, 0x0000, 0x03e2, 0x0000, 0x03e4, 0x0000, 0x03e6,
	0x0000, 0x03e8, 0x0000, 0x03ea, 0x0000, 0x03ec, 0x0000, 0x03ee,
	0x0000, 0x0000, 0x03f9, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x03f7, 0x0000, 0x0000, 0x03fa, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const wchar_t t2_04[256] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417,
	0x0418, 0x0419, 0x041a, 0x041b, 0x041c, 0x041d, 0x041e, 0x041f,
	0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427,
	0x0428, 0x0429, 0x042a, 0x042b, 0x042c, 0x042d, 0x042e, 0x042f,
	0x0400, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407,
	0x0408, 0x0409, 0x040a, 0x040b, 0x040c, 0x040d, 0x040e, 0x040f,
	0x0000, 0x0460, 0x0000, 0x0462, 0x0000, 0x0464, 0x0000, 0x0466,
	0x0000, 0x0468, 0x0000, 0x046a, 0x0000, 0x046c, 0x0000, 0x046e,
	0x0000, 0x0470, 0x0000, 0x0472, 0x0000, 0x0474, 0x0000, 0x0476,
	0x0000, 0x0478, 0x0000, 0x047a, 0x0000, 0x047c, 0x0000, 0x047e,
	0x0000, 0x0480, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x048a, 0x0000, 0x048c, 0x0000, 0x048e,
	0x0000, 0x0490, 0x0000, 0x0492, 0x0000, 0x0494, 0x0000, 0x0496,
	0x0000, 0x0498, 0x0000, 0x049a, 0x0000, 0x049c, 0x0000, 0x049e,
	0x0000, 0x04a0, 0x0000, 0x04a2, 0x0000, 0x04a4, 0x0000, 0x04a6,
	0x0000, 0x04a8, 0x0000, 0x04aa, 0x0000, 0x04ac, 0x0000, 0x04ae,
	0x0000, 0x04b0, 0x0000, 0x04b2, 0x0000, 0x04b4, 0x0000, 0x04b6,
	0x0000, 0x04b8, 0x0000, 0x04ba, 0x0000, 0x04bc, 0x0000, 0x04be,
	0x0000, 0x0000, 0x04c1, 0x0000, 0x04c3, 0x0000, 0x04c5, 0x0000,
	0x04c7, 0x0000, 0x04c9, 0x0000, 0x04cb, 0x0000, 0x04cd, 0x04c0,
	0x0000, 0x04d0, 0x0000, 0x04d2, 0x0000, 0x04d4, 0x0000, 0x04d6,
	0x0000, 0x04d8, 0x0000, 0x04da, 0x0000, 0x04dc, 0x0000, 0x04de,
	0x0000, 0x04e0, 0x0000, 0x04e2, 0x0000, 0x04e4, 0x0000, 0x04e6,
	0x0000, 0x04e8, 0x0000, 0x04ea, 0x0000, 0x04ec, 0x0000, 0x04ee,
	0x0000, 0x04f0, 0x0000, 0x04f2, 0x0000, 0x04f4, 0x0000, 0x04f6,
	0x0000, 0x04f8, 0x0000, 0x04fa, 0x0000, 0x04fc, 0x0000, 0x04fe,
};

static const wchar_t t2_05[256] = {
	0x0000, 0x0500, 0x0000, 0x0502, 0x0000, 0x0504, 0x0000, 0x0506,
	0x0000, 0x0508, 0x0000, 0x050a, 0x0000, 0x050c, 0x0000, 0x050e,
	0x0000, 0x0510, 0x0000, 0x0512, 0x0000, 0x0514, 0x0000, 0x0516,
	0x0000, 0x0518, 0x0000, 0x051a, 0x0000, 0x051c, 0x0000, 0x051e,
	0x0000, 0x0520, 0x0000, 0x0522, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0531, 0x0532, 0x0533, 0x0534, 0x0535, 0x0536, 0x0537,
	0x0538, 0x0539, 0x053a, 0x053b, 0x053c, 0x053d, 0x053e, 0x053f,
	0x0540, 0x0541, 0x0542, 0x0543, 0x0544, 0x0545, 0x0546, 0x0547,
	0x0548, 0x0549, 0x054a, 0x054b, 0x054c, 0x054d, 0x054e, 0x054f,
	0x0550, 0x0551, 0x0552, 0x0553, 0x0554, 0x0555, 0x0556, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const wchar_t t2_1d[256] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0xa77d, 0x0000, 0x0000, 0x0000, 0x2c63, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const wchar_t t2_1e[256] = {
	0x0000, 0x1e00, 0x0000, 0x1e02, 0x0000, 0x1e04, 0x0000, 0x1e06,
	0x0000, 0x1e08, 0x0000, 0x1e0a, 0x0000, 0x1e0c, 0x0000, 0x1e0e,
	0x0000, 0x1e10, 0x0000, 0x1e12, 0x0000, 0x1e14, 0x0000, 0x1e16,
	0x0000, 0x1e18, 0x0000, 0x1e1a, 0x0000, 0x1e1c, 0x0000, 0x1e1e,
	0x0000, 0x1e20, 0x0000, 0x1e22, 0x0000, 0x1e24, 0x0000, 0x1e26,
	0x0000, 0x1e28, 0x0000, 0x1e2a, 0x0000, 0x1e2c, 0x0000, 0x1e2e,
	0x0000, 0x1e30, 0x0000, 0x1e32, 0x0000, 0x1e34, 0x0000, 0x1e36,
	0x0000, 0x1e38, 0x0000, 0x1e3a, 0x0000, 0x1e3c, 0x0000, 0x1e3e,
	0x0000, 0x1e40, 0x0000, 0x1e42, 0x0000, 0x1e44, 0x0000, 0x1e46,
	0x0000, 0x1e48, 0x0000, 0x1e4a, 0x0000, 0x1e4c, 0x0000, 0x1e4e,
	0x0000, 0x1e50, 0x0000, 0x1e52, 0x0000, 0x1e54, 0x0000, 0x1e56,
	0x0000, 0x1e58, 0x0000, 0x1e5a, 0x0000, 0x1e5c, 0x0000, 0x1e5e,
	0x0000, 0x1e60, 0x0000, 0x1e62, 0x0000, 0x1e64, 0x0000, 0x1e66,
	0x0000, 0x1e68, 0x0000, 0x1e6a, 0x0000, 0x1e6c, 0x0000, 0x1e6e,
	0x0000, 0x1e70, 0x0000, 0x1e72, 0x0000, 0x1e74, 0x0000, 0x1e76,
	0x0000, 0x1e78, 0x0000, 0x1e7a, 0x0000, 0x1e7c, 0x0000, 0x1e7e,
	0x0000, 0x1e80, 0x0000, 0x1e82, 0x0000, 0x1e84, 0x0000, 0x1e86,
	0x0000, 0x1e88, 0x0000, 0x1e8a, 0x0000, 0x1e8c, 0x0000, 0x1e8e,
	0x0000, 0x1e90, 0x0000, 0x1e92, 0x0000, 0x1e94, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x1ea0, 0x0000, 0x1ea2, 0x0000, 0x1ea4, 0x0000, 0x1ea6,
	0x0000, 0x1ea8, 0x0000, 0x1eaa, 0x0000, 0x1eac, 0x0000, 0x1eae,
	0x0000, 0x1eb0, 0x0000, 0x1eb2, 0x0000, 0x1eb4, 0x0000, 0x1eb6,
	0x0000, 0x1eb8, 0x0000, 0x1eba, 0x0000, 0x1ebc, 0x0000, 0x1ebe,
	0x0000, 0x1ec0, 0x0000, 0x1ec2, 0x0000, 0x1ec4, 0x0000, 0x1ec6,
	0x0000, 0x1ec8, 0x0000, 0x1eca, 0x0000, 0x1ecc, 0x0000, 0x1ece,
	0x0000, 0x1ed0, 0x0000, 0x1ed2, 0x0000, 0x1ed4, 0x0000, 0x1ed6,
	0x0000, 0x1ed8, 0x0000, 0x1eda, 0x0000, 0x1edc, 0x0000, 0x1ede,
	0x0000, 0x1ee0, 0x0000, 0x1ee2, 0x0000, 0x1ee4, 0x0000, 0x1ee6,
	0x0000, 0x1ee8, 0x0000, 0x1eea, 0x0000, 0x1eec, 0x0000, 0x1eee,
	0x0000, 0x1ef0, 0x0000, 0x1ef2, 0x0000, 0x1ef4, 0x0000, 0x1ef6,
	0x0000, 0x1ef8, 0x0000, 0x1efa, 0x0000, 0x1efc, 0x0000, 0x1efe,
};

static const wchar_t t2_1f[256] = {
	0x1f08, 0x1f09, 0x1f0a, 0x1f0b, 0x1f0c, 0x1f0d, 0x1f0e, 0x1f0f,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x1f18, 0x1f19, 0x1f1a, 0x1f1b, 0x1f1c, 0x1f1d, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x1f28, 0x1f29, 0x1f2a, 0x1f2b, 0x1f2c, 0x1f2d, 0x1f2e, 0x1f2f,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x1f38, 0x1f39, 0x1f3a, 0x1f3b, 0x1f3c, 0x1f3d, 0x1f3e, 0x1f3f,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x1f48, 0x1f49, 0x1f4a, 0x1f4b, 0x1f4c, 0x1f4d, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x1f59, 0x0000, 0x1f5b, 0x0000, 0x1f5d, 0x0000, 0x1f5f,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x1f68, 0x1f69, 0x1f6a, 0x1f6b, 0x1f6c, 0x1f6d, 0x1f6e, 0x1f6f,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x1fba, 0x1fbb, 0x1fc8, 0x1fc9, 0x1fca, 0x1fcb, 0x1fda, 0x1fdb,
	0x1ff8, 0x1ff9, 0x1fea, 0x1feb, 0x1ffa, 0x1ffb, 0x0000, 0x0000,
	0x1f88, 0x1f89, 0x1f8a, 0x1f8b, 0x1f8c, 0x1f8d, 0x1f8e, 0x1f8f,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x1f98, 0x1f99, 0x1f9a, 0x1f9b, 0x1f9c, 0x1f9d, 0x1f9e, 0x1f9f,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x1fa8, 0x1fa9, 0x1faa, 0x1fab, 0x1fac, 0x1fad, 0x1fae, 0x1faf,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x1fb8, 0x1fb9, 0x0000, 0x1fbc, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x1fcc, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x1fd8, 0x1fd9, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x1fe8, 0x1fe9, 0x0000, 0x0000, 0x0000, 0x1fec, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x1ffc, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const wchar_t t2_21[256] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2132, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x2160, 0x2161, 0x2162, 0x2163, 0x2164, 0x2165, 0x2166, 0x2167,
	0x2168, 0x2169, 0x216a, 0x216b, 0x216c, 0x216d, 0x216e, 0x216f,
	0x0000, 0x0000, 0x0000, 0x0000, 0x2183, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const wchar_t t2_24[256] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x24b6, 0x24b7, 0x24b8, 0x24b9, 0x24ba, 0x24bb, 0x24bc, 0x24bd,
	0x24be, 0x24bf, 0x24c0, 0x24c1, 0x24c2, 0x24c3, 0x24c4, 0x24c5,
	0x24c6, 0x24c7, 0x24c8, 0x24c9, 0x24ca, 0x24cb, 0x24cc, 0x24cd,
	0x24ce, 0x24cf, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const wchar_t t2_2c[256] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x2c00, 0x2c01, 0x2c02, 0x2c03, 0x2c04, 0x2c05, 0x2c06, 0x2c07,
	0x2c08, 0x2c09, 0x2c0a, 0x2c0b, 0x2c0c, 0x2c0d, 0x2c0e, 0x2c0f,
	0x2c10, 0x2c11, 0x2c12, 0x2c13, 0x2c14, 0x2c15, 0x2c16, 0x2c17,
	0x2c18, 0x2c19, 0x2c1a, 0x2c1b, 0x2c1c, 0x2c1d, 0x2c1e, 0x2c1f,
	0x2c20, 0x2c21, 0x2c22, 0x2c23, 0x2c24, 0x2c25, 0x2c26, 0x2c27,
	0x2c28, 0x2c29, 0x2c2a, 0x2c2b, 0x2c2c, 0x2c2d, 0x2c2e, 0x0000,
	0x0000, 0x2c60, 0x0000, 0x0000, 0x0000, 0x023a, 0x023e, 0x0000,
	0x2c67, 0x0000, 0x2c69, 0x0000, 0x2c6b, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x2c72, 0x0000, 0x0000, 0x2c75, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x2c80, 0x0000, 0x2c82, 0x0000, 0x2c84, 0x0000, 0x2c86,
	0x0000, 0x2c88, 0x0000, 0x2c8a, 0x0000, 0x2c8c, 0x0000, 0x2c8e,
	0x0000, 0x2c90, 0x0000, 0x2c92, 0x0000, 0x2c94, 0x0000, 0x2c96,
	0x0000, 0x2c98, 0x0000, 0x2c9a, 0x0000, 0x2c9c, 0x0000, 0x2c9e,
	0x0000, 0x2ca0, 0x0000, 0x2ca2, 0x0000, 0x2ca4, 0x0000, 0x2ca6,
	0x0000, 0x2ca8, 0x0000, 0x2caa, 0x0000, 0x2cac, 0x0000, 0x2cae,
	0x0000, 0x2cb0, 0x0000, 0x2cb2, 0x0000, 0x2cb4, 0x0000, 0x2cb6,
	0x0000, 0x2cb8, 0x0000, 0x2cba, 0x0000, 0x2cbc, 0x0000, 0x2cbe,
	0x0000, 0x2cc0, 0x0000, 0x2cc2, 0x0000, 0x2cc4, 0x0000, 0x2cc6,
	0x0000, 0x2cc8, 0x0000, 0x2cca, 0x0000, 0x2ccc, 0x0000, 0x2cce,
	0x0000, 0x2cd0, 0x0000, 0x2cd2, 0x0000, 0x2cd4, 0x0000, 0x2cd6,
	0x0000, 0x2cd8, 0x0000, 0x2cda, 0x0000, 0x2cdc, 0x0000, 0x2cde,
	0x0000, 0x2ce0, 0x0000, 0x2ce2, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const wchar_t t2_2d[256] = {
	0x10a0, 0x10a1, 0x10a2, 0x10a3, 0x10a4, 0x10a5, 0x10a6, 0x10a7,
	0x10a8, 0x10a9, 0x10aa, 0x10ab, 0x10ac, 0x10ad, 0x10ae, 0x10af,
	0x10b0, 0x10b1, 0x10b2, 0x10b3, 0x10b4, 0x10b5, 0x10b6, 0x10b7,
	0x10b8, 0x10b9, 0x10ba, 0x10bb, 0x10bc, 0x10bd, 0x10be, 0x10bf,
	0x10c0, 0x10c1, 0x10c2, 0x10c3, 0x10c4, 0x10c5, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const wchar_t t2_a6[256] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0xa640, 0x0000, 0xa642, 0x0000, 0xa644, 0x0000, 0xa646,
	0x0000, 0xa648, 0x0000, 0xa64a, 0x0000, 0xa64c, 0x0000, 0xa64e,
	0x0000, 0xa650, 0x0000, 0xa652, 0x0000, 0xa654, 0x0000, 0xa656,
	0x0000, 0xa658, 0x0000, 0xa65a, 0x0000, 0xa65c, 0x0000, 0xa65e,
	0x0000, 0x0000, 0x0000, 0xa662, 0x0000, 0xa664, 0x0000, 0xa666,
	0x0000, 0xa668, 0x0000, 0xa66a, 0x0000, 0xa66c, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0xa680, 0x0000, 0xa682, 0x0000, 0xa684, 0x0000, 0xa686,
	0x0000, 0xa688, 0x0000, 0xa68a, 0x0000, 0xa68c, 0x0000, 0xa68e,
	0x0000, 0xa690, 0x0000, 0xa692, 0x0000, 0xa694, 0x0000, 0xa696,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const wchar_t t2_a7[256] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0xa722, 0x0000, 0xa724, 0x0000, 0xa726,
	0x0000, 0xa728, 0x0000, 0xa72a, 0x0000, 0xa72c, 0x0000, 0xa72e,
	0x0000, 0x0000, 0x0000, 0xa732, 0x0000, 0xa734, 0x0000, 0xa736,
	0x0000, 0xa738, 0x0000, 0xa73a, 0x0000, 0xa73c, 0x0000, 0xa73e,
	0x0000, 0xa740, 0x0000, 0xa742, 0x0000, 0xa744, 0x0000, 0xa746,
	0x0000, 0xa748, 0x0000, 0xa74a, 0x0000, 0xa74c, 0x0000, 0xa74e,
	0x0000, 0xa750, 0x0000, 0xa752, 0x0000, 0xa754, 0x0000, 0xa756,
	0x0000, 0xa758, 0x0000, 0xa75a, 0x0000, 0xa75c, 0x0000, 0xa75e,
	0x0000, 0xa760, 0x0000, 0xa762, 0x0000, 0xa764, 0x0000, 0xa766,
	0x0000, 0xa768, 0x0000, 0xa76a, 0x0000, 0xa76c, 0x0000, 0xa76e,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0xa779, 0x0000, 0xa77b, 0x0000, 0x0000, 0xa77e,
	0x0000, 0xa780, 0x0000, 0xa782, 0x0000, 0xa784, 0x0000, 0xa786,
	0x0000, 0x0000, 0x0000, 0x0000, 0xa78b, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const wchar_t t2_ff[256] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0xff21, 0xff22, 0xff23, 0xff24, 0xff25, 0xff26, 0xff27,
	0xff28, 0xff29, 0xff2a, 0xff2b, 0xff2c, 0xff2d, 0xff2e, 0xff2f,
	0xff30, 0xff31, 0xff32, 0xff33, 0xff34, 0xff35, 0xff36, 0xff37,
	0xff38, 0xff39, 0xff3a, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static const wchar_t *const toplevel[256] = {
	t2_00, t2_01, t2_02, t2_03, t2_04, t2_05,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL, t2_1d, t2_1e, t2_1f,
	NULL, t2_21,  NULL,  NULL, t2_24,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL, t2_2c, t2_2d,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL, t2_a6, t2_a7,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL,
	NULL,  NULL,  NULL,  NULL,  NULL,  NULL,  NULL, t2_ff,
};

/**
 * cifs_toupper - convert a wchar_t from lower to uppercase
 * @in: character to convert from lower to uppercase
 *
 * This function consults the static tables above to convert a wchar_t from
 * lower to uppercase. In the event that there is no mapping, the original
 * "in" character is returned.
 */
wchar_t
cifs_toupper(wchar_t in)
{
	unsigned char idx;
	const wchar_t *tbl;
	wchar_t out;

	/* grab upper byte */
	idx = (in & 0xff00) >> 8;

	/* find pointer to 2nd layer table */
	tbl = toplevel[idx];
	if (!tbl)
		return in;

	/* grab lower byte */
	idx = in & 0xff;

	/* look up character in table */
	out = tbl[idx];
	if (out)
		return out;

	return in;
}