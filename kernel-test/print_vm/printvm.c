#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/mm_types.h>

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

static void printvm_list(void)
{
	struct task_struct *p;
	struct pid *k;
	struct vm_area_struct *tmp;
	
	k = find_vpid(pid);
	p = pid_task(k,PIDTYPE_PID);
	tmp = p->mm->mmap;
	
	printk("process:%s,pid:%d\n",p->comm,p->pid);

	while (tmp != NULL) {
		printk("0x%lx - 0x%lx\t",tmp->vm_start,tmp->vm_end);
		if (tmp->vm_flags & VM_READ)
			printk("r");
		else
			printk("-");

		if (tmp->vm_flags & VM_WRITE)
			printk("w");
		else 
			printk("-");
	
		if (tmp->vm_flags & VM_EXEC)
			printk("x");
		else
			printk("-");
		
		if (tmp->vm_flags & VM_SHARED)
			printk("s\n");
		else
			printk("p\n");

		tmp = tmp->vm_next;	
	}
}



static void visit(struct rb_node *root)
{
	struct vm_area_struct *tmp;
	tmp =  container_of(root,struct vm_area_struct,vm_rb);
	printk("0x%lx - 0x%lx\t",tmp->vm_start,tmp->vm_end);
	if (tmp->vm_flags & VM_READ)
		printk("r");
	else
		printk("-");

	if (tmp->vm_flags & VM_WRITE)
		printk("w");
	else 
		printk("-");
	
	if (tmp->vm_flags & VM_EXEC)
		printk("x");
	else
		printk("-");
		
	if (tmp->vm_flags & VM_SHARED)
		printk("s\n");
	else
		printk("p\n");
}

static void print_rb_tree(struct rb_node *root)
{
	if (root != NULL) {
		visit(root);
		print_rb_tree(root->rb_left);
		print_rb_tree(root->rb_right);
	}
}

static void printvm_tree(void)
{
    struct task_struct *p;
	struct pid *k;
	struct rb_node *root = NULL;

	k = find_vpid(pid);
	p = pid_task(k,PIDTYPE_PID);
	root = p->mm->mm_rb.rb_node;
	print_rb_tree(root);
	return ;      
}

static int __init printvm_init(void)
{
	printvm_list();
	printk("------------------------\n");
	printvm_tree();		
	return 0;
}

static void __exit printvm_exit(void)
{
   printk("<1>exit ---------------------!\n");
}

module_init(printvm_init);
module_exit(printvm_exit);
MODULE_LICENSE("GPL");


