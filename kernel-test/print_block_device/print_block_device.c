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
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <uapi/linux/hdreg.h>
#include <linux/cpumask.h>




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

static int print_hd_struct(struct hd_struct *hd);
static int print_gendisk(struct gendisk * gd);
static int print_geometry(struct hd_geometry * hg);
static int print_disk_stats(struct disk_stats __percpu *dkstats);
static int print_device(struct device *ddev) ;



static int print_block_device(void) {
	struct task_struct *p;
	struct pid *k;
	struct files_struct *files;
	struct file *file;
	struct block_device *bdev;
	struct gendisk *	bd_disk;
	struct hd_struct *hd;
	struct hd_geometry geometry;

	k = find_vpid(pid);
	p = pid_task(k,PIDTYPE_PID);
	files = p->files;
	printk("process:%s,pid:%d\n",p->comm,p->pid);

	file = (files->fdt->fd)[fd];
	//bdev = bd_acquire(file->f_inode);//获取block device实例
	bdev = file->f_inode->i_bdev;
	if (bdev == NULL)
		return -ENOMEM;

	printk("MAJOR:%d MINOR:%d \n",MAJOR(bdev->bd_dev),MINOR(bdev->bd_dev));
	printk("bd_openers:%d \n",bdev->bd_openers);
	printk("bd_holders:%d \n",bdev->bd_holders);
	printk("bd_write_holder:%d \n",bdev->bd_write_holder);
	printk("bd_block_size:%d \n",bdev->bd_block_size);
	printk("bd_part_count:%d \n",bdev->bd_part_count);
	printk("bd_invalidated:%d \n",bdev->bd_invalidated);
	printk("bd_fsfreeze_count:%d \n",bdev->bd_fsfreeze_count);
	printk("bd_queue:%p \n",bdev->bd_queue);


	hd = bdev->bd_part;
	if (hd != NULL) {
		printk("\n");
		printk("bd_part\n");
		print_hd_struct(hd);
		printk("\n");
	}
	bd_disk = bdev->bd_disk;
	bd_disk->fops->getgeo(bdev,&geometry);
	printk("\n");
	printk("geometry\n");
	print_geometry(&geometry);
	printk("\n");
	printk("gendisk\n");
	print_gendisk(bd_disk);




	return 0;
}


//struct block_device {
//	dev_t			bd_dev;  /* not a kdev_t - it's a search key */
//	int			bd_openers;
//	struct inode *		bd_inode;	/* will die */
//	struct super_block *	bd_super;
//	struct mutex		bd_mutex;	/* open/close mutex */
//	struct list_head	bd_inodes;
//	void *			bd_claiming;
//	void *			bd_holder;
//	int			bd_holders;
//	bool			bd_write_holder;
//#ifdef CONFIG_SYSFS
//	struct list_head	bd_holder_disks;
//#endif
//	struct block_device *	bd_contains;
//	unsigned		bd_block_size;
//	struct hd_struct *	bd_part;
//	/* number of times partitions within this device have been opened. */
//	unsigned		bd_part_count;
//	int			bd_invalidated;
//	struct gendisk *	bd_disk;
//	struct request_queue *  bd_queue;
//	struct list_head	bd_list;
//	/*
//	 * Private data.  You must have bd_claim'ed the block_device
//	 * to use this.  NOTE:  bd_claim allows an owner to claim
//	 * the same device multiple times, the owner must take special
//	 * care to not mess up bd_private for that case.
//	 */
//	unsigned long		bd_private;

//	/* The counter of freeze processes */
//	int			bd_fsfreeze_count;
//	/* Mutex for freeze */
//	struct mutex		bd_fsfreeze_mutex;
//};


