#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>      // for char dev related functions
#include <linux/uaccess.h> // for put_user()

int init_module(void);
void cleanup_module(void);

static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset);
static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset);

#define SUCCESS 0
#define DEVICE_NAME "chardev"
#define BUF_LEN 80

static int major;
static int device_opened = 0;

static char msg[BUF_LEN];
static char *msg_ptr;

static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write};

int init_module(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0)
    {
        printk(KERN_ALERT "Registering char device failed with %d\n", major);
        return major;
    }
    printk(KERN_INFO "chardev::Major number: %d\n", major);
    printk(KERN_INFO "chardev::Device name: %s\n", DEVICE_NAME);

    return SUCCESS;
}

void cleanup_module(void)
{
    unregister_chrdev(major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file)
{
    static int counter = 0;

    if (device_opened)
        return -EBUSY;
    device_opened++;

    sprintf(msg, "I already told you %d times. Hello world!\n", counter++);
    msg_ptr = msg;

    try_module_get(THIS_MODULE);

    return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
    device_opened--;
    module_put(THIS_MODULE);
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    int bytes_read = 0;

    if (*msg_ptr == 0)
        return 0;

    while (length && *msg_ptr)
    {
        put_user(*(msg_ptr++), buffer++);
        length--;
        bytes_read++;
    }

    return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset)
{
    printk(KERN_ALERT "This operation is not supported\n");
    return -EINVAL;
}