/*
insmod printvm.ko pid=$$
dmesg

process:bash,pid:7249
0x400000 - 0x4d4000	r-xp
0x6d3000 - 0x6dd000	rw-p
0x6dd000 - 0x6e2000	rw-p
0x125d000 - 0x129f000	rw-p
0x7f11a1279000 - 0x7f11a710e000	r--p
0x7f11a710e000 - 0x7f11a711b000	r-xp
0x7f11a711b000 - 0x7f11a731a000	---p
0x7f11a731a000 - 0x7f11a731b000	r--p
0x7f11a731b000 - 0x7f11a731c000	rw-p
0x7f11a731c000 - 0x7f11a74a6000	r-xp
0x7f11a74a6000 - 0x7f11a76a6000	---p
0x7f11a76a6000 - 0x7f11a76aa000	r--p
0x7f11a76aa000 - 0x7f11a76ac000	rw-p
0x7f11a76ac000 - 0x7f11a76b0000	rw-p
0x7f11a76b0000 - 0x7f11a76b2000	r-xp
0x7f11a76b2000 - 0x7f11a78b2000	---p
0x7f11a78b2000 - 0x7f11a78b3000	r--p
0x7f11a78b3000 - 0x7f11a78b4000	rw-p
0x7f11a78b4000 - 0x7f11a78d1000	r-xp
0x7f11a78d1000 - 0x7f11a7ad0000	---p
0x7f11a7ad0000 - 0x7f11a7ad4000	rw-p
0x7f11a7ad4000 - 0x7f11a7ad5000	rw-p
0x7f11a7ad5000 - 0x7f11a7af5000	r-xp
0x7f11a7ce5000 - 0x7f11a7cec000	r--p
0x7f11a7cec000 - 0x7f11a7cef000	rw-p
0x7f11a7cf2000 - 0x7f11a7cf5000	rw-p
0x7f11a7cf5000 - 0x7f11a7cf6000	r--p
0x7f11a7cf6000 - 0x7f11a7cf7000	rw-p
0x7f11a7cf7000 - 0x7f11a7cf8000	rw-p
0x7fffe595f000 - 0x7fffe5975000	rw-p
0x7fffe59b0000 - 0x7fffe59b1000	r-xp
------------------------
0x7f11a76b0000 - 0x7f11a76b2000	r-xp
0x7f11a731c000 - 0x7f11a74a6000	r-xp
0x125d000 - 0x129f000	rw-p
0x6d3000 - 0x6dd000	rw-p
0x400000 - 0x4d4000	r-xp
0x6dd000 - 0x6e2000	rw-p
0x7f11a711b000 - 0x7f11a731a000	---p
0x7f11a710e000 - 0x7f11a711b000	r-xp
0x7f11a1279000 - 0x7f11a710e000	r--p
0x7f11a731a000 - 0x7f11a731b000	r--p
0x7f11a731b000 - 0x7f11a731c000	rw-p
0x7f11a76a6000 - 0x7f11a76aa000	r--p
0x7f11a74a6000 - 0x7f11a76a6000	---p
0x7f11a76ac000 - 0x7f11a76b0000	rw-p
0x7f11a76aa000 - 0x7f11a76ac000	rw-p
0x7f11a7ad5000 - 0x7f11a7af5000	r-xp
0x7f11a78b4000 - 0x7f11a78d1000	r-xp
0x7f11a78b2000 - 0x7f11a78b3000	r--p
0x7f11a76b2000 - 0x7f11a78b2000	---p
0x7f11a78b3000 - 0x7f11a78b4000	rw-p
0x7f11a7ad0000 - 0x7f11a7ad4000	rw-p
0x7f11a78d1000 - 0x7f11a7ad0000	---p
0x7f11a7ad4000 - 0x7f11a7ad5000	rw-p
0x7f11a7cf7000 - 0x7f11a7cf8000	rw-p
0x7f11a7cf2000 - 0x7f11a7cf5000	rw-p
0x7f11a7cec000 - 0x7f11a7cef000	rw-p
0x7f11a7ce5000 - 0x7f11a7cec000	r--p
0x7f11a7cf5000 - 0x7f11a7cf6000	r--p
0x7f11a7cf6000 - 0x7f11a7cf7000	rw-p
0x7fffe595f000 - 0x7fffe5975000	rw-p
0x7fffe59b0000 - 0x7fffe59b1000	r-xp


[root@bogon print_vm]# cat /proc/7249/maps 
00400000-004d4000 r-xp 00000000 fd:00 1567528                            /bin/bash
006d3000-006dd000 rw-p 000d3000 fd:00 1567528                            /bin/bash
006dd000-006e2000 rw-p 00000000 00:00 0 
0125d000-0129f000 rw-p 00000000 00:00 0                                  [heap]
7f11a1279000-7f11a710e000 r--p 00000000 fd:00 1437786                    /usr/lib/locale/locale-archive
7f11a710e000-7f11a711b000 r-xp 00000000 fd:00 652829                     /lib64/libnss_files-2.12.so
7f11a711b000-7f11a731a000 ---p 0000d000 fd:00 652829                     /lib64/libnss_files-2.12.so
7f11a731a000-7f11a731b000 r--p 0000c000 fd:00 652829                     /lib64/libnss_files-2.12.so
7f11a731b000-7f11a731c000 rw-p 0000d000 fd:00 652829                     /lib64/libnss_files-2.12.so
7f11a731c000-7f11a74a6000 r-xp 00000000 fd:00 652813                     /lib64/libc-2.12.so
7f11a74a6000-7f11a76a6000 ---p 0018a000 fd:00 652813                     /lib64/libc-2.12.so
7f11a76a6000-7f11a76aa000 r--p 0018a000 fd:00 652813                     /lib64/libc-2.12.so
7f11a76aa000-7f11a76ac000 rw-p 0018e000 fd:00 652813                     /lib64/libc-2.12.so
7f11a76ac000-7f11a76b0000 rw-p 00000000 00:00 0 
7f11a76b0000-7f11a76b2000 r-xp 00000000 fd:00 652819                     /lib64/libdl-2.12.so
7f11a76b2000-7f11a78b2000 ---p 00002000 fd:00 652819                     /lib64/libdl-2.12.so
7f11a78b2000-7f11a78b3000 r--p 00002000 fd:00 652819                     /lib64/libdl-2.12.so
7f11a78b3000-7f11a78b4000 rw-p 00003000 fd:00 652819                     /lib64/libdl-2.12.so
7f11a78b4000-7f11a78d1000 r-xp 00000000 fd:00 652855                     /lib64/libtinfo.so.5.7
7f11a78d1000-7f11a7ad0000 ---p 0001d000 fd:00 652855                     /lib64/libtinfo.so.5.7
7f11a7ad0000-7f11a7ad4000 rw-p 0001c000 fd:00 652855                     /lib64/libtinfo.so.5.7
7f11a7ad4000-7f11a7ad5000 rw-p 00000000 00:00 0 
7f11a7ad5000-7f11a7af5000 r-xp 00000000 fd:00 652803                     /lib64/ld-2.12.so
7f11a7ce5000-7f11a7cec000 r--s 00000000 fd:00 1437785                    /usr/lib64/gconv/gconv-modules.cache
7f11a7cec000-7f11a7cef000 rw-p 00000000 00:00 0 
7f11a7cf2000-7f11a7cf5000 rw-p 00000000 00:00 0 
7f11a7cf5000-7f11a7cf6000 r--p 00020000 fd:00 652803                     /lib64/ld-2.12.so
7f11a7cf6000-7f11a7cf7000 rw-p 00021000 fd:00 652803                     /lib64/ld-2.12.so
7f11a7cf7000-7f11a7cf8000 rw-p 00000000 00:00 0 
7fffe5960000-7fffe5975000 rw-p 00000000 00:00 0                          [stack]
7fffe59b0000-7fffe59b1000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]

*/