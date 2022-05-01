
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

#define LOAD_VALUE_TYPE(dst, addr, type) (dst) = *(type*)(addr)
#define LOAD_VALUE_16(dst, addr) LOAD_VALUE_TYPE(dst, addr, __uint128_t)
#define LOAD_VALUE_8(dst, addr) LOAD_VALUE_TYPE(dst, addr, uint64_t)
#define LOAD_VALUE_4(dst, addr) LOAD_VALUE_TYPE(dst, addr, uint32_t)
#define LOAD_VALUE_2(dst, addr) LOAD_VALUE_TYPE(dst, addr, uint16_t)
#define LOAD_VALUE_1(dst, addr) LOAD_VALUE_TYPE(dst, addr, uint8_t)
#define LOAD_VALUE(dst, addr, len) LOAD_VALUE_##len(dst, addr)

/*
#define __memtrace_saveip    \         
	if (p->saved_ip_len < MEMTRACE_SAVEDIP_LEN) { \
		p->saved_ip[p->saved_ip_len] = _RET_IP_; \
	} \
	(p->saved_ip_len)++;

#define __memtrace_loadip_read_false                     \
	if (p->saved_ip_len <= MEMTRACE_SAVEDIP_LEN) \
		ip = p->saved_ip[p->saved_ip_len-1]; \
	else \
		ip = 0xffffffffffffffff; \
	p->saved_ip_len--;	

#define __memtrace_loadip_read_true      \
	ip = _RET_IP_;

#define __memtrace_loadip(load) __memtrace_loadip_read_##load
*/

#define __memtrace_loadip      \
	ip = _RET_IP_;

#define __memtrace(len, load) \
	unsigned long ip;\
	__memtrace_loadip \
	int pkt_field_idx = p->mem_access_cnt&0xf;                  \
	LOAD_VALUE(p->cur->val[pkt_field_idx], addr, len);          \
	p->cur->pc[pkt_field_idx] = ip; \
	p->cur->addr[pkt_field_idx] = addr;                         \
	p->cur->info[pkt_field_idx] = load;                         \
	p->cur->info[pkt_field_idx] |= len<<1;                      \
	if (pkt_field_idx == 0xf) {                                 \
		p->cur++;                                           \
	}                                                           \
	p->mem_access_cnt++;

#define __memtraceN(len, load)                                      \
	unsigned long n, ip;\
	__memtrace_loadip \
	while ((p->mem_access_cnt < MEMTRACE_NUM_MAX) && (len != 0)) {       \
		int pkt_field_idx = p->mem_access_cnt&0xf;                  \
		uint8_t addr_low = addr & 0xf; \
		uint8_t aligned = (addr_low & (addr_low-1)) ^ addr_low; \
		aligned = (aligned == 0 ? 16 : aligned); \
		while (aligned > len) {aligned >>= 1;} \
		switch (aligned) {  \
			case 16:\
				LOAD_VALUE_16(p->cur->val[pkt_field_idx], addr); \
				n = 16; \
				break; \
			case 8:\
				LOAD_VALUE_8(p->cur->val[pkt_field_idx], addr); \
				n = 8; \
				break; \
			case 4:\
				LOAD_VALUE_4(p->cur->val[pkt_field_idx], addr); \
				n = 4; \
				break; \
			case 2:\
				LOAD_VALUE_2(p->cur->val[pkt_field_idx], addr); \
				n = 2; \
				break; \
			default:\
				LOAD_VALUE_1(p->cur->val[pkt_field_idx], addr);\
				n = 1;\
				break;\
		}\
		p->cur->pc[pkt_field_idx] = ip; \
		p->cur->addr[pkt_field_idx] = addr;                         \
		p->cur->info[pkt_field_idx] = load;                         \
		p->cur->info[pkt_field_idx] |= n<<1;                      \
		if (pkt_field_idx == 0xf) {                                 \
			p->cur++;                                           \
		}                                                           \
		p->mem_access_cnt++;                                        \
		addr += n;\
		len -= n;\
	}


#define __memtrace_wrapper(code) \
	do {                                                                                \
		if (memtrace_init_done) {                                                   \
			int i;                                                              \
			struct memtrace_hub *h;                                             \
			struct memtrace_proxy *p;                                           \
			unsigned long rsp, cr3, fl;                                         \
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
			rsp = read_rsp();                                                   \
			if ((rsp&(-1l<<(PAGE_SHIFT+THREAD_SIZE_ORDER))) ==                  \
				(addr&(-1l<<(PAGE_SHIFT+THREAD_SIZE_ORDER)))) {             \
				return;                                                     \
			}                                                                   \
			asm volatile("# __raw_save_flags\n\t"                               \
					"pushf ; pop %0"                                    \
					: "=rm" (fl)                                        \
					: /* no input */                                    \
					: "memory");                                        \
			asm volatile("cli": : :"memory");                                   \
			p = &h->proxy[i];                                                   \
			if (p->mem_access_cnt < MEMTRACE_NUM_MAX) {                         \
				code \
			} \
			if (fl & X86_EFLAGS_IF) {                                        \
				asm volatile("sti": : :"memory");                           \
			}                                                                   \
		}                                                                           \
	} while(0)

// #define memtrace_saveip __memtrace_wrapper(__memtrace_saveip)
#define memtraceN(len, load) __memtrace_wrapper(__memtraceN(len, load))
#define memtrace(len, load) __memtrace_wrapper(__memtrace(len, load))

#endif