#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>

MODULE_AUTHOR("wwxxxxll");
MODULE_LICENSE("Dual BSD/GPL");

#define DEBUG_EN//debugʹ�ܿ���

#define PRINT_COLOR_RESET     "\33[0m"
#define PRINT_COLOR_BLACK     "\33[30m"
#define PRINT_COLOR_RED       "\33[31m"
#define PRINT_COLOR_GREEN     "\33[32m"
#define PRINT_COLOR_YELLOW    "\33[33m"
#define PRINT_COLOR_BLUE      "\33[34m"
#define PRINT_COLOR_MAGENTA   "\33[35m"
#define PRINT_COLOR_CYAN      "\33[36m"
#define PRINT_COLOR_WHITE     "\33[37m"
//ѡ���������ɫ��û�й���ͬѧ�ɽ��һ�£���Ū�ˣ�
//KERN_DEBUG�ն���ʾû�еĻ���kmesg
#define Printk(COLOR, ARGs...) \
    printk(COLOR);             \
    printk(KERN_DEBUG ARGs);   \
    printk(PRINT_COLOR_RESET)  \

#ifdef DEBUG_EN
#define MY_DEBUG(ARGs...) Printk(PRINT_COLOR_YELLOW, ARGs)
#else
#define MY_DEBUG(ARGs...) ;
#endif

#define CHILD_NUM 3
#define MY_BIN_SIZE 1
#define to_my_kobj(data) container_of(data, struct my_kobj, kobj)
#define to_my_kobj_child(data)  container_of(data, struct my_kobj_child, kobj_true)
#define to_my_kobj_child2(data)  container_of(data, struct my_kobj_child, kobj_cacogenic)

//���ȼ򵥶�ΪPAGE_SIZE��
struct child_info {
	char name[PAGE_SIZE];
	int age;
	int height;
	int weight;
};//̫��c++��ĸ����ˣ��ܲ��ˣ�

//bin��link����sysfsһ������ûϸ������ҿ��Կ�����д�ģ��Ҿ�͵һ������
struct my_kobj_child {
	struct kobject kobj_cacogenic;//һ�����εĶ���,��bin�ļ�
	struct kobject kobj_virtual;//һ������Ķ���,��һ��link
	struct kobject kobj_true;//һ����ʵ�Ķ���
	struct child_info true_child_info;
	char cacogenic_info;//���εȼ�
};//�������ӣ�����������

struct my_kobj {
	struct kobject kobj;
	struct my_kobj_child kobj_child;
};

/***************************************/
//parent
void my_kobj_release(struct kobject *kobj) {
	struct my_kobj *my_kobj_p = to_my_kobj(kobj);

	MY_DEBUG("my_kobj_release\n");

	kfree(my_kobj_p);
}

static struct kobj_type my_ktype= {
	.release = my_kobj_release,
	.sysfs_ops = NULL,
	.default_attrs = NULL,
};

/***************************************/

/***************child*******************/

//true child define
// show�����ԵĶ���������
static ssize_t my_true_child_show(struct kobject *kobj,struct attribute *attr,char *buf) {
	struct my_kobj_child *my_kobj_child_p = to_my_kobj_child(kobj);

	if (0 == strcmp(attr->name, "name")) {
		return snprintf(buf, PAGE_SIZE, "%s\n", my_kobj_child_p->true_child_info.name);
	} else if (0 == strcmp(attr->name, "age")) {
		return snprintf(buf, PAGE_SIZE, "%d\n", my_kobj_child_p->true_child_info.age);
	} else if (0 == strcmp(attr->name, "height")) {
		return snprintf(buf, PAGE_SIZE, "%d\n", my_kobj_child_p->true_child_info.height);
	} else if (0 == strcmp(attr->name, "weight")) {
		return snprintf(buf, PAGE_SIZE, "%d\n", my_kobj_child_p->true_child_info.weight);
	}

	return 0;
}
// store�����Ե�д��������
static ssize_t my_true_child_store(struct kobject *kobj,struct attribute *attr,const char *buf,size_t count) {
	struct my_kobj_child *my_kobj_child_p = to_my_kobj_child(kobj);

	//û��ʲô������жϣ���Ҳ�Ҫ���ң�����һЩ��̬�ַ���
	if (0 == strcmp(attr->name, "name")) {
		sscanf(buf, "%s", my_kobj_child_p->true_child_info.name);//���벻Ҫ�ӿո�
	} else if (0 == strcmp(attr->name, "age")) {
		sscanf(buf, "%d", &my_kobj_child_p->true_child_info.age);
	} else if (0 == strcmp(attr->name, "height")) {
		sscanf(buf, "%d", &my_kobj_child_p->true_child_info.height);
	} else if (0 == strcmp(attr->name, "weight")) {
		sscanf(buf, "%d", &my_kobj_child_p->true_child_info.weight);
	}

	return count;
}