static int print_hd_struct(struct hd_struct * hd) {
	struct device *ddev;
	//printk("MAJOR:%d MINOR:%d \n",MAJOR(bdev->bd_dev),MINOR(bdev->bd_dev));
	printk("start_sect:%ld \n",hd->start_sect);
	printk("nr_sects:%ld \n",hd->nr_sects);
	printk("nr_sects_seq:%d \n",hd->nr_sects_seq.sequence);
	printk("alignment_offset:%ld \n",hd->alignment_offset);
	printk("discard_alignment:%d \n",hd->discard_alignment);
	//printk("__dev:%s \n",hd->__dev);
	printk("policy:%d \n",hd->policy);
	printk("partno:%d \n",hd->partno);
	printk("stamp:%ld \n",hd->stamp);
	printk("discard_alignment:%d \n",hd->discard_alignment);
	printk("uuid:%s \n",hd->info->uuid);
	printk("volname:%s \n",hd->info->volname);
	print_disk_stats(hd->dkstats);
	ddev = &(hd->__dev);
	printk("print_device \n");
	print_device(ddev);
	return 0;
}


//struct hd_struct {
//	sector_t start_sect;
//	/*
//	 * nr_sects is protected by sequence counter. One might extend a
//	 * partition while IO is happening to it and update of nr_sects
//	 * can be non-atomic on 32bit machines with 64bit sector_t.
//	 */
//	sector_t nr_sects;
//	seqcount_t nr_sects_seq;
//	sector_t alignment_offset;
//	unsigned int discard_alignment;
//	struct device __dev;
//	struct kobject *holder_dir;
//	int policy, partno;
//	struct partition_meta_info *info;
//#ifdef CONFIG_FAIL_MAKE_REQUEST
//	int make_it_fail;
//#endif
//	unsigned long stamp;
//	atomic_t in_flight[2];
//#ifdef	CONFIG_SMP
//	struct disk_stats __percpu *dkstats;
//#else
//	struct disk_stats dkstats;
//#endif
//	struct percpu_ref ref;
//	struct rcu_head rcu_head;
//};


static int print_device(struct device *ddev) {
	//printk("MAJOR:%d MINOR:%d \n",MAJOR(bdev->bd_dev),MINOR(bdev->bd_dev));
	printk("init_name:%s \n",ddev->init_name);
	if (ddev->bus!=NULL) {
		printk("bus->name:%s \n",ddev->bus->name);
		printk("bus->dev_name:%s \n",ddev->bus->dev_name);
	}
	return 0;
}


//struct device {
//	struct device		*parent;

//	struct device_private	*p;

//	struct kobject kobj;
//	const char		*init_name; /* initial name of the device */
//	const struct device_type *type;

//	struct mutex		mutex;	/* mutex to synchronize calls to
//					 * its driver.
//					 */

//	struct bus_type	*bus;		/* type of bus device is on */
//	struct device_driver *driver;	/* which driver has allocated this
//					   device */
//	void		*platform_data;	/* Platform specific data, device
//					   core doesn't touch it */
//	void		*driver_data;	/* Driver data, set and get with
//					   dev_set/get_drvdata */
//	struct dev_pm_info	power;
//	struct dev_pm_domain	*pm_domain;

//#ifdef CONFIG_GENERIC_MSI_IRQ_DOMAIN
//	struct irq_domain	*msi_domain;
//#endif
//#ifdef CONFIG_PINCTRL
//	struct dev_pin_info	*pins;
//#endif
//#ifdef CONFIG_GENERIC_MSI_IRQ
//	struct list_head	msi_list;
//#endif

//#ifdef CONFIG_NUMA
//	int		numa_node;	/* NUMA node this device is close to */
//#endif
//	u64		*dma_mask;	/* dma mask (if dma'able device) */
//	u64		coherent_dma_mask;/* Like dma_mask, but for
//					     alloc_coherent mappings as
//					     not all hardware supports
//					     64 bit addresses for consistent
//					     allocations such descriptors. */
//	unsigned long	dma_pfn_offset;

//	struct device_dma_parameters *dma_parms;

//	struct list_head	dma_pools;	/* dma pools (if dma'ble) */

//	struct dma_coherent_mem	*dma_mem; /* internal for coherent mem
//					     override */
//#ifdef CONFIG_DMA_CMA
//	struct cma *cma_area;		/* contiguous memory area for dma
//					   allocations */
//#endif
//	/* arch specific additions */
//	struct dev_archdata	archdata;

