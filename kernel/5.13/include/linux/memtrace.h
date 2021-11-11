
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

#define LOAD_VALUE_TYPE(dst, type) (dst) = *(type*)(addr)
#define LOAD_VALUE_16(dst) LOAD_VALUE_TYPE(dst, __uint128_t)
#define LOAD_VALUE_8(dst) LOAD_VALUE_TYPE(dst, uint64_t)
#define LOAD_VALUE_4(dst) LOAD_VALUE_TYPE(dst, uint32_t)
#define LOAD_VALUE_2(dst) LOAD_VALUE_TYPE(dst, uint16_t)
#define LOAD_VALUE_1(dst) LOAD_VALUE_TYPE(dst, uint8_t)
#define LOAD_VALUE(dst, len) LOAD_VALUE_##len(dst)

// static void __always_inline memtrace_inline(unsigned long addr, int len, bool read) {
#define memtrace(len, read, ip)                                                             \
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
			if (p->buf_unused >= 0) {                                           \
				int pkt_field_idx = p->mem_access_cnt&0xf;                  \
				LOAD_VALUE(p->cur->val[pkt_field_idx], len);                \
				p->cur->pc[pkt_field_idx] = ip;                             \
				p->cur->addr[pkt_field_idx] = addr;                         \
				p->cur->info[pkt_field_idx] = read;                         \
				p->cur->info[pkt_field_idx] |= len<<1;                      \
				if (pkt_field_idx == 0x0) {                                 \
					p->buf_unused -= sizeof(struct memtrace_packet);    \
				}                                                           \
				if (pkt_field_idx == 0xf) {                                 \
					p->cur++;                                           \
				}                                                           \
				p->mem_access_cnt++;                                        \
			}                                                                   \
			if (fl & X86_EFLAGS_IF) {                                        \
				asm volatile("sti": : :"memory");                           \
			}                                                                   \
		}                                                                           \
	} while(0)

#endif