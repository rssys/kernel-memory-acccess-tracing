
#ifndef _MEMTRACE_H
#define _MEMTRACE_H

#include <linux/memtrace_types.h>
#include <linux/mm.h>
#include <linux/irq.h>

extern bool memtrace_init_done;

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
	p->pc[i] = ip; 				\
	p->addr[i] = ad;                        	\
	p->info[i] = (((load)&0x1)|((len)<<3));

#define __memtrace_ret(p, i, ip)  	\
	p->pc[i] = ip; 		\
	p->addr[i] = id; 		\
	p->info[i] = 0x2;

#define __memtrace_call(p, i, ip)  	\
	p->pc[i] = ip; 		\
	p->addr[i] = id; 		\
	p->info[i] = 0x3;

#define __memtrace_wrapper(filter, code) \
	do {                                                                  		\
		if (memtrace_init_done) {                                         	\
			int i, cnt;                                   			\
			struct memtrace_hub *h;                           		\
			struct memtrace_proxy *p;      	                                \
			struct task_struct *cur_t; 					\
			struct memtrace_packet *cur_p; 					\
			h = mt_hub;							\
			cur_t = current; 						\
			for (i = 0; i < MEMTRACE_NUM_PROXY; i++) {                          \
				if (cur_t == READ_ONCE(h->t[i])) {                                     \
					break;                                              \
				}                                                           \
			}                                                                   \
			if (i == MEMTRACE_NUM_PROXY) {                                      \
				return;                                                     \
			} 								\
		        if (!in_task()) { 			\
				return; 						\
			}							\
			filter	 							\
			p = &h->proxy[i];                                                   \
			if (READ_ONCE(p->writing)) { \
				return; \
			}\
			WRITE_ONCE(p->writing, 1); \
			barrier(); \
			cnt = READ_ONCE(p->mem_access_cnt); 				\
			if (likely(cnt < MEMTRACE_NUM_MAX)) {                         \
				unsigned long ip;\
				int pkt_field_idx; \
				ip = _RET_IP_; \
				pkt_field_idx = cnt&0xf;                  \
				cur_p = (struct memtrace_packet*)(p->buf)+(cnt>>4);\
				code 						            \
				WRITE_ONCE(p->mem_access_cnt, cnt+1); \
			} \
			barrier(); \
			WRITE_ONCE(p->writing, 0); \
		}                                                                           \
	} while(0)

#define memtrace(len, load) __memtrace_wrapper(__memtrace_stack_filter, __memtrace(cur_p, pkt_field_idx, ip, addr, len, load))
#define memtraceN(len, load) memtrace(len, load)

#define memtrace_ret __memtrace_wrapper(, __memtrace_ret(cur_p, pkt_field_idx, ip))
#define memtrace_call __memtrace_wrapper(, __memtrace_call(cur_p, pkt_field_idx, ip))


#endif