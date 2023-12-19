#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEVICE_NAME "chardev"
#define BUF_LEN 80

MODULE_LICENSE("GPL");

static int major;
static char msg[BUF_LEN];
static short readPos;
static int isOpen = 0;

static int chardev_open(struct inode *inode, struct file *filp) {
    if (isOpen)
        return -EBUSY;
    isOpen++;
    return 0;
}

static int chardev_release(struct inode *inode, struct file *filp) {
    isOpen--;
    return 0;
}

static ssize_t chardev_read(struct file *filp, char *buffer, size_t length, loff_t * offset) {
    short bytesRead = 0;
    while (length && (msg[readPos] != 0)) {
        put_user(msg[readPos], buffer++);
        bytesRead++;
        length--;
        readPos++;
    }
    return bytesRead;
}

static ssize_t chardev_write(struct file *filp, const char *buffer, size_t length, loff_t * offset) {
    int i;
    for (i = 0; i < length && i < BUF_LEN; i++)
        get_user(msg[i], buffer + i);
    readPos = 0;
    return i;
}

static struct file_operations fops = {
    .read = chardev_read,
    .write = chardev_write,
    .open = chardev_open,
    .release = chardev_release
};

static int chardev_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major;
    }

    printk(KERN_INFO "Chardev module has been registered with major number %d\n", major);
    return 0;
}

static void chardev_exit(void) {
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "Chardev module has been unregistered\n");
}

module_init(chardev_init);
module_exit(chardev_exit);
