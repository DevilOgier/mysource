/**
*  This is a simple demon of uio driver.
*  Last modified by
        09-20-2011   Joseph Yang(Yang Honggang)<ganggexiongqi@gmail.com>
*
ԭ�������
�ڵ�һ�����ӵĻ����ϣ��������жϲ��֡����ǣ�����û��ʵ�ʲ����жϵ�Ӳ�����������ǿ��ԡ�ģ�⡰Ӳ���жϡ�
ÿ���жϷ���ʱ��uio_event_notify �������ã��������豸���ж��¼�������()��һ����֪ͨ�������̣�
�����ݿɶ������ԣ�����ͨ���ں˶�ʱ�����������ԵĲ����жϣ����ڶ�ʱ���Ĵ��������
����uio_event_notify���Ӷ�������Ч������Ӳ������ͬ��
*/

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/slab.h> /* kmalloc, kfree */
#include <linux/device.h> /* class_create */
#include <linux/kobject.h> /* kobject_uevent */
#define FREQ HZ

static long freq = FREQ;
static long my_event_count = 0;

struct uio_info kpart_info = {
	.name = "kpart",
	.version = "0.1",
	.irq = UIO_IRQ_NONE,
};

static int drv_kpart_probe(struct device *dev);
static int drv_kpart_remove(struct device *dev);
static struct device_driver uio_dummy_driver = {
	.name = "kpart",
	.bus = &platform_bus_type,
	.probe = drv_kpart_probe,
	.remove = drv_kpart_remove,
};

static struct timer_list poll_timer;// generate interruption

static void drv_kpart_timer(unsigned long data) {
	struct uio_info *info = (struct uio_info *)data;
	unsigned long *addr = (unsigned long *)info->mem[0].addr;
	unsigned long swap = 0;

	if (my_event_count == 0) {
		printk(KERN_EMERG"first timer interrupt \n");
		*addr = my_event_count;
	} else if (my_event_count == 10) {
		printk(KERN_EMERG"timer interrupt happened 10 times\n"
		       "it works well\n");
	}
	swap = *addr;
	if ( swap != my_event_count) {
		printk(KERN_EMERG"counter reset\n");
		my_event_count = swap;
	} else {
		my_event_count++;
		*addr = my_event_count;
    //printk(KERN_EMERG"update counter \n");
	}
    //*addr = my_event_count;
	uio_event_notify(&kpart_info); // gernerate a interrupt here
	mod_timer(&poll_timer, jiffies + freq); // reset the timer
}

static int drv_kpart_probe(struct device *dev) {

	printk(KERN_EMERG"----->  /// drv_kpart_probe( %p)\n", dev);
	kpart_info.mem[0].addr = (unsigned long)kmalloc(1024,GFP_KERNEL);

	if(kpart_info.mem[0].addr == 0)
		return -ENOMEM;
	kpart_info.mem[0].memtype = UIO_MEM_LOGICAL;
	kpart_info.mem[0].size = 1024;
	// for the timer interruption
	kpart_info.irq_flags = UIO_IRQ_CUSTOM;

	if( uio_register_device(dev, &kpart_info)) {
		kfree((void *)kpart_info.mem[0].addr);
		return -ENODEV;
	}
	//initiate and add the timer
	init_timer(&poll_timer);
	poll_timer.data = (unsigned long)&kpart_info;
	poll_timer.function = drv_kpart_timer;
	mod_timer(&poll_timer, jiffies + freq);//set timer

	return 0;
}

static int drv_kpart_remove(struct device *dev) {
	uio_unregister_device(&kpart_info);

	//delet the timer
	del_timer_sync(&poll_timer);

	return 0;
}

static struct platform_device * uio_dummy_device;

static int __init uio_kpart_init(void) {
	uio_dummy_device = platform_device_register_simple("kpart", -1,
	                   NULL, 0);
	printk("&platform_device->dev = (%p)\n", &uio_dummy_device->dev);
	return driver_register(&uio_dummy_driver);

}

static void __exit uio_kpart_exit(void) {
	platform_device_unregister(uio_dummy_device);
	driver_unregister(&uio_dummy_driver);
}

module_init(uio_kpart_init);
module_exit(uio_kpart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benedikt Spranger");
MODULE_DESCRIPTION("UIO dummy driver");

