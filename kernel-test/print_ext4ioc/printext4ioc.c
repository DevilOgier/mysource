#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <fs/ext4/ext4.h>


static int __init printext4ioc_init(void)
{

	printk("------------------------\n");
	printk("EXT4_IOC_MOVE_EXT:%ld\n",EXT4_IOC_MOVE_EXT);	
	return 0;
}

static void __exit printext4ioc_exit(void)
{
   printk("<1>exit ---------------------!\n");
}

module_init(printext4ioc_init);
module_exit(printext4ioc_exit);
MODULE_LICENSE("GPL");


/*
insmod printext4ioc.ko 
dmesg

*/