
#ifndef _MEMTRACE_TYPES_H
#define _MEMTRACE_TYPES_H

#include <linux/types.h>
#include <linux/mm_types.h>
#include <linux/spinlock.h>

#define MEMTRACE_NUM_PROXY 4
#define MEMTRACE_BUF_SIZE (128ul*0x100000ul)
#define MEMTRACE_SIZE (MEMTRACE_BUF_SIZE*MEMTRACE_NUM_PROXY+0x1000)
#define MEMTRACE_START 0xfffffc0000000000ul
#define MEMTRACE_END (MEMTRACE_START + MEMTRACE_SIZE)
#define MEMTRACE_SAVEDIP_LEN 8

struct memtrace_packet {
	uint64_t pc[16];
	// __uint128_t val[16];
	uint64_t addr[16];
	uint32_t info[16];
};

struct memtrace_proxy {
	// buffer
	char *buf;
	struct memtrace_packet *cur;
	int mem_access_cnt;
	uint64_t saved_ip[MEMTRACE_SAVEDIP_LEN];
	uint8_t saved_ip_len;
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
#define MEMTRACE_NUM_MAX ((MEMTRACE_BUF_SIZE/(sizeof(struct memtrace_packet)))*16)

#endif