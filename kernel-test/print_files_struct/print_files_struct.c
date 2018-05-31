#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/fdtable.h>

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



static void print_files_struct(void) {
	struct task_struct *p;
	struct pid *k;
	struct files_struct *files;

	k = find_vpid(pid);
	p = pid_task(k,PIDTYPE_PID);
	files = p->files;
	printk("process:%s,pid:%d\n",p->comm,p->pid);

	printk("count : %d \n",files->count.counter);
	printk("next_fd : %d \n",files->next_fd);
	printk("close_on_exec_init : 0x%lx \n",files->close_on_exec_init[0]);
	printk("open_fds_init : 0x%lx \n",files->open_fds_init[0]);
	printk("full_fds_bits_init : 0x%lx \n",files->full_fds_bits_init[0]);
	printk("fd_array : %p \n",files->fd_array);
	printk("fd_array[0] : %p \n",files->fd_array[0]);
	printk("fd_array[1] : %p \n",files->fd_array[1]);
	printk("fd_array[2] : %p \n",files->fd_array[2]);
	printk("fd_array[3] : %p \n",files->fd_array[3]);
	printk("fd_array[4] : %p \n",files->fd_array[4]);
	printk("fdt : %p \n",files->fdt);
	printk("fdtab address: %p \n",&files->fdtab);
	printk("fdtab.max_fds : %d \n",files->fdtab.max_fds);
	printk("fdtab.fd : %p \n",files->fdtab.fd);
	printk("fdtab.close_on_exec : %p \n",files->fdtab.close_on_exec);
	printk("fdtab.open_fds : %p \n",files->fdtab.open_fds);
	printk("fdtab.full_fds_bits : %p \n",files->fdtab.full_fds_bits);
	printk("fdtab.fd.file0 : %p \n",(files->fdtab.fd)[0]);

	printk("fdt.max_fds : %d \n",files->fdt->max_fds);
	printk("fdt.fd : %p \n",files->fdt->fd);
	printk("fdt.close_on_exec : %p \n",files->fdt->close_on_exec);
	printk("fdt.open_fds : %p \n",files->fdt->open_fds);
	printk("fdt.full_fds_bits : %p \n",files->fdt->full_fds_bits);
	printk("fdt.fd.file0 : %p \n",(files->fdt->fd)[0]);
	printk("fdt.fd.file1 : %p \n",(files->fdt->fd)[1]);
	printk("fdt.fd.file2 : %p \n",(files->fdt->fd)[2]);
	printk("fdt.fd.file3 : %p \n",(files->fdt->fd)[3]);
	printk("fdt.fd.file4 : %p \n",(files->fdt->fd)[4]);


	return ;
}


static int __init print_files_struct_init(void) {
	print_files_struct();
	return 0;
}

static void __exit print_files_struct_exit(void) {
	printk("<1>exit ---------------------!\n");
}

module_init(print_files_struct_init);
module_exit(print_files_struct_exit);
MODULE_LICENSE("GPL");


