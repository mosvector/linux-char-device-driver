#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define DEVICE_NAME "mosvector"
#define MESSAGE "Hello, World!\n"

static dev_t dev_num;
static struct cdev mosvector_cdev;
static struct class *mosvector_class;

// Called when the device is opened
static int mosvector_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "mosvector: Device opened\n");
    return 0;
}

// Called when the device is closed
static int mosvector_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "mosvector: Device closed\n");
    return 0;
}

// Called when the device is read from
static ssize_t mosvector_read(struct file *file, char __user *buffer, size_t len, loff_t *offset) {
    ssize_t msg_len = strlen(MESSAGE);

    if (*offset >= msg_len) // End of message
        return 0;

    if (len > msg_len - *offset)
        len = msg_len - *offset;

    if (copy_to_user(buffer, MESSAGE + *offset, len) != 0)
        return -EFAULT;

    *offset += len;
    return len;
}

// File operations for the character device
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = mosvector_open,
    .release = mosvector_release,
    .read = mosvector_read,
};

// Module initialization
static int __init mosvector_init(void) {
    // Allocate a device number
    if (alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME) < 0) {
        printk(KERN_ALERT "mosvector: Failed to allocate a device number\n");
        return -1;
    }

    // Create device class with only the name argument
    mosvector_class = class_create(DEVICE_NAME);
    if (IS_ERR(mosvector_class)) {
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "mosvector: Failed to create device class\n");
        return PTR_ERR(mosvector_class);
    }

    // Initialize and add character device
    cdev_init(&mosvector_cdev, &fops);
    if (cdev_add(&mosvector_cdev, dev_num, 1) < 0) {
        class_destroy(mosvector_class);
        unregister_chrdev_region(dev_num, 1);
        printk(KERN_ALERT "mosvector: Failed to add cdev\n");
        return -1;
    }

    // Create the device node
    device_create(mosvector_class, NULL, dev_num, NULL, DEVICE_NAME);

    printk(KERN_INFO "mosvector: Device initialized\n");
    return 0;
}

// Module cleanup
static void __exit mosvector_exit(void) {
    device_destroy(mosvector_class, dev_num);
    class_destroy(mosvector_class);
    cdev_del(&mosvector_cdev);
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_INFO "mosvector: Device removed\n");
}

module_init(mosvector_init);
module_exit(mosvector_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Raymond Lin");
MODULE_DESCRIPTION("A simple Linux character device driver that returns 'Hello, World!'");
MODULE_VERSION("1.0");