//	struct device_node	*of_node; /* associated device tree node */
//	struct fwnode_handle	*fwnode; /* firmware device node */

//	dev_t			devt;	/* dev_t, creates the sysfs "dev" */
//	u32			id;	/* device instance */

//	spinlock_t		devres_lock;
//	struct list_head	devres_head;

//	struct klist_node	knode_class;
//	struct class		*class;
//	const struct attribute_group **groups;	/* optional groups */

//	void	(*release)(struct device *dev);
//	struct iommu_group	*iommu_group;

//	bool			offline_disabled:1;
//	bool			offline:1;
//};





static int print_gendisk(struct gendisk * gd) {
	struct disk_part_tbl __rcu *part_tbl;
	int i;
	printk("major:%d \n",gd->major);
	printk("first_minor:%d \n",gd->first_minor);
	printk("minors:%d \n",gd->minors);
	printk("disk_name:%s \n",gd->disk_name);
	printk("events:%d \n",gd->events);
	printk("async_events:%d \n",gd->async_events);
	printk("queue:%p \n",gd->queue);
	printk("fops.open:%p \n",gd->fops->open);  //sd_open              如果是virtio磁盘，这里为null
	printk("fops.getgeo:%p \n",gd->fops->getgeo); //sd_getgeo                  如果是virtio磁盘 cat /proc/kallsyms |grep ffffffff815918f0     ffffffff815918f0 t virtblk_getgeo




	printk("----------- start print gd->part0 -----------\n");
	print_hd_struct(&gd->part0);
	printk("----------- end print gd->part0 -----------\n");
	part_tbl = gd->part_tbl;
	for (i=0; i<part_tbl->len; i++) {
		printk("----------- start print part_tbl.part[%d] -----------\n",i);
		if (part_tbl->part[i] == NULL) {
			printk("null \n");
		} else {
			print_hd_struct(part_tbl->part[i]);
		}
		printk("----------- end print part_tbl.part[%d] -----------\n",i);
	}
	return 0;
}


//struct gendisk {
//	/* major, first_minor and minors are input parameters only,
//	 * don't use directly.  Use disk_devt() and disk_max_parts().
//	 */
//	int major;			/* major number of driver */
//	int first_minor;
//	int minors;                     /* maximum number of minors, =1 for
//                                         * disks that can't be partitioned. */

//	char disk_name[DISK_NAME_LEN];	/* name of major driver */
//	char *(*devnode)(struct gendisk *gd, umode_t *mode);

//	unsigned int events;		/* supported events */
//	unsigned int async_events;	/* async events, subset of all */

//	/* Array of pointers to partitions indexed by partno.
//	 * Protected with matching bdev lock but stat and other
//	 * non-critical accesses use RCU.  Always access through
//	 * helpers.
//	 */
//	struct disk_part_tbl __rcu *part_tbl;
//	struct hd_struct part0;

//	const struct block_device_operations *fops;
//	struct request_queue *queue;
//	void *private_data;

//	int flags;
//	struct device *driverfs_dev;  // FIXME: remove
//	struct kobject *slave_dir;

//	struct timer_rand_state *random;
//	atomic_t sync_io;		/* RAID */
//	struct disk_events *ev;
//#ifdef  CONFIG_BLK_DEV_INTEGRITY
//	struct kobject integrity_kobj;
//#endif	/* CONFIG_BLK_DEV_INTEGRITY */
//	int node_id;
//};


static int print_geometry(struct hd_geometry * hg) {
	printk("heads:%d \n",hg->heads);
	printk("sectors:%d \n",hg->sectors);
	printk("cylinders:%d \n",hg->cylinders);
	printk("start:%lu \n",hg->start);
	return 0;
}


//struct hd_geometry {
//      unsigned char heads;
//      unsigned char sectors;
//      unsigned short cylinders;
//      unsigned long start;
//};