/*
insmod print_files_struct.ko pid=$$
dmesg

files_struct.count  //一般等于线程数

insmod print_files_struct.ko pid=$$

[ 7250.084127] process:test,pid:6243
[ 7250.084134] count : 1
[ 7250.084137] next_fd : 3
[ 7250.084139] close_on_exec_init : 0x0
[ 7250.084142] open_fds_init : 0x1
[ 7250.084144] full_fds_bits_init : 0x0
[ 7250.084147] fd_array : ffff880035a3c8e0
[ 7250.084150] fd_array[0] :           (null)
[ 7250.084152] fd_array[1] :           (null)
[ 7250.084155] fd_array[2] :           (null)
[ 7250.084157] fd_array[3] :           (null)
[ 7250.084160] fd_array[4] :           (null)
[ 7250.084162] fdt : ffff880035427780
[ 7250.084165] fdtab address: ffff880035a3c868
[ 7250.084168] fdtab.max_fds : 64
[ 7250.084170] fdtab.fd : ffff880035a3c8e0
[ 7250.084173] fdtab.close_on_exec : ffff880035a3c8c8
[ 7250.084196] fdtab.open_fds : ffff880035a3c8d0
[ 7250.084199] fdtab.full_fds_bits : ffff880035a3c8d8
[ 7250.084202] fdtab.fd.file0 :           (null)
[ 7250.084205] fdt.max_fds : 256
[ 7250.084207] fdt.fd : ffff8800bb3de000
[ 7250.084210] fdt.close_on_exec : ffff8800b6fad080
[ 7250.084212] fdt.open_fds : ffff8800b6fad060
[ 7250.084215] fdt.full_fds_bits : ffff8800b6fad0a0
[ 7250.084230] fdt.fd.file0 : ffff8800b8e9b100
[ 7250.084232] fdt.fd.file1 : ffff8800b8e9b100
[ 7250.084235] fdt.fd.file2 : ffff8800b8e9b100
[ 7250.084237] fdt.fd.file3 :           (null)
[ 7250.084239] fdt.fd.file4 :           (null)


打开文件数很少也是使用fdt, fd_array被废弃了吗？？？


ls -alh /proc/6243/fd/
total 0
dr-x------ 2 root root  0 Jan 15 12:41 .
dr-xr-xr-x 9 root root  0 Jan 15 12:41 ..
lrwx------ 1 root root 64 Jan 15 12:41 0 -> /dev/pts/2
lrwx------ 1 root root 64 Jan 15 12:41 1 -> /dev/pts/2
lrwx------ 1 root root 64 Jan 15 12:41 2 -> /dev/pts/2


test8.go

package main

import (
	"fmt"
	"os"
	"time"
)

func main() {

	fileList := make([]*os.File, 0)
	for i := 0; i < 10000; i++ {
		path := fmt.Sprintf("/home/test/%d", i)
		f, err := os.Create(path)
		if err != nil {
			fmt.Println(err.Error())
			return
		}
		fileList = append(fileList, f)
	}
	for {
		time.Sleep(time.Second)
	}

}

ulimit -n 100000

[ 6708.227791] process:test8,pid:6067
[ 6708.227794] count : 5
[ 6708.227796] next_fd : 10004
[ 6708.227797] close_on_exec_init : 0x0
[ 6708.227798] open_fds_init : 0x7
[ 6708.227799] full_fds_bits_init : 0x0
[ 6708.227800] fd_array : ffff880035a62f60
[ 6708.227801] fd_array[0] :           (null)
[ 6708.227802] fd_array[1] :           (null)
[ 6708.227803] fd_array[2] :           (null)
[ 6708.227804] fd_array[3] :           (null)
[ 6708.227805] fd_array[4] :           (null)
[ 6708.227806] fdt : ffff8800ba918f80
[ 6708.227808] fdtab address: ffff880035a62ee8
[ 6708.227809] fdtab.max_fds : 64
[ 6708.227811] fdtab.fd : ffff880035a62f60
[ 6708.227812] fdtab.close_on_exec : ffff880035a62f48
[ 6708.227814] fdtab.open_fds : ffff880035a62f50
[ 6708.227816] fdtab.full_fds_bits : ffff880035a62f58
[ 6708.227818] fdtab.fd.file0 :           (null)
[ 6708.227819] fdt.max_fds : 16384
[ 6708.227821] fdt.fd : ffffc90000a66000
[ 6708.227823] fdt.close_on_exec : ffff8800b8d96800
[ 6708.227824] fdt.open_fds : ffff8800b8d96000
[ 6708.227826] fdt.full_fds_bits : ffff8800b8d97000
[ 6708.227828] fdt.fd.file0 : ffff8801390afe00
[ 6708.227829] fdt.fd.file1 : ffff8801390afe00
[ 6708.227831] fdt.fd.file2 : ffff8801390afe00
[ 6708.227832] fdt.fd.file3 : ffff880138741f00
[ 6708.227834] fdt.fd.file4 : ffff880138741000





*/