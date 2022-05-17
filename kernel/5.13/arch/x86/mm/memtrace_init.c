
#include <linux/memtrace_types.h>
#include <linux/mm.h>

// This file should be excluded from KASAN instrumentation
// 1. Don't need to trace this file
// 2. Not sure if KASAN has set up shadow memory for our buffer. (should refer to pfn_mapped)

bool memtrace_init_done;

void __init memtrace_init(void) {
	int ret, i;
	char *buf;
	ret = vmemmap_populate(MEMTRACE_START, MEMTRACE_END, NUMA_NO_NODE, NULL);
	if (ret < 0) {
		panic("memtrace: cannot allocate memory!");
	}
	memset(mt_hub, 0, sizeof(struct memtrace_hub));
	spin_lock_init(&mt_hub->proxy_lock);

	buf = (char *)MEMTRACE_BUF_START;
	for (i = 0; i < MEMTRACE_NUM_PROXY; i++) {
		mt_hub->proxy[i].buf = buf;
		mt_hub->proxy[i].free = true;
		mt_hub->proxy[i].writing = 0;
		buf += MEMTRACE_BUF_SIZE;
	}
	memtrace_init_done = true;
	printk(KERN_INFO "memtrace init!");
}
