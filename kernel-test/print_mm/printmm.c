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



static void print_mm(void)
{
    struct task_struct *p;
	struct pid *k;
    struct mm_struct *mm;

	k = find_vpid(pid);
	p = pid_task(k,PIDTYPE_PID);
    mm = p->mm;
    printk("process:%s,pid:%d\n",p->comm,p->pid);
    
    printk("mmap_base : 0x%lx \n",mm->mmap_base);
    printk("task_size : 0x%lx \n",mm->task_size);
    printk("mm_users : %d \n",mm->mm_users.counter);
    printk("mm_count : %d \n",mm->mm_count.counter);
    printk("map_count : %d \n",mm->map_count);
    //printk("_file_rss : %d \n",mm->_file_rss);
    //printk("_anon_rss : %d \n",mm->_anon_rss);
    printk("hiwater_rss : 0x%lx \n",mm->hiwater_rss);
    printk("hiwater_vm : 0x%lx \n",mm->hiwater_vm);
    printk("total_vm : 0x%lx \n",mm->total_vm);
    printk("locked_vm : 0x%lx \n",mm->locked_vm);
    printk("shared_vm : 0x%lx \n",mm->shared_vm);
    printk("exec_vm : 0x%lx \n",mm->exec_vm);
    printk("stack_vm : 0x%lx \n",mm->stack_vm);
    printk("def_flags : 0x%lx \n",mm->def_flags);
    //printk("nr_ptes : 0x%lx \n",mm->nr_ptes);
    printk("start_code : 0x%lx \n",mm->start_code);
    printk("end_code : 0x%lx \n",mm->end_code);
    printk("start_data : 0x%lx \n",mm->start_data);
    printk("end_data : 0x%lx \n",mm->end_data);
    printk("start_brk : 0x%lx \n",mm->start_brk);
    printk("brk : 0x%lx \n",mm->brk);
    printk("start_stack : 0x%lx \n",mm->start_stack);
    printk("arg_start : 0x%lx \n",mm->arg_start);
    printk("arg_end : 0x%lx \n",mm->arg_end);
    printk("env_start : 0x%lx \n",mm->env_start);
    printk("env_end : 0x%lx \n",mm->env_end);
	return ;      
}

static int __init printvm_init(void)
{
	print_mm();	
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
insmod printmm.ko pid=$$
dmesg

[1037234.975045] mmap_base : 0x7fdf665ae000 
[1037234.975053] task_size : 0x7ffffffff000 
[1037234.975056] mm_users : 12 
[1037234.975059] mm_count : 14 
[1037234.975062] map_count : 81 
[1037234.975064] hiwater_rss : 0x1606 
[1037234.975067] hiwater_vm : 0x1ad94 
[1037234.975070] total_vm : 0x1bd96 
[1037234.975072] locked_vm : 0x0 
[1037234.975075] shared_vm : 0x1af5 
[1037234.975077] exec_vm : 0x1069 
[1037234.975080] stack_vm : 0x21 
[1037234.975082] def_flags : 0x0 
[1037234.975085] start_code : 0x400000 
[1037234.975087] end_code : 0x10e610c 
[1037234.975090] start_data : 0x12e6d98 
[1037234.975093] end_data : 0x134e240 
[1037234.975095] start_brk : 0x2685000 
[1037234.975098] brk : 0x26a6000 
[1037234.975100] start_stack : 0x7ffe39b87660 
[1037234.975103] arg_start : 0x7ffe39b88f3a 
[1037234.975105] arg_end : 0x7ffe39b88f7c 
[1037234.975107] env_start : 0x7ffe39b88f7c 
[1037234.975109] env_end : 0x7ffe39b88fe2
*/