static struct attribute my_true_child_name = {
	.name = "name",
	.mode = S_IRUGO | S_IWUSR,//root �û�����д
};

static struct attribute my_true_child_age = {
	.name = "age",
	.mode = S_IRUGO | S_IWUSR,//root �û�����д
};

static struct attribute my_true_child_height = {
	.name = "height",
	.mode = S_IRUGO | S_IWUSR,//root �û�����д
};

static struct attribute my_true_child_weight = {
	.name = "weight",
	.mode = S_IRUGO | S_IWUSR,//root �û�����д
};

//ÿ��kobject�����Ի��Ϊsysfs�е�һ���ļ�
static struct attribute *my_true_child_def[] = {
	&my_true_child_name,
	&my_true_child_age,
	&my_true_child_height,
	&my_true_child_weight,
	NULL,
};

void my_true_child_release(struct kobject *kobj) {
	MY_DEBUG("my_true_child_release\n");
}

static struct sysfs_ops my_true_child_op = {
	.show = my_true_child_show,
	.store = my_true_child_store,
};

static struct kobj_type my_true_child_ktype = {
	.release = my_true_child_release,
	.sysfs_ops = &my_true_child_op,
	.default_attrs = my_true_child_def,
};
//end true child define

//virtaul child define
static void my_virtaul_child_release(struct kobject *kobj) {
	MY_DEBUG("my_virtaul_child_release\n");
}

static struct kobj_type my_virtaul_child_ktype = {
	.release = my_virtaul_child_release,
	.sysfs_ops = NULL,
	.default_attrs = NULL,
};
//end virtaul child define

//cacogenic child define
static void my_cacogenic_child_release(struct kobject *kobj) {
	MY_DEBUG("my_cacogenic_child_release\n");
}

static ssize_t my_bin_read(struct file *filp, struct kobject *kobj,
                           struct bin_attribute *attr,
                           char *buf, loff_t off, size_t count) {
	//�ҿ���at24.c��read�����eeprom�ж�256�ֽ���Ϣ��

	ssize_t size = 0;
	struct my_kobj_child *my_kobj_child_p = to_my_kobj_child2(kobj);

	while(count) {
		*buf = my_kobj_child_p->cacogenic_info;
		buf += 1;
		off += 1;
		count -= 1;
		size += 1;
	}

	return size;
}

static struct bin_attribute my_bin_attr = {
	.attr = {
		.name = "my_bin",
		.mode = S_IRUGO | S_IWUSR,//root �û�����д
	},
	.size = MY_BIN_SIZE,
	.read = my_bin_read,
	//write��mmap�Ͳ�ʵ����
};

static struct kobj_type my_cacogenic_child_ktype = {
	.release = my_cacogenic_child_release,
	.sysfs_ops = NULL,
	.default_attrs = NULL,
};
//end cacogenic child define

/***************************************/
static struct my_kobj *my_kobj_p;

