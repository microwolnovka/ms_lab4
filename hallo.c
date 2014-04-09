#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#define MAX_PROC_SIZE 100

static int a;
static int b;
static char operation;
static int result;

static struct proc_dir_entry *proc_write_entry;
static struct proc_dir_entry *proc_a_entry;
static struct proc_dir_entry *proc_b_entry;
static struct proc_dir_entry *proc_result_entry;

int read_a(char *buf,char **start,off_t offset,int count,int *eof,void *data )
{
	int len=0;
	len = sprintf(buf,"\n %d\n ",a);

	return len;
}

int write_a(struct file *file, const char *buf, int count, void *data )
{
	static char temp[MAX_PROC_SIZE];
	if(count > MAX_PROC_SIZE)
	    count = MAX_PROC_SIZE;
	if(copy_from_user(temp, buf, count))
	    return -EFAULT;
	sscanf(temp, "%d", &a);

	return count;
}
int read_b(char *buf,char **start,off_t offset,int count,int *eof,void *data )
{	
	int len=0;
	len = sprintf(buf,"\n %d\n ",b);
	
	return len;
}

int write_b(struct file *file, const char *buf, int count, void *data )
{
	static char temp[MAX_PROC_SIZE];
	if(count > MAX_PROC_SIZE)
	    count = MAX_PROC_SIZE;
	if(copy_from_user(temp, buf, count))
	    return -EFAULT;

	sscanf(temp, "%d", &b);
	return count;

}
int read_operation(char *buf,char **start,off_t offset,int count,int *eof,void *data )
{
	int len=0;
	len = sprintf(buf,"\n %c\n ",operation);

	return len;
}

int write_operation(struct file *file, const char *buf, int count, void *data )
{

	static char temp[MAX_PROC_SIZE];
	if(count > MAX_PROC_SIZE)
	    count = MAX_PROC_SIZE;
	if(copy_from_user(temp, buf, count))
	    return -EFAULT;
	operation = temp[0];

	return count;
}

void countRes()
{
	if (operation == '+'){
		result = a + b;
	} else if (operation == '-'){
		result = a - b;
	} else if (operation == '*'){
		result = a * b;
	} else if (operation == '/'){
		result = a / b;
	}
}

int read_result(char *buf,char **start,off_t offset,int count,int *eof,void *data )
{
	int len=0;

	countRes();
	len = sprintf(buf,"\n %d\n ",result);

	return len;
}

void create_new_proc_entry()
{
	proc_write_entry = create_proc_entry("oper1",0666,NULL);
	if (!proc_write_entry) {	
		printk(KERN_INFO "Error creating proc entry");
		return -ENOMEM;
	}
	proc_write_entry->read_proc = read_operation ;
	proc_write_entry->write_proc = write_operation;


	proc_a_entry = create_proc_entry("a",0666,NULL);
	if (!proc_a_entry) {	
		printk(KERN_INFO "Error creating proc entry");
		return -ENOMEM;
	}
	proc_a_entry->read_proc = read_a ;
	proc_a_entry->write_proc = write_a;


	proc_b_entry = create_proc_entry("b",0666,NULL);
	if (!proc_b_entry) {	
		printk(KERN_INFO "Error creating proc entry");
		return -ENOMEM;
	}
	proc_b_entry->read_proc = read_b ;
	proc_b_entry->write_proc = write_b;


	proc_result_entry = create_proc_entry("result",0666,NULL);
	if (!proc_result_entry) {	
		printk(KERN_INFO "Error creating proc entry");
		return -ENOMEM;
	}
	proc_result_entry->read_proc = read_result;
}

int proc_init (void) 
{
    create_new_proc_entry();
    return 0;
}

void proc_cleanup(void) 
{
    printk(KERN_INFO " Inside cleanup_module\n");
    remove_proc_entry("proc_entry",NULL);
}
MODULE_LICENSE("GPL");   
module_init(proc_init);
module_exit(proc_cleanup);
 
