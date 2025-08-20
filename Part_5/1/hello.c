#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros

MODULE_LICENSE("NONAME_LICENSE");
MODULE_AUTHOR("FilatovR");
MODULE_DESCRIPTION("A bespoke kernel module for Eltex");

static int __init hello_init(void)
{
    printk(KERN_INFO "Got into kernel!\n");
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Leaving kernel!\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