static int print_disk_stats(struct disk_stats __percpu *dkstats) {
	unsigned long sectors[2]= {0,0};	/* READs and WRITEs */
	unsigned long ios[2]= {0,0};
	unsigned long merges[2]= {0,0};
	unsigned long ticks[2]= {0,0};
	unsigned long io_ticks =0;
	unsigned long time_in_queue=0;
	int i;
	for_each_possible_cpu(i) {
		sectors[0] += per_cpu_ptr(dkstats, i)->sectors[0];
		sectors[1] += per_cpu_ptr(dkstats, i)->sectors[1];
		ios[0] += per_cpu_ptr(dkstats, i)->ios[0];
		ios[1] += per_cpu_ptr(dkstats, i)->ios[1];
		merges[0] += per_cpu_ptr(dkstats, i)->merges[0];
		merges[1] += per_cpu_ptr(dkstats, i)->merges[1];
		ticks[0] += per_cpu_ptr(dkstats, i)->ticks[0];
		ticks[1] += per_cpu_ptr(dkstats, i)->ticks[1];
		io_ticks += per_cpu_ptr(dkstats, i)->io_ticks;
		time_in_queue += per_cpu_ptr(dkstats, i)->time_in_queue;
	}
	printk("%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu\n",sectors[0],sectors[1],ios[0],ios[1], merges[0], merges[1],ticks[0],ticks[1],io_ticks,time_in_queue);
	return 0;
}

//struct disk_stats {
//	unsigned long sectors[2];	/* READs and WRITEs */
//	unsigned long ios[2];
//	unsigned long merges[2];
//	unsigned long ticks[2];
//	unsigned long io_ticks;
//	unsigned long time_in_queue;
//};








static int __init print_block_device_init(void) {
	printk("<1>enter print_block_device ---------------------!\n");
	print_block_device();
	return 0;
}

static void __exit print_block_device_exit(void) {
	printk("<1>exit print_block_device ---------------------!\n");
}

module_init(print_block_device_init);
module_exit(print_block_device_exit);
MODULE_LICENSE("GPL");






