#include <linux/module.h>
#include <linux/kernel.h>

void exit_hello_module(void)
{
    printk("***************End***************\n");
    printk("Hello World! End of hello world module!\n");
}

module_exit(exit_hello_module);
