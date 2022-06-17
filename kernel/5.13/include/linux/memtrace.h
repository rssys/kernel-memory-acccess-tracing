
#ifndef _MEMTRACE_H
#define _MEMTRACE_H

notrace void instrument_memtrace_loadN(unsigned long addr, size_t size);
notrace void instrument_memtrace_storeN(unsigned long addr, size_t size);
notrace void instrument_memtrace_load4(unsigned long addr);
notrace void instrument_memtrace_store4(unsigned long addr);

#endif