/*

root@ubuntu:/home/code/github/Costor-micro-service# fdisk -l /dev/vda
Disk /dev/vda: 30 GiB, 32212254720 bytes, 62914560 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0xe28315ac

Device     Boot   Start      End  Sectors  Size Id Type
/dev/vda1  *       2048   999423   997376  487M 83 Linux
/dev/vda2       1001470 62912511 61911042 29.5G  5 Extended
/dev/vda5       1001472 62912511 61911040 29.5G 8e Linux LVM

blkid
/dev/vda1: UUID="97921364-83a5-45f3-b77d-eda828e1517a" TYPE="ext2" PARTUUID="e28315ac-01"
/dev/vda5: UUID="U0Bnrj-bx6f-JoDs-zWcc-Wcst-JF9w-G9lZrP" TYPE="LVM2_member" PARTUUID="e28315ac-05"
/dev/vdb: UUID="170b5d5c-0075-4123-88bf-742310c68917" TYPE="xfs"
/dev/vdd: UUID="1c47b5f9-2ce1-47da-b151-c29f7e1b771c" TYPE="xfs"
/dev/vdg: UUID="49230143-d799-4dc8-b457-b6622a1c5c1b" TYPE="ext4"
/dev/mapper/ubuntu--vg-root: UUID="20e7d465-08da-4d17-bcce-3100715ef1de" TYPE="ext4"
/dev/mapper/ubuntu--vg-swap_1: UUID="961b4263-c841-4ff9-81d9-c9dcab77c05e" TYPE="swap"


./test /dev/vdd
dmesg -c; rmmod print_block_device; insmod print_block_device.ko pid=3436 fd=3

dmesg

[ 4076.103259] <1>enter print_block_device ---------------------!
[ 4076.103263] process:test,pid:3436
[ 4076.103266] MAJOR:253 MINOR:0
[ 4076.103268] bd_openers:3
[ 4076.103270] bd_holders:3
[ 4076.103271] bd_write_holder:0
[ 4076.103272] bd_block_size:4096
[ 4076.103273] bd_part_count:2
[ 4076.103274] bd_invalidated:0
[ 4076.103275] bd_fsfreeze_count:0
[ 4076.103277] bd_queue:ffff880035988000

[ 4076.103279] bd_part
[ 4076.103280] start_sect:0
[ 4076.103281] nr_sects:62914560
[ 4076.103282] nr_sects_seq:0
[ 4076.103283] alignment_offset:0
[ 4076.103284] discard_alignment:0
[ 4076.103285] policy:0
[ 4076.103286] partno:0
[ 4076.103288] stamp:4295910481
[ 4076.103289] discard_alignment:0
[ 4076.103290] uuid:(null)
[ 4076.103291] volname:(null)


[ 5368.156750] geometry
[ 5368.156758] heads:16
[ 5368.156767] sectors:63
[ 5368.156773] cylinders:16383
[ 5368.156775] start:18446744072635936768


[ 4076.103294] gendisk
[ 4076.103295] major:253
[ 4076.103296] first_minor:0
[ 4076.103297] minors:16
[ 4076.103298] disk_name:vda
[ 4076.103299] events:0
[ 4076.103300] async_events:0
[ 4076.103301] queue:ffff880035988000
[ 4076.103303] fops.open:          (null)
[ 4076.103304] fops.getgeo:ffffffff815918f0
[ 4076.103305] ----------- start print gd->part0 -----------
[ 4076.103306] start_sect:0
[ 4076.103307] nr_sects:62914560
[ 4076.103308] nr_sects_seq:0
[ 4076.103309] alignment_offset:0
[ 4076.103310] discard_alignment:0
[ 4076.103311] policy:0
[ 4076.103312] partno:0
[ 4076.103313] stamp:4295910481
[ 4076.103314] discard_alignment:0
[ 4076.103315] uuid:(null)
[ 4076.103316] volname:(null)
[ 4076.103317] ----------- end print gd->part0 -----------
[ 4076.103319] ----------- start print part_tbl.part[0] -----------
[ 4076.103320] start_sect:0
[ 4076.103321] nr_sects:62914560
[ 4076.103322] nr_sects_seq:0
[ 4076.103323] alignment_offset:0
[ 4076.103324] discard_alignment:0
[ 4076.103325] policy:0
[ 4076.103326] partno:0
[ 4076.103327] stamp:4295910481
[ 4076.103328] discard_alignment:0
[ 4076.103329] uuid:(null)
[ 4076.103330] volname:(null)
[ 4076.103331] ----------- end print part_tbl.part[0] -----------
[ 4076.103332] ----------- start print part_tbl.part[1] -----------
[ 4076.103333] start_sect:2048
[ 4076.103334] nr_sects:997376
[ 4076.103335] nr_sects_seq:0
[ 4076.103336] alignment_offset:0
[ 4076.103337] discard_alignment:0
[ 4076.103338] policy:0
[ 4076.103339] partno:1
[ 4076.103340] stamp:4295732625
[ 4076.103342] discard_alignment:0
[ 4076.103343] uuid:e28315ac-01
[ 4076.103344] volname:
[ 4076.103345] ----------- end print part_tbl.part[1] -----------
[ 4076.103346] ----------- start print part_tbl.part[2] -----------
[ 4076.103347] start_sect:1001470
[ 4076.103348] nr_sects:2
[ 4076.103349] nr_sects_seq:0
[ 4076.103350] alignment_offset:0
[ 4076.103351] discard_alignment:0
[ 4076.103352] policy:0
[ 4076.103353] partno:2
[ 4076.103354] stamp:4294894728
[ 4076.103355] discard_alignment:0
[ 4076.103356] uuid:
[ 4076.103357] volname:
[ 4076.103358] ----------- end print part_tbl.part[2] -----------
[ 4076.103359] ----------- start print part_tbl.part[3] -----------
[ 4076.103360] null
[ 4076.103362] ----------- end print part_tbl.part[3] -----------
[ 4076.103363] ----------- start print part_tbl.part[4] -----------
[ 4076.103364] null
[ 4076.103365] ----------- end print part_tbl.part[4] -----------
[ 4076.103366] ----------- start print part_tbl.part[5] -----------
[ 4076.103367] start_sect:1001472
[ 4076.103368] nr_sects:61911040
[ 4076.103369] nr_sects_seq:0
[ 4076.103370] alignment_offset:0
[ 4076.103371] discard_alignment:0
[ 4076.103372] policy:0
[ 4076.103373] partno:5
[ 4076.103374] stamp:4295910481
[ 4076.103375] discard_alignment:0
[ 4076.103376] uuid:e28315ac-05
[ 4076.103377] volname:
[ 4076.103378] ----------- end print part_tbl.part[5] -----------




fdisk -l /dev/sda
Disk /dev/sda: 1 GiB, 1073741824 bytes, 2097152 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
./test /dev/sda
dmesg -c; rmmod print_block_device; insmod print_block_device.ko pid=3436 fd=3

dmesg


[ 4424.285906] <1>enter print_block_device ---------------------!
[ 4424.285913] process:test,pid:5303
[ 4424.285918] MAJOR:8 MINOR:0
[ 4424.285920] bd_openers:1
[ 4424.285923] bd_holders:0
[ 4424.285926] bd_write_holder:0
[ 4424.285929] bd_block_size:4096
[ 4424.285932] bd_part_count:0
[ 4424.285935] bd_invalidated:0
[ 4424.285938] bd_fsfreeze_count:0
[ 4424.285941] bd_queue:ffff8800350b9a18

[ 4424.285947] bd_part
[ 4424.285950] start_sect:0
[ 4424.285953] nr_sects:2097152
[ 4424.285955] nr_sects_seq:0
[ 4424.285958] alignment_offset:0
[ 4424.285961] discard_alignment:0
[ 4424.285963] policy:0
[ 4424.285966] partno:0
[ 4424.285968] stamp:4295996954
[ 4424.285970] discard_alignment:0
[ 4424.285972] uuid:(null)
[ 4424.285973] volname:(null)

[ 5328.570997] geometry
[ 5328.570998] heads:34
[ 5328.571000] sectors:61
[ 5328.571001] cylinders:1011
[ 5328.571003] start:18446744072635936768


[ 4424.285978] gendisk
[ 4424.285980] major:8
[ 4424.285982] first_minor:0
[ 4424.285984] minors:16
[ 4424.285986] disk_name:sda
[ 4424.285987] events:0
[ 4424.285989] async_events:0
[ 4424.285991] queue:ffff8800350b9a18
[ 4424.285993] fops.open:ffffffff815dc840
[ 4424.285995] fops.getgeo:ffffffff815ddaa0
[ 4424.285997] ----------- start print gd->part0 -----------
[ 4424.285999] start_sect:0
[ 4424.286001] nr_sects:2097152
[ 4424.286002] nr_sects_seq:0
[ 4424.286004] alignment_offset:0
[ 4424.286006] discard_alignment:0
[ 4424.286008] policy:0
[ 4424.286009] partno:0
[ 4424.286011] stamp:4295996954
[ 4424.286013] discard_alignment:0
[ 4424.286015] uuid:(null)
[ 4424.286017] volname:(null)
[ 4424.286018] ----------- end print gd->part0 -----------
[ 4424.286021] ----------- start print part_tbl.part[0] -----------
[ 4424.286022] start_sect:0
[ 4424.286024] nr_sects:2097152
[ 4424.286026] nr_sects_seq:0
[ 4424.286028] alignment_offset:0
[ 4424.286029] discard_alignment:0
[ 4424.286031] policy:0
[ 4424.286033] partno:0
[ 4424.286035] stamp:4295996954
[ 4424.286037] discard_alignment:0
[ 4424.286038] uuid:(null)
[ 4424.286040] volname:(null)
[ 4424.286042] ----------- end print part_tbl.part[0] -----------

cat /proc/kallsyms |grep ffffffff815ddaa0
ffffffff815ddaa0 t sd_getgeo
root@ubuntu:/home/code/github/chenzhongtao/mysource/kernel-test/print_block_device# cat /proc/kallsyms |grep ffffffff815dc840
ffffffff815dc840 t sd_open


*/
