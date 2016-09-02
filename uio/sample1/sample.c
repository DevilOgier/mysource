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
	.irq = UIO_IRQ_NONE, //û��ʹ���жϣ����Գ�ʼΪUIO_IRQ_NONE
};

static int drv_kpart_probe(struct device *dev);
static int drv_kpart_remove(struct device *dev);
static struct device_driver uio_dummy_driver = {
	.name = "kpart",
	.bus = &platform_bus_type,
	.probe = drv_kpart_probe,
	.remove = drv_kpart_remove,
};

// �����uio_info�ĳ�ʼ����uio�豸��ע��
static int drv_kpart_probe(struct device *dev) {
	// ��ʼ���豸��ص� uio_info�ṹ
	printk(KERN_ERR "drv_kpart_probe( %p)\n", dev);

    // �����ڴ�ӳ�����Ҫ���õ�
	kpart_info.mem[0].addr = (unsigned long)kmalloc(1024,GFP_KERNEL);

	if(kpart_info.mem[0].addr == 0)
		return -ENOMEM;
    /*
    memtype������ UIO_MEM_PHYS����ô��ӳ�䵽�û��ռ������
    �豸�������ڴ档Ҳ������UIO_MEM_LOGICAL����ô��ӳ�䵽�û��ռ�����߼��ڴ�
    (����ʹ�� kmalloc������ڴ�)����������UIO_MEM_VIRTUAL,��ô��ӳ�䵽�û��ռ����
    �����ڴ�(������vmalloc������ڴ�)
    */
	kpart_info.mem[0].memtype = UIO_MEM_LOGICAL;
	kpart_info.mem[0].size = 1024;

	// ����uio_register_device ���䲢ע��һ��uio�豸
	if( uio_register_device(dev, &kpart_info))
		return -ENODEV;
	return 0;
}

static int drv_kpart_remove(struct device *dev) {
	printk(KERN_ERR "drv_kpart_remove");
	uio_unregister_device(&kpart_info);

	return 0;
}
/*���ǵ����������������������豸��֮��Ӧ���� Platform  �������Զ�̽�⡰�����������
  ������û��ʵ��Ӳ�����������Ҫ��*/
static struct platform_device * uio_dummy_device;

static int __init uio_kpart_init(void) {
	printk(KERN_ERR "uio_kpart_init");
    //������һ���򵥵�platform�豸
	uio_dummy_device = platform_device_register_simple("kpart", -1,
	                   NULL, 0);

    /*
    ע��device_driver���͵�uio_dummy_driver������bus��
      ������Ҫע��һ�����⣬���� device_driver�ṹ�е�nameΪ��kpart", ���Ǵ�����platform�豸
      ����Ҳ��"kpart"�������ȴ���platform�豸����ע��������������Ϊ���������豸��ע������ʱ��
      ��������name��ƥ���豸��
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
/sys/class/uio/uio0
������ dev
������ device -> ../../../kpart
������ event
������ maps
��   ������ map0
��       ������ addr
��       ������ name
��       ������ offset
��       ������ size
������ name
������ power
��   ������ wakeup
������ subsystem -> ../../../../../class/uio
������ uevent
������ version


[root@zhong8 subsystem]# tree /sys/class/uio/uio0/device/
/sys/class/uio/uio0/device/
������ driver -> ../../../bus/platform/drivers/kpart
������ modalias
������ power
��?? ������ wakeup
������ subsystem -> ../../../bus/platform
������ uevent
������ uio
    ������ uio0
        ������ dev
        ������ device -> ../../../kpart
        ������ event
        ������ maps
        ��?? ������ map0
        ��??     ������ addr
        ��??     ������ name
        ��??     ������ offset
        ��??     ������ size
        ������ name
        ������ power
        ��?? ������ wakeup
        ������ subsystem -> ../../../../../class/uio
        ������ uevent
        ������ version

10 directories, 13 files

[root@zhong8 sample1]# tree /sys/bus/platform/devices/kpart 
/sys/bus/platform/devices/kpart
������ driver -> ../../../bus/platform/drivers/kpart
������ modalias
������ power
��?? ������ wakeup
������ subsystem -> ../../../bus/platform
������ uevent
������ uio
    ������ uio0
        ������ dev
        ������ device -> ../../../kpart
        ������ event
        ������ maps
        ��?? ������ map0
        ��??     ������ addr
        ��??     ������ name
        ��??     ������ offset
        ��??     ������ size
        ������ name
        ������ power
        ��?? ������ wakeup
        ������ subsystem -> ../../../../../class/uio
        ������ uevent
        ������ version

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

