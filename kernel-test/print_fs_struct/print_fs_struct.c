#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/fdtable.h>
#include <linux/fs_struct.h>

#ifndef offsetof 
#define offsetof(type, field)   ((long) &((type *)0)->field) 
#endif   /* offsetof */ 
 
#ifndef container_of 
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})
#endif 


static int pid = 1;

module_param(pid,int,0644);



static void print_fs_struct(void)
{
    struct task_struct *p;
	struct pid *k;
    struct fs_struct *fs;

	k = find_vpid(pid);
	p = pid_task(k,PIDTYPE_PID);
    fs = p->fs;
    printk("process:%s,pid:%d\n",p->comm,p->pid);
    
    printk("users : %d \n",fs->users);
    printk("umask : %d \n",fs->umask);
    printk("in_exec : %d \n",fs->in_exec);
    printk("root : %s \n",fs->root.dentry->d_name.name);
    printk("pwd : %s \n",fs->pwd.dentry->d_name.name);
    
	return ;      
}

static int __init print_fs_struct_init(void)
{
	print_fs_struct();	
	return 0;
}

static void __exit print_fs_struct_exit(void)
{
   printk("<1>exit ---------------------!\n");
}

module_init(print_fs_struct_init);
module_exit(print_fs_struct_exit);
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A simple Module for testing ");
MODULE_VERSION("V1.0");


/*
insmod print_fs_struct.ko pid=$$
dmesg

[34674.643940] process:costor,pid:14905
[34674.643943] users : 17 
[34674.643945] umask : 18 
[34674.643946] in_exec : 0 
[34674.643948] root : / 
[34674.643949] pwd : /
*/