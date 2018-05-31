#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/fdtable.h>
#include <linux/fs.h>

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

static int fd = 0;

module_param(fd,int,0644);


static void print_file(void)
{
    struct task_struct *p;
	struct pid *k;
    struct files_struct *files;
    struct file *file;

	k = find_vpid(pid);
	p = pid_task(k,PIDTYPE_PID);
    files = p->files;
    printk("process:%s,pid:%d\n",p->comm,p->pid);
    
    file = (files->fdt->fd)[fd];

    printk("fdt.max_fds : %d \n",files->fdt->max_fds);
    // printk("count : %d \n",file->count.counter);
    printk("filename : %s \n",file->f_path.dentry->d_name.name);
    printk("f_op->read : %p \n",file->f_op->read);
    printk("f_op->read_iter : %p \n",file->f_op->read_iter);
    
    
	return ;      
}


static int __init print_file_init(void)
{
    printk("<1>enter ---------------------!\n");
	print_file();	
	return 0;
}

static void __exit print_file_exit(void)
{
   printk("<1>exit ---------------------!\n");
}

module_init(print_file_init);
module_exit(print_file_exit);
MODULE_LICENSE("GPL");


/*
insmod print_file.ko pid=$$
dmesg

[124324.120262] <1>enter ---------------------!
[124324.120268] process:costor,pid:4131
[124324.120271] fdt.max_fds : 128 
[124324.120273] filename : engine.db 
[124324.120275] f_op->read :           (null) 
[124324.120276] f_op->read_iter : ffffffff81191ba0

cat /proc/kallsyms  |grep ffffffff81191ba0
ffffffff81191ba0 T generic_file_read_iter

*/