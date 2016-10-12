#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>    /* printk() */
#include <linux/slab.h>        /* kmalloc() */
#include <linux/fs.h>        /* everything... */
#include <linux/errno.h>    /* error codes */
#include <linux/types.h>    /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>    /* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>

#include <asm/system.h>        /* cli(), *_flags */
#include <asm/uaccess.h>    /* copy_*_user */

//#include "scull.h"        /* local definitions */
#define PDEBUG(fmt, args...) printk( KERN_ERR "scull: " fmt, ## args)

MODULE_AUTHOR("Alessandro Rubini, Jonathan Corbet");
MODULE_LICENSE("Dual BSD/GPL");

char buf[400] = "12345678901234567890";
/*
 * Here are our sequence iteration methods.  Our "position" is
 * simply the device number.
 */
/*
 *  s:     almost always ignored
 *  pos:   integer position indicateing where to start
 *         need not be a byte position
 */
static void *seq_seq_start(struct seq_file *s, loff_t *pos)
{
    PDEBUG("position is %d", *pos);
    if (*pos >= 400)
        return NULL;
    return  buf + *pos;
}

/*
 *  v:       is the iterator as returned from previous call to start or next
 *  return:  NULL means nothing left 
 */
static void *seq_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
    PDEBUG("next: %d", *pos);
    if (*pos >= 400)
        return NULL;
    (*pos) = (*pos) + 50;      // 对换赋值和比较，可以修复这个bug 
    if (*pos > 400)
        *pos = 400;
    return  buf + *pos;
}

static void seq_seq_stop(struct seq_file *s, void *v)
{
    /* Actually, there's nothing to do here */
}

static int seq_seq_show(struct seq_file *s, void *v)
{
    PDEBUG("show start: %p", v);
    char *p = (char*)v;
    char data[51] = {0};
    int i;
    
    for (i=0; (i<50) && (p+i<buf+400); i++)
    {
        data[i] = *(p + i);
    }

    seq_printf(s, "show: %p:%s\n", v, data);
    return 0;
}
/*
 * Tie the sequence operators up.
 */
static struct seq_operations seq_seq_ops = {
    .start = seq_seq_start,
    .next  = seq_seq_next,
    .stop  = seq_seq_stop,
    .show  = seq_seq_show
};

/*
 * Now to implement the /proc file we need only make an open
 * method which sets up the sequence operators.
 */
static int seq_proc_open(struct inode *inode, struct file *file)
{
    return seq_open(file, &seq_seq_ops);
}

/*
 * Create a set of file operations for our proc file.
 */
static struct file_operations seq_proc_ops = {
    .owner   = THIS_MODULE,
    .open    = seq_proc_open,
    .read    = seq_read,
    .llseek  = seq_lseek,
    .release = seq_release
};

int seq_init_module(void)
{
    struct proc_dir_entry *entry;
    PDEBUG("seq test/n");
    entry = create_proc_entry("seq_test", 0, NULL);
    if (entry)
        entry->proc_fops = &seq_proc_ops;
    /* create file /proc/proc_test */
    return 0;
}

void seq_cleanup_module(void)
{
    /* remove /proc/seq_test */
    remove_proc_entry("seq_test", NULL);
    return; 
}
module_init(seq_init_module);
module_exit(seq_cleanup_module);