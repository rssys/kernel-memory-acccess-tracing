#include <sys/syscall.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define BUF_SIZE (32 << 20)
struct memtrace_packet {
	uint64_t pc[16];
	__uint128_t val[16];
	uint64_t addr[16];
	uint8_t info[16];
};
#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

void parse_memtrace_packet(struct memtrace_packet* cur, int idx) {
	int len;
	bool read;
	if (idx >= 16) {
		return;
	}
	len = cur->info[idx]>>1;
	read = (cur->info[idx] & 1) == 1;
	if (len == 16) {
		printf("pc = %p, addr = %p, %c, len = 16, val (high) = %llx, val (low) = %llx\n",
		cur->pc[idx], cur->addr[idx], read?'r':'w', (uint64_t)(cur->val[idx]>>64), (uint64_t)((cur->val[idx]<<64)>>64)); 
	} else {
		printf("pc = %p, addr = %p, %c, len = %d, val = %llx\n",
		cur->pc[idx], cur->addr[idx], read?'r':'w', len, (uint64_t)((cur->val[idx]<<64)>>64)); 
	}


}
int main(int argc, char *argv[]) {
	void *buf;
	int i, num_print;
	struct memtrace_packet* cur;
	if (argc == 2) {
		num_print = atoi(argv[1]);
	} else {
		num_print = 32;
	}
	system("echo 8 > /proc/sys/kernel/printk");
	buf = mmap(0, BUF_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
	if (buf == MAP_FAILED) {
		handle_error("mmap");
	}
	int ret;
	if ((ret = syscall(667, 0, 0)) < 0) {
		handle_error("memtrace start");
	}
	printf("Hello!\n");
	if ((ret = syscall(667, 1, buf)) < 0) {
		handle_error("memtrace stop");
	}
	printf("receive %d memory access traces!\n", ret);
	cur = (struct memtrace_packet*)buf;
	for (i = 0; i < num_print; i++) {
		parse_memtrace_packet(cur, i%16);
		if ((i % 16) == 15) {
			cur++;
		}
	}
	return 0;
}