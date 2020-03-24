/*
 *Name: jiffies.c
 *Program: Kernel Intro
 *Authors: Brayden Faulkner and Hayden Nanny 
 *
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>

#define BUFFER_SIZE 128

#define PROC_NAME "jiffies"
#define MESSAGE "Hello World\n"

/*
 * Function prototypes
*/

/*
 *Name: ssize_t proc_read
 *Purpose: Runs when ever proc/jiffies is read and returns current jiffies vales
 *Parameters:
 *	struct file *file: custom struct that sets module owner and the read process function
 *	char *buf: stores the user space buffer
 *	size_t count:
 *	loff_t *pos:
*/
ssize_t proc_read(struct file *file, char *buf, size_t count, loff_t *pos);

//Struct that stores the module owner and the read process function
static struct file_operations proc_ops = {
        .owner = THIS_MODULE,
        .read = proc_read,
};

/*
 *Name: int proc_int
 *Purpose: Runs whenever the module is loaded
 *Parameters: None
*/
int proc_init(void)
{

        // creates the /proc/jiffies entry
        proc_create(PROC_NAME, 0, NULL, &proc_ops);
	//Prints confirmation message to kernel output to confirm module was loaded
        printk(KERN_INFO "/proc/%s created\n", PROC_NAME);

	return 0;
}

/*
 *Name: void proc_exit
 *Purpose: Runs whenever the module is removed
 *Parameters: None
*/
void proc_exit(void) {

        // removes the /proc/jiffies entry
        remove_proc_entry(PROC_NAME, NULL);
	// prints confirmation message
        printk( KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

//Implentation of proc_read
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
	//gets current value of jiffies
	unsigned long localJiff = jiffies;
	//Stores the return valie
        int rv = 0;
	//Stores the string that will be loaded into the user space buffer
        char buffer[BUFFER_SIZE];
	//Stores whether the operation has been completed or not
        static int completed = 0;
	//Checks if the function has been completed
        if (completed) {
                completed = 0;
                return 0;
        }
	//Says the function has been completed
        completed = 1;
	//Sets the return value and what will be loaded into buffer
        rv = sprintf(buffer, "Jiffies = %lu\n",localJiff);
        // copies the contents of buffer to userspace buffer
        copy_to_user(usr_buf, buffer, rv);

        return rv;
}


/* Macros for registering module entry and exit points. */
module_init( proc_init );
module_exit( proc_exit );