static int __init my_kobj_init(void) {
	int ret = 0;
	my_kobj_p = (struct my_kobj *)kzalloc(sizeof(struct my_kobj), GFP_KERNEL);
	if (my_kobj_p == NULL) {
		ret = -ENOMEM;
		goto fail;
	}

	snprintf(my_kobj_p->kobj_child.true_child_info.name, PAGE_SIZE, "wwxxxxll");
	my_kobj_p->kobj_child.true_child_info.age = 25;
	my_kobj_p->kobj_child.true_child_info.height = 171;
	my_kobj_p->kobj_child.true_child_info.weight = 77;

	my_kobj_p->kobj_child.cacogenic_info = 1;//��ʼ�����εȼ�Ϊ1

    // ��������ӵ�sysfs
	ret = kobject_init_and_add(&my_kobj_p->kobj, &my_ktype, NULL, "my_kobj"); // parent Ϊnull, ��ʾ��sysĿ¼��
	if (ret) {
		goto fail1;
	}
	//kobject_init_and_add�����kobject_init��kobject_add_varg()
	//kset�Ĺ�ϵ�����ǵ���kobj_kset_join(),�ҾͲ�������
	ret = kobject_init_and_add(&my_kobj_p->kobj_child.kobj_cacogenic, &my_cacogenic_child_ktype, &my_kobj_p->kobj, "cacogenic");
	if (ret) {
		goto fail2;
	}

	ret = kobject_init_and_add(&my_kobj_p->kobj_child.kobj_true, &my_true_child_ktype, &my_kobj_p->kobj, "true");
	if (ret) {
		goto fail3;
	}

	ret = kobject_init_and_add(&my_kobj_p->kobj_child.kobj_virtual, &my_virtaul_child_ktype, &my_kobj_p->kobj, "virtual");
	if (ret) {
		goto fail4;
	}
	//����Ϊ����ģ�ֻ�����ӵ�true����ʵ��struct ktype��
	ret = sysfs_create_link(&my_kobj_p->kobj_child.kobj_virtual, &my_kobj_p->kobj_child.kobj_true, "virtual_child");
	if (ret) {
		sysfs_remove_link(&my_kobj_p->kobj_child.kobj_virtual, "virtual_child");//�ҿ��ں��еĳ������Ҳremoveһ�£���Ҳ��ôд�ˡ�

		goto fail5;
	}

	ret = sysfs_create_bin_file(&my_kobj_p->kobj_child.kobj_cacogenic, &my_bin_attr);
	if (ret) {
		goto fail6;
	}
	return 0;

fail6:
	sysfs_remove_link(&my_kobj_p->kobj_child.kobj_virtual, "virtual_child");
fail5:
	kobject_del(&my_kobj_p->kobj_child.kobj_virtual);
	kobject_put(&my_kobj_p->kobj_child.kobj_virtual);
fail4:
	kobject_del(&my_kobj_p->kobj_child.kobj_true);
	kobject_put(&my_kobj_p->kobj_child.kobj_true);
fail3:
	kobject_del(&my_kobj_p->kobj_child.kobj_cacogenic);
	kobject_put(&my_kobj_p->kobj_child.kobj_cacogenic);
fail2:
	kobject_del(&my_kobj_p->kobj);
	kobject_put(&my_kobj_p->kobj);//�����my_kobj_release
fail1:
	kfree(my_kobj_p);
fail:
	return ret;
}
static void __exit my_kobj_exit(void) {
	sysfs_remove_bin_file(&my_kobj_p->kobj_child.kobj_cacogenic, &my_bin_attr);//�����int���أ��е��ر�

	sysfs_remove_link(&my_kobj_p->kobj_child.kobj_virtual, "virtual_child");

	kobject_del(&my_kobj_p->kobj_child.kobj_virtual);
	kobject_put(&my_kobj_p->kobj_child.kobj_virtual);

	kobject_del(&my_kobj_p->kobj_child.kobj_true);
	kobject_put(&my_kobj_p->kobj_child.kobj_true);

	kobject_del(&my_kobj_p->kobj_child.kobj_cacogenic);
	kobject_put(&my_kobj_p->kobj_child.kobj_cacogenic);

	kobject_del(&my_kobj_p->kobj);
	kobject_put(&my_kobj_p->kobj);//�����my_kobj_release
}

module_init(my_kobj_init);
module_exit(my_kobj_exit);


/*
http://blog.csdn.net/mbmlccking168/article/details/51970001


[root@zhong8 my_kobj]# tree /sys/my_kobj/
/sys/my_kobj/
������ cacogenic
��?? ������ my_bin
������ true
��?? ������ age
��?? ������ height
��?? ������ name
��?? ������ weight
������ virtual
    ������ virtual_child -> ../true

4 directories, 5 files
*/
