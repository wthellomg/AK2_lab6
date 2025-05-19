#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include "hello1.h"

MODULE_AUTHOR("Sviatoslav Zakharov, IO-24 <9818138@gmail.com>");
MODULE_DESCRIPTION("AK2, Lab 5 (hello2)");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int repeat = 1;
module_param(repeat, uint, 0644);
MODULE_PARM_DESC(repeat, "Number of times to print 'Hello, world!'");

static int __init hello2_init(void) {
    if (repeat == 0 || (repeat >= 5 && repeat <= 10)) {
        pr_warn("Warning: unusual repeat value (%d)\n", repeat);
    } else if (repeat > 10) {
        pr_err("Error: repeat value too large (%d)\n", repeat);
        return -EINVAL;
    }

    print_hello(repeat);  // Call hello1's function
    pr_info("hello2 module loaded\n");
    return 0;
}

static void __exit hello2_exit(void) {
    pr_info("hello2 module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
