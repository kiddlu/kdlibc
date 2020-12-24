#include <linux/module.h>
#include <linux/kernel.h>

int init_hello_module(void)
{
    printk("***************Start***************\n");
    printk("Hello World! Start of hello world module!\n");
    return 0;
}


MODULE_LICENSE("Dual BSD/GPL");
module_init(init_hello_module);
