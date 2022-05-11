
#ifndef _MEMTRACE_H
#define _MEMTRACE_H

#include <linux/memtrace_types.h>
#include <linux/mm.h>
#include <linux/irq.h>

extern bool memtrace_init_done;

static __always_inline unsigned long read_cr3(void) {
	unsigned long cr3;
	asm volatile("mov %%cr3,%0\n\t" : "=r" (cr3));
	return cr3;
}

static __always_inline unsigned long read_rsp(void) {
	unsigned long rsp;
	asm volatile("mov %%rsp,%0\n\t" : "=r" (rsp));
	return rsp;
}

#define __memtrace_stack_filter \
	do { 									\
		unsigned long rsp;						\
		rsp = read_rsp();                                               \
		if ((rsp&(-1l<<(PAGE_SHIFT+THREAD_SIZE_ORDER))) ==              \
			(addr&(-1l<<(PAGE_SHIFT+THREAD_SIZE_ORDER)))) {         \
			return;                                                 \
		} 								\
	} while (0);							

#define __memtrace(p, i, ip, ad, len, load) 		\
	p->cur->pc[i] = ip; 				\
	p->cur->addr[i] = ad;                        	\
	p->cur->info[i] = (((load)&0x1)|((len)<<3));

#define __memtrace_ret(p, i, ip)  	\
	p->cur->pc[i] = ip; 		\
	p->cur->info[i] = 0x2;

#define __memtrace_call(p, i, ip)  	\
	p->cur->pc[i] = ip; 		\
	p->cur->info[i] = 0x3;

#define __memtrace_wrapper(filter, code) \
	do {                                                                                \
		if (memtrace_init_done) {                                                   \
			int i;                                                              \
			struct memtrace_hub *h;                                             \
			struct memtrace_proxy *p;                                           \
			unsigned long cr3, fl;                                         \
			cr3 = read_cr3();                                                   \
			h = mt_hub;                                                         \
			for (i = 0; i < MEMTRACE_NUM_PROXY; i++) {                          \
				if (cr3 == h->cr3[i]) {                                     \
					break;                                              \
				}                                                           \
			}                                                                   \
			if (i == MEMTRACE_NUM_PROXY) {                                      \
				return;                                                     \
			}                                                                   \
			filter	 							\
			asm volatile("# __raw_save_flags\n\t"                               \
					"pushf ; pop %0"                                    \
					: "=rm" (fl)                                        \
					: /* no input */                                    \
					: "memory");                                        \
			asm volatile("cli": : :"memory");                                   \
			p = &h->proxy[i];                                                   \
			if (p->mem_access_cnt < MEMTRACE_NUM_MAX) {                         \
				unsigned long ip;\
				int pkt_field_idx; \
				ip = _RET_IP_; \
				pkt_field_idx = p->mem_access_cnt&0xf;                  \
				code 						            \
				if (pkt_field_idx == 0xf) {                                 \
					p->cur++;                                           \
				}                                                           \
				p->mem_access_cnt++; \
			} \
			if (fl & X86_EFLAGS_IF) {                                        \
				asm volatile("sti": : :"memory");                           \
			}                                                                   \
		}                                                                           \
	} while(0)

#define memtrace(len, load) __memtrace_wrapper(__memtrace_stack_filter, __memtrace(p, pkt_field_idx, ip, addr, len, load))
#define memtraceN(len, load) memtrace(len, load)

#define memtrace_ret __memtrace_wrapper(, __memtrace_ret(p, pkt_field_idx, ip))
#define memtrace_call __memtrace_wrapper(, __memtrace_call(p, pkt_field_idx, ip))


#endif