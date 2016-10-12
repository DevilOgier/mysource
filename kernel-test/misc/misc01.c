#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

/*
http://blog.csdn.net/yao_guet/article/details/6591322

*/

/////////////////////////////////////////////////
MODULE_LICENSE("Dual BSD/GPL");
int open_state = 0;
/////////////////////////////////////////////////
int misc01_open(struct inode *inode, struct file *filp)
{
    if (open_state == 0)
    {
        open_state = 1;
        printk("misc01 open!\n");
        return 0;
    }
    printk("misc01 has been open!\n");
    return -1;
}

int misc01_release(struct inode *inode, struct file *filp)
{
    if (open_state == 1)
    {
        open_state = 0;
        printk("misc01 release!\n");
        return 0;
    }
    printk("misc01 has not been open yet!\n");
    return -1;
}

ssize_t misc01_read(struct file *filp, char *buf,
        size_t count, loff_t fpos)
{
    printk("misc01 read!\n");
    return 0;
}

ssize_t misc01_write(struct file *filp, char *buf,
        size_t count, loff_t fpos)
{
    printk("misc01 write!\n");
    return 0;
}

int misc01_ioctl(struct inode *inode, struct file *filp,
        unsigned int cmd, unsigned long arg)
{
    printk("ioctl is called!\n");
    printk("cmd:%d arg:%d\n", cmd, arg);
    return 0;
}
/////////////////////////////////////////////////
struct file_operations fops = 
{
    .owner      =   THIS_MODULE,
    .open       =   misc01_open,
    .release    =   misc01_release,
    .write      =   misc01_write,
    .read       =   misc01_read,
    .ioctl      =   misc01_ioctl
};

struct miscdevice dev = 
{
    .minor  =   MISC_DYNAMIC_MINOR,
    .fops    =   &fops,
    .name   =   "misc01",
    .nodename = "misc01_node"
};

int setup_misc01(void)
{
    
    return misc_register(&dev);
}
/////////////////////////////////////////////////
static int __init misc01_init(void)
{
    printk("misc01 init!\n");
    return setup_misc01();
}

static void __exit misc01_exit(void)
{
    printk("misc01 exit!\n");
    misc_deregister(&dev);
}

/////////////////////////////////////////////////
module_init(misc01_init);
module_exit(misc01_exit);
