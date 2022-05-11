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
	uint64_t addr[16];
	uint32_t info[16];
};
#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

void parse_memtrace_packet(struct memtrace_packet* cur, int idx) {
	int len;
	bool read;
	if (idx >= 16) {
		return;
	}
	int type = cur->info[idx] & 0x7;
	if (type < 2) {
		len = cur->info[idx]>>3;
		read = (type == 1);
		printf("pc = %p, addr = %p, %c, len = %d\n",
		cur->pc[idx], cur->addr[idx], read?'r':'w', len); 
	} else if (type == 2) {
		printf("pc = %p, ret\n", cur->pc[idx]);
	} else if (type == 3) {
		printf("pc = %p, call\n", cur->pc[idx]);
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