#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>

MODULE_AUTHOR("Edsionte Wu");
MODULE_LICENSE("GPL");

#define MYCDEV_MAJOR 231 /*the predefined mycdev's major devno*/
#define MYCDEV_SIZE 100

static int mycdev_open(struct inode *inode, struct file *fp)
{
	return 0;
}

static int mycdev_release(struct inode *inode, struct file *fp)
{
	return 0;
}

static ssize_t mycdev_read(struct file *fp, char __user *buf, size_t size, loff_t *pos)
{
	unsigned long p = *pos;
	unsigned int count = size;
	int i;
	char kernel_buf[MYCDEV_SIZE] = "This is mycdev!";

	if(p >= MYCDEV_SIZE)
		return -1;
	if(count > MYCDEV_SIZE)
		count = MYCDEV_SIZE - p;

	if (copy_to_user(buf, kernel_buf, count) != 0) {
		printk("read error!\n");
		return -1;
	}

	/*
	for (i = 0; i < count; i++) {
		__put_user(i, buf);//write 'i' from kernel space to user space's buf;
		buf++;
	}
	*/

	printk("edsionte's reader: %d bytes was read...\n", count);
	return count;

}

static ssize_t mycdev_write(struct file *fp, const char __user *buf, size_t size, loff_t *pos)
{
	return size;
}

/*filling the mycdev's file operation interface in the struct file_operations*/
static const struct file_operations mycdev_fops =
{
	.owner = THIS_MODULE,
	.read = mycdev_read,
	.write = mycdev_write,
	.open = mycdev_open,
	.release = mycdev_release,
};

/*module loading function*/
static int __init mycdev_init(void)
{
	int ret;

	printk("mycdev module is staring..\n");
    
    // 老的方法
	ret=register_chrdev(MYCDEV_MAJOR,"cdev",&mycdev_fops);
	if(ret<0)
	{
		printk("register failed..\n");
		return 0;
	}
	else
	{
		printk("register success..\n");
	}

	return 0;
}

/*module unloading function*/
static void __exit mycdev_exit(void)
{
	printk("mycdev module is leaving..\n");
    // 老的方法
	unregister_chrdev(MYCDEV_MAJOR,"cdev");
}

module_init(mycdev_init);
module_exit(mycdev_exit);

/*
http://edsionte.com/techblog/archives/2977

1.make编译mycdev.c文件，并插入到内核；
2.通过cat /proc/devices 查看系统中未使用的字符设备主设备号，比如当前231未使用；
3.创建设备文件结点：sudo mknod /dev/mycdev c 231 0；具体使用方法通过man mknod命令查看；
4.修改设备文件权限：sudo chmod 777 /dev/mycdev；
5.以上成功完成后，编译本用户态测试程序；运行该程序查看结果；
6.通过dmesg查看日志信息；
*/