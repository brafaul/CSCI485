/* Author(s): Brayden Faulkner & Hayden Nanney
 * Date: 2/27/2020
 * File Name: seconds.c
 * Description: this file is a loadable kernel module that displays the elapsed time since the module was loaded.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <asm/param.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

#define BUFFER_SIZE 128

#define PROC_NAME "seconds"

unsigned long startJiffies; //contains the value of jiffies at initialization

//store the module owner and read proc function in struct
static struct file_operations proc_ops = {
	.owner = THIS_MODULE,
	.read = proc_read,
};

/*** FUNCTION DESCRIPTIONS ***/

ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);
/* Description:  This function is called each time /proc/seconds is read.
 *
 */

int proc_init(void);
/* Description: This function is called when the module is loaded.
 *
 */

void proc_exit(void);
/*Description:This function is called when the module is removed.
 *
 */

/*** FUNCTIONS ***/

int proc_init(void){

	//create proc/seconds
	proc_create(PROC_NAME, 0, NULL, &proc_ops);
	
	//print to kernel that the process was loaded
	printk(KERN_INFO "/proc/%s created\n",PROC_NAME);

	//get initial jiffies when seconds is initialized
	startJiffies = jiffies;

	return 0;
}

void proc_exit(void) {

	//remove proc/seconds
	remove_proc_entry(PROC_NAME, NULL);

	//print removal message to kernel
	printk(KERN_INFO "/proc/%s removed\n" PROC_NAME);
}

ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos){
	int rv = 0; //return value

	//get current value of jiffies and HZ
	unsigned long localJiff = jiffies;
	unsigned long clockrate = HZ;

	char buffer[BUFFER_SIZE]; //store string
	static int completed = 0; //stores completion state

	//check if funxtion is complete
	if(completed){
		completed = 0;
		return 0;
	}

	//function complete
	completed = 1;

	//set return value and load string into buffer
	rv = sprintf(buffer,"Seconds = %lu\n",(localJiff - startJiffies) / clockrate);
	
	//copy buffer to userspace buffer
	copy_to_user(usr_buf,buffer,rv);
	
	return rv;
}

/*Macros for registering module entry and exit points*/
module_init(proc_init);
module_exit(proc_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Seconds Module");
MODULE_AUTHOR("HN and BF");
