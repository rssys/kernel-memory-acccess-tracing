
#ifndef _MEMTRACE_TYPES_H
#define _MEMTRACE_TYPES_H

#include <linux/types.h>
#include <linux/mm_types.h>
#include <linux/spinlock.h>

#define MEMTRACE_NUM_PROXY 1
#define MEMTRACE_PKT_NUM_MAX (0x100000)
#define MEMTRACE_NUM_MAX (MEMTRACE_PKT_NUM_MAX*16)
#define MEMTRACE_START 0xfffffc0000000000ul

struct memtrace_packet {
	uint64_t pc[16];
	uint64_t addr[16];
	uint32_t info[16];
};

#define MEMTRACE_BUF_SIZE (((MEMTRACE_PKT_NUM_MAX*sizeof(struct memtrace_packet))+PAGE_SIZE-1)&(0xFFFFFFFFFFFFFFFF<<PAGE_SHIFT))
#define MEMTRACE_SIZE (MEMTRACE_BUF_SIZE*MEMTRACE_NUM_PROXY+PAGE_SIZE)
#define MEMTRACE_END (MEMTRACE_START + MEMTRACE_SIZE)

struct memtrace_proxy {
	// buffer
	char *buf;
	struct memtrace_packet *cur;
	int mem_access_cnt;
	bool free;
};

struct memtrace_hub {
	// TODO: switch to atomic value
	unsigned long cr3[MEMTRACE_NUM_PROXY];
	struct memtrace_proxy proxy[MEMTRACE_NUM_PROXY];
	spinlock_t proxy_lock;
};

#define mt_hub ((struct memtrace_hub *)(MEMTRACE_START))

#define MEMTRACE_BUF_START ((MEMTRACE_START+sizeof(struct memtrace_hub)+15)&0xFFFFFFFFFFFFFFF0l)

#endif