* Disable `CONFIG_X86_X32`, `CONFIG_DEBUG_INFO_BTF` since they prevent building.
* Disable `CONFIG_KASAN_INLINE`
* For GDB: to [skip timer interrupt](https://stackoverflow.com/questions/64961631/how-to-skip-timer-interrupt-while-debugging-linux), enable `CONFIG_X86_X2APIC`, `CONFIG_PARAVIRT` and `CONFIG_KVM_GUEST`.
