//#define DEBUG
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>
#include "hello1.h"

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

MODULE_AUTHOR("Sviatoslav Zakharov, IO-24 <9818138@gmail.com>");
MODULE_DESCRIPTION("AK2, lab 6 (hello1)");
MODULE_LICENSE("Dual BSD/GPL");

struct hello_time {
	ktime_t start_time;
	ktime_t end_time;
	ktime_t duration;
	struct list_head list;
};

static LIST_HEAD(hello_list);

void print_hello(unsigned int repeat) {
    int i;
    struct hello_time *entry;

    for (i = 0; i < repeat; i++) {
        entry = kmalloc(sizeof(*entry), GFP_KERNEL);
        if (!entry) {
            pr_err("Failed to allocate memory!\n");
            return;
        }
        entry->start_time = ktime_get();
	pr_info("Hello, world! (iteration %d)\n", i+1);
	entry->end_time = ktime_get();
	entry->duration = ktime_sub(entry->end_time, entry->start_time);
	list_add(&entry->list, &hello_list);
    }
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1 module loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_time *pos, *tmp;
	ktime_t total_duration = ktime_set(0, 0);
	pr_debug("=== start dumping timestamps ===\n");
	pr_debug("--- Timestamps (ns) ---\n");
	list_for_each_entry_safe(pos, tmp, &hello_list, list) {
		pr_debug("Iteration: start=%lld, duration=%lld\n",
ktime_to_ns(pos->start_time),
ktime_to_ns(pos->duration));
		total_duration = ktime_add(total_duration, pos->duration);
		list_del(&pos->list);
		kfree(pos);
	}
	pr_debug("--- Total print time: %lld ns ---\n", ktime_to_ns(total_duration));
	pr_debug("=== end dumping timestamps ===\n");
	pr_info("hello1 module unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
