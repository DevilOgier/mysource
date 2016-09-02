/*
usage: Compiling this file by make 
and using the following command to insert the mode which the make generated just now

command: sudo insmod filename.ko irq=1 devname=myirq

This interrupt shared the one irq with keyboard

http://edsionte.com/techblog/archives/1521

*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

static int irq;
static char* devname;
static struct tasklet_struct mytasklet;

module_param(irq,int,0644);
module_param(devname,charp,0644);

struct myirq
{
	int devid;
};
struct myirq mydev={1119};

static void mytasklet_handler(unsigned long data)
{
	printk("tasklet is wroking..\n");
}

static irqreturn_t myirq_handler(int irq,void* dev)
{
	struct myirq mydev;
	static int count=0;
	mydev=*(struct myirq*)dev;
	printk("key:%d..\n",count+1);
	printk("devid:%d ISR is working..\n",mydev.devid);
	printk("Bottom half will be working..\n");
	tasklet_init(&mytasklet,mytasklet_handler,0);
	tasklet_schedule(&mytasklet);
	printk("ISR is leaving..\n");
	count++;
	return IRQ_HANDLED;
}

static int __init myirq_init()
{
	printk("Module is working..\n");
	if(request_irq(irq,myirq_handler,IRQF_SHARED,devname,&mydev)!=0)
	{
		printk("%s request IRQ:%d failed..\n",devname,irq);
		return -1;
	}
	printk("%s rquest IRQ:%d success..\n",devname,irq);
	return 0;
}

static void __exit myirq_exit()
{
	printk("Module is leaving..\n");
	free_irq(irq,&mydev);
	printk("%s request IRQ:%d success..\n",devname,irq);
}



module_init(myirq_init);
module_exit(myirq_exit);
MODULE_LICENSE("GPL");

/*
1.通过cat /proc/interrupts查看中断号，以确定一个即将要共享的中断号。本程序因为是与键盘共享1号中断线，因此irq=1；

2.使用如下命令就可以插入内核：

sudo insmod myirq.ko irq=1 devname=myirq

3.再次查看/proc/interrupts文件，可以发现1号中断线对应的的设备名处多了myirq设备名；

4.dmesg查看内核日志文件，可看到在中断处理程序中所显示的信息；

5.卸载内核模块；
*/
