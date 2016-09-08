/*
 *  hello-5.c - Demonstrates command line argument passing to a module.
 */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Peter Jay Salzman");

static short int myshort = 1;
static int myint = 420;
static long int mylong = 9999;
static char *mystring = "blah";
static int myintArray[2] = { -1, -1 };
static int arr_argc = 0;

/* 
 * module_param(foo, int, 0000)
 * The first param is the parameters name
 * The second param is it's data type
 * The final argument is the permissions bits, 
 * for exposing parameters in sysfs (if non-zero) at a later stage.
 */

module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");
module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");

/*
 * module_param_array(name, type, num, perm);
 * The first param is the parameter's (in this case the array's) name
 * The second param is the data type of the elements of the array
 * The third argument is a pointer to the variable that will store the number 
 * of elements of the array initialized by the user at module loading time
 * The fourth argument is the permission bits
 */
module_param_array(myintArray, int, &arr_argc, 0000);
MODULE_PARM_DESC(myintArray, "An array of integers");

static int __init hello_5_init(void)
{
	int i;
	printk(KERN_ERR "Hello, world 5\n=============\n");
	printk(KERN_ERR "myshort is a short integer: %hd\n", myshort);
	printk(KERN_ERR "myint is an integer: %d\n", myint);
	printk(KERN_ERR "mylong is a long integer: %ld\n", mylong);
	printk(KERN_ERR "mystring is a string: %s\n", mystring);
	for (i = 0; i < (sizeof myintArray / sizeof (int)); i++)
	{
		printk(KERN_ERR "myintArray[%d] = %d\n", i, myintArray[i]);
	}
	printk(KERN_ERR "got %d arguments for myintArray.\n", arr_argc);
	return 0;
}

static void __exit hello_5_exit(void)
{
	printk(KERN_ERR "Goodbye, world 5\n");
}

module_init(hello_5_init);
module_exit(hello_5_exit);

/*
insmod hello-5.ko mystring="bebop" myint=255 myintArray=1,2

Sep  6 10:58:06 zhong8 kernel: Hello, world 5
Sep  6 10:58:06 zhong8 kernel: =============
Sep  6 10:58:06 zhong8 kernel: myshort is a short integer: 1
Sep  6 10:58:06 zhong8 kernel: myint is an integer: 255
Sep  6 10:58:06 zhong8 kernel: mylong is a long integer: 9999
Sep  6 10:58:06 zhong8 kernel: mystring is a string: bebop
Sep  6 10:58:06 zhong8 kernel: myintArray[0] = 1
Sep  6 10:58:06 zhong8 kernel: myintArray[1] = 2
Sep  6 10:58:06 zhong8 kernel: got 2 arguments for myintArray.
*/
