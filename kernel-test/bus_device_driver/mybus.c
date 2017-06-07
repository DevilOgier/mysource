#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/string.h>
static char *version = "version 1.0";
//�����ж�ָ�������������Ƿ��ܴ���ָ�����豸��

static int my_match(struct device *dev, struct device_driver *driver) {
    return !strncmp(dev->bus_id, driver->name, strlen(driver->name));
}
static int my_bus_release(struct device *dev) {
    return 0;
}
static ssize_t show_bus_version(struct bus_type *bus, char *buf) {
    return sprintf(buf, PAGE_SIZE, "%s\n", version);
}
static BUS_ATTR(version, S_IRUGO, show_bus_version, NULL);

struct device my_bus = {//���������豸

    //.bus_id = "my_bus0",
    .release = my_bus_release,
};
EXPORT_SYMBOL(my_bus);
struct bus_type my_bus_type = { //������������

    .name = "my_bus",
    .match = my_match,
};
EXPORT_SYMBOL(my_bus_type);
static int __init my_bus_init(void){
    int ret;
    ret = bus_register(&my_bus_type);//ע������

    if(ret)    
        printk("bus_register failed!\n");
    if(bus_create_file(&my_bus_type, &bus_attr_version))//������������    

        printk("Creat bus failed!\n");
    ret = device_register(&my_bus);//ע�������豸

    if (ret)
        printk("device_register failed!\n");
    return ret;
}
static void __exit my_bus_exit(void) {
   bus_unregister(&my_bus_type);//ɾ����������

    device_unregister(&my_bus);//ɾ�������豸

}
module_init(my_bus_init);
module_exit(my_bus_exit);
MODULE_AUTHOR("Fany");
MODULE_LICENSE("GPL");