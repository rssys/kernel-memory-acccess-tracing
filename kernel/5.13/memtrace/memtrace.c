#include <linux/memtrace_types.h>
#include <linux/mm.h>
#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/spinlock.h>
#include <linux/irq.h>

#include "memtrace.h"

static inline unsigned long read_cr3(void) {
	unsigned long cr3;
	asm volatile("mov %%cr3,%0\n\t" : "=r" (cr3));
	return cr3;
}

static int start_memtrace(void) {
	// TODO: mlock all
	struct memtrace_hub *h = mt_hub;
	unsigned long cr3, irq_state;
	int i, ret;

	ret = 0;
	cr3 = read_cr3();
	for (i = 0; i < MEMTRACE_NUM_PROXY; i++) {
		if (h->cr3[i] == cr3) {
			ret = -EPERM;
			goto out;
		}
	}
	spin_lock_irqsave(&h->proxy_lock, irq_state);
	for (i = 0; i < MEMTRACE_NUM_PROXY; i++) {
		if (h->proxy[i].free) {
			break;
		}
	}
	if (unlikely(i == MEMTRACE_NUM_PROXY)) {
		spin_unlock_irqrestore(&h->proxy_lock, irq_state);
		ret = -EBUSY;
		goto out;
	}
	h->proxy[i].free = false;
	spin_unlock_irqrestore(&h->proxy_lock, irq_state);
	printk(KERN_DEBUG "memtrace: process cr3 = 0x%lx start memory tracing in proxy %d.", cr3, i);
	h->cr3[i] = cr3;
out:
	return ret;
}


static int stop_memtrace(void __user *buf) {
	struct memtrace_hub *h = mt_hub;
	struct memtrace_proxy *p;
	int i, ret, trace_cnt;
	unsigned long cr3, irq_state, trace_len;
	
	// Reset cr3 first so we can use kernel functions without creating trace noise.
	cr3 = read_cr3();
	for (i = 0; i < MEMTRACE_NUM_PROXY; i++) {
		if (h->cr3[i] == cr3) {
			break;
		}
	}
	if (i == MEMTRACE_NUM_PROXY) {
		ret = -EPERM;
		goto out;
	}
	h->cr3[i] = 0;
	p = &h->proxy[i];
	trace_len = MEMTRACE_BUF_SIZE-p->buf_unused;
	trace_cnt = p->mem_access_cnt;
	if (copy_to_user(buf, (const void *)p->buf, trace_len)) {
		ret = -EFAULT;
	} else {
		ret = trace_cnt;
	}
	// Release proxy.
	spin_lock_irqsave(&h->proxy_lock, irq_state);
	p->buf_unused = MEMTRACE_BUF_SIZE;
	p->cur = (struct memtrace_packet *)p->buf;
	p->mem_access_cnt = 0;
	p->free = true;
	spin_unlock_irqrestore(&h->proxy_lock, irq_state);
	printk(KERN_DEBUG "memtrace: process cr3 = 0x%lx in proxy %d stops memory tracing, count = %d", cr3, i, trace_cnt);
out:
	return ret;
}


SYSCALL_DEFINE2(mtrace, unsigned long, op, unsigned long, addr) {
	int ret;
	ret = 0;
	if (op == 0) {
		ret = start_memtrace();
	} else if (op == 1) {
		// TODO: check how read/write cast types
		ret = stop_memtrace((__user void *)addr);
	} else {
		ret = -EINVAL;
	}
	return ret;
}