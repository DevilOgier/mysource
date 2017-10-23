/**
*  This is a simple demon of uio driver.
*  Last modified by
        09-05-2011   Joseph Yang(Yang Honggang)<ganggexiongqi@gmail.com>
* modprobe uio
*/

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/slab.h> /* kmalloc, kfree */


struct uio_info kpart_info = {
	.name = "kpart",
	.version = "0.1",
	.irq = UIO_IRQ_NONE, //没有使用中断，所以初始为UIO_IRQ_NONE
};

static int drv_kpart_probe(struct device *dev);
static int drv_kpart_remove(struct device *dev);
static struct device_driver uio_dummy_driver = {
	.name = "kpart",
	.bus = &platform_bus_type,
	.probe = drv_kpart_probe,
	.remove = drv_kpart_remove,
};

// 完成了uio_info的初始化和uio设备的注册
static int drv_kpart_probe(struct device *dev) {
	// 初始化设备相关的 uio_info结构
	printk(KERN_ERR "drv_kpart_probe( %p)\n", dev);

    // 这是内存映射必须要设置的
	kpart_info.mem[0].addr = (unsigned long)kmalloc(1024,GFP_KERNEL);

	if(kpart_info.mem[0].addr == 0)
		return -ENOMEM;
    /*
    memtype可以是 UIO_MEM_PHYS，那么被映射到用户空间的是你
    设备的物理内存。也可以是UIO_MEM_LOGICAL，那么被映射到用户空间的是逻辑内存
    (比如使用 kmalloc分配的内存)。还可以是UIO_MEM_VIRTUAL,那么被映射到用户空间的是
    虚拟内存(比如用vmalloc分配的内存)
    */
	kpart_info.mem[0].memtype = UIO_MEM_LOGICAL;
	kpart_info.mem[0].size = 1024;

	// 调用uio_register_device 分配并注册一个uio设备
	if( uio_register_device(dev, &kpart_info))
		return -ENODEV;
	return 0;
}

static int drv_kpart_remove(struct device *dev) {
	printk(KERN_ERR "drv_kpart_remove");
	uio_unregister_device(&kpart_info);

	return 0;
}
/*我们的驱动不存在真正的物理设备与之对应。而 Platform  驱动“自动探测“，这个特性是
  我们在没有实际硬件的情况下需要的*/
static struct platform_device * uio_dummy_device;

static int __init uio_kpart_init(void) {
	printk(KERN_ERR "uio_kpart_init");
    //创建了一个简单的platform设备
	uio_dummy_device = platform_device_register_simple("kpart", -1,
	                   NULL, 0);

    /*
    注册device_driver类型的uio_dummy_driver变量到bus。
      这里需要注意一个问题，就是 device_driver结构中的name为“kpart", 我们创建的platform设备
      名称也是"kpart"。而且先创建platform设备，后注册驱动。这是因为，创建好设备后，注册驱动时，
      驱动依靠name来匹配设备。
    */
	return driver_register(&uio_dummy_driver);
}

static void __exit uio_kpart_exit(void) {
    printk(KERN_ERR "uio_kpart_exit");
	platform_device_unregister(uio_dummy_device);
	driver_unregister(&uio_dummy_driver);
}

module_init(uio_kpart_init);
module_exit(uio_kpart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benedikt Spranger");
MODULE_DESCRIPTION("UIO dummy driver");

/*
tree /sys/class/uio/uio0
/sys/class/uio/uio0
├── dev
├── device -> ../../../kpart
├── event
├── maps
│   └── map0
│       ├── addr
│       ├── name
│       ├── offset
│       └── size
├── name
├── power
│   └── wakeup
├── subsystem -> ../../../../../class/uio
├── uevent
└── version


[root@zhong8 subsystem]# tree /sys/class/uio/uio0/device/
/sys/class/uio/uio0/device/
├── driver -> ../../../bus/platform/drivers/kpart
├── modalias
├── power
│?? └── wakeup
├── subsystem -> ../../../bus/platform
├── uevent
└── uio
    └── uio0
        ├── dev
        ├── device -> ../../../kpart
        ├── event
        ├── maps
        │?? └── map0
        │??     ├── addr
        │??     ├── name
        │??     ├── offset
        │??     └── size
        ├── name
        ├── power
        │?? └── wakeup
        ├── subsystem -> ../../../../../class/uio
        ├── uevent
        └── version

10 directories, 13 files

[root@zhong8 sample1]# tree /sys/bus/platform/devices/kpart 
/sys/bus/platform/devices/kpart
├── driver -> ../../../bus/platform/drivers/kpart
├── modalias
├── power
│?? └── wakeup
├── subsystem -> ../../../bus/platform
├── uevent
└── uio
    └── uio0
        ├── dev
        ├── device -> ../../../kpart
        ├── event
        ├── maps
        │?? └── map0
        │??     ├── addr
        │??     ├── name
        │??     ├── offset
        │??     └── size
        ├── name
        ├── power
        │?? └── wakeup
        ├── subsystem -> ../../../../../class/uio
        ├── uevent
        └── version

10 directories, 13 files

[root@zhong8 ~]# cat /sys/class/uio/uio0/maps/map0/addr
0xffff880138909000
[root@zhong8 ~]# cat /sys/class/uio/uio0/maps/map0/size 
0x400
[root@zhong8 subsystem]# cat /sys/class/uio/uio0/maps/map0/name 

[root@zhong8 subsystem]# cat /sys/class/uio/uio0/maps/map0/offset 
0x0
[root@zhong8 ~]# cat /sys/class/uio/uio0/
dev        device/    event      maps/      name       power/     subsystem/ uevent     version    
[root@zhong8 ~]# cat /sys/class/uio/uio0/uevent 
MAJOR=248
MINOR=0
DEVNAME=uio0
[root@zhong8 ~]# cat /sys/class/uio/uio0/version 
0.1
[root@zhong8 ~]# cat /sys/class/uio/uio0/name 
kpart
[root@zhong8 ~]# cat /sys/class/uio/uio0/event 
0
[root@zhong8 ~]# cat /sys/class/uio/uio0/dev
248:0


*/

