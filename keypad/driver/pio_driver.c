/*Basic USB PIO driver that doesn't really do much for now apart from probe and create 2 device files in /dev
Nathan
*/
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/slab.h>
#include <linux/usb.h>
#include <linux/mutex.h>
#include <linux/ioctl.h>
#include <asm/uaccess.h>

/* pio specifics */
#define PIO_VENDOR_ID 0x09ca
#define PIO_PRODUCT_ID 0x5544
#define PIO_NODE "pio"
#define PIO_STOP 0x1

#define INTERUPT_EP_3_IN 0x83
#define BULK_EP_2_IN 0x82
#define BULK_EP_1_OUT 0x01

#define PIO_CTRL_BUFFER_SIZE 8
#define PIO_CTRL_REQUEST_TYPE      0x21
#define PIO_CTRL_REQUEST           0x09
#define PIO_CTRL_VALUE             0x0        
#define PIO_CTRL_INDEX             0x0
#define PIO_CTRL_COMMAND_PREFIX    0x02
struct pio_driver
{
  struct usb_device *udev;
  struct usb_interface *interface;
  unsigned char minor;
  char serial_number[8];
  int open_count;
  struct semaphore sem;
  spinlock_t cmd_spinlock;
  char *int_in_buffer;
  struct usb_endpoint_descriptor *int_in_endpoint;
  struct urb *int_in_urb;
  int int_in_running;
  char *ctrl_buffer;
  struct urb *ctrl_urb;
  struct usb_ctrlrequest *ctrl_dr;
  int correction_required;
  __u8 command;
};

static struct usb_class_driver class;

/* Table of devices that work with this driver */
static struct usb_device_id pio_table[] =
  {
    { USB_DEVICE(PIO_VENDOR_ID, PIO_PRODUCT_ID) },
    {} /* Terminating entry */
  };
MODULE_DEVICE_TABLE (usb, pio_table);

static struct usb_driver pio_driver =
  {
    .name = "pio_driver",
    .id_table = pio_table,
  };

static void pio_ctrl_callback(struct urb *urb)
{
  //pr_debug("does ctrl_callback appear?\n");
        struct pio_driver *dev = urb->context;
        pr_debug("pio_ctrl_callback\n");
        dev->correction_required = 0;        /* TODO: do we need race protection? */
}

static int pio_open(struct inode *inodep, struct file *filp)
{
  int retval = 0;
  pr_debug("pio_open\n");
  /* Save our object in the file's private structure. */
  /* filp->private_data = dev; */
  return retval;
}

static int pio_close(struct inode *inodep, struct file *filp)
{
  int retval = 0;
  pr_debug("pio_close\n");
  /* We can get private data from filp->private_data; */
  return retval;
}

static ssize_t pio_read(struct file *f, char __user *buf, size_t cnt,
			loff_t *off)
{
  int retval = -EFAULT;
  pr_debug("pio_read\n");
  return retval;
}

static ssize_t pio_write(struct file *filp, const char __user *user_buf,
			 size_t count, loff_t *off)
{
  int retval = -EFAULT;
  pr_debug("pio_write\n");
  /* We can get private data from filp->private_data; */
  return retval;
}

static struct file_operations fops =
  {
    .open = pio_open,
    .release = pio_close,
    .read = pio_read,
    .write = pio_write,
  };

static struct usb_endpoint_descriptor *set_endpoint(struct usb_endpoint_descriptor *endpoint, int endpoint_type, int endpoint_direction)
{
	if   (((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == endpoint_direction)
	      && ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == endpoint_type))
	{
	  pr_debug("interupt endpoint found!\n");
	  return endpoint;
	}
	return NULL;
}

static int pio_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
  struct usb_device *udev = interface_to_usbdev(interface);
  struct pio_driver *dev = NULL;
  struct usb_host_interface *iface_desc;
  struct usb_endpoint_descriptor *endpoint;
  int i;
  int endpoint_size;
  int retval = -ENODEV;

  pr_debug("pio_probe\n");

  if(!udev)
    {
      pr_debug("udev is null\n");
    }

  dev = kzalloc(sizeof(struct pio_driver), GFP_KERNEL);

  if(!dev)
    {
      pr_debug("Cannot allocate memory for struct pio_driver\n");
      retval = -ENOMEM;
      //goto exit;
      return retval;
    }

  dev->command = PIO_STOP;

  sema_init(&dev->sem,1);
  spin_lock_init(&dev->cmd_spinlock);
  dev->udev = udev;
  dev->interface = interface;
  iface_desc = interface->cur_altsetting;

  pr_debug("Number of endpoints found: %d\n",iface_desc->desc.bNumEndpoints);
  
  /*Set up interupt endpoints*/
  for(i = 0; i < iface_desc->desc.bNumEndpoints; ++i)
    {
      endpoint = &iface_desc->endpoint[i].desc;
      pr_debug("Found endpoint address %x\n",endpoint->bEndpointAddress);
      //DON'T USE THIS RUBBISH
      /*if (((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN)
	&& ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) ==
	USB_ENDPOINT_XFER_INT))
	{
	dev->int_in_endpoint = endpoint;
	}*/
      if(endpoint->bEndpointAddress == INTERUPT_EP_3_IN)
	{
	  pr_info("Setting interupt endpoint\n");
	  dev->int_in_endpoint = set_endpoint(endpoint, USB_ENDPOINT_XFER_INT, USB_DIR_IN);
	  endpoint_size = le16_to_cpu(dev->int_in_endpoint->wMaxPacketSize);
	  pr_debug("Interupt endpoint size is %d\n",endpoint_size);
	  //dev->int_in_buffer = initialise_urb_buffer(endpoint_size, &buffer_err);
	  dev->int_in_buffer = kmalloc(endpoint_size,GFP_KERNEL);
	  if(!dev->int_in_buffer)
	    {
	      pr_debug("Oh bugger it failed to kmalloc\n");
	    }
	  //dev->int_in_urb = initialise_urb(&urb_err);
	  dev->int_in_urb = usb_alloc_urb(0, GFP_KERNEL);
	  if(!dev->int_in_urb)
	    {
	      pr_debug("You fucking bastard, no urbs for you, throw the strawberries in the air!\n");
	    }
	}
    }

  if(!dev->int_in_endpoint)
    {
      pr_debug("Could not find endpoint interupt but if interupt is already done then please ignore\n");
      return retval;
    }

  /*Get endpoint size*/
  //THIS IS DANGEROUS YOU WILL LOCK UP YOUR KERNEL
  // endpoint_size = le16_to_cpu(dev->int_in_endpoint->wMaxPacketSize);
  // pr_debug("Endpoint size is %d\n",endpoint_size);

  dev->ctrl_buffer = kzalloc(PIO_CTRL_BUFFER_SIZE, GFP_KERNEL);
  if(!dev->ctrl_buffer)
    {
      pr_debug("Oh bugger the butter, ctrl_buffer failed to allocate\n");
      retval = -ENOMEM;
      return retval;
    }

  dev->ctrl_dr = kmalloc(sizeof(struct usb_ctrlrequest), GFP_KERNEL);
  if(!dev->ctrl_dr)
    {
      pr_debug("usb_ctrlrequest doesn't wanna work today goodbye\n");
      retval = -ENOMEM;
      return retval;
    }

  pr_debug("Does it get this far?\n");

  dev->ctrl_dr->bRequestType = PIO_CTRL_REQUEST_TYPE;
   pr_debug("REQUESTTYPEDoes it get this far?\n");
  dev->ctrl_dr->bRequest = PIO_CTRL_REQUEST;
   pr_debug("REQUESTDoes it get this far?\n");
  dev->ctrl_dr->wValue = cpu_to_le16(PIO_CTRL_VALUE);
   pr_debug("VALUEDoes it get this far?\n");
  dev->ctrl_dr->wIndex = cpu_to_le16(PIO_CTRL_INDEX);
   pr_debug("INDEXDoes it get this far?\n");
  dev->ctrl_dr->wLength = cpu_to_le16(PIO_CTRL_BUFFER_SIZE);
   pr_debug("LENGTHDoes it get this far?\n");

  usb_fill_control_urb(dev->int_in_urb, dev->udev,
		       usb_sndctrlpipe(dev->udev, 0),
		       (unsigned char *)dev->ctrl_dr,
		       dev->ctrl_buffer,
		       PIO_CTRL_BUFFER_SIZE,
		       pio_ctrl_callback,
		       dev);
   pr_debug("FILLCONTROLURBDoes it get this far?\n");
	

   if(!usb_string(udev,udev->descriptor.iSerialNumber, dev->serial_number,
		  sizeof(dev->serial_number)))
     {
       pr_debug("Could not retrieve serial number\n");
       return retval;
     }

   usb_set_intfdata(interface, dev);

   dev->minor = interface->minor;
  /* Set up our class */
  class.name = PIO_NODE"%d";
  class.fops = &fops;

  if ((retval = usb_register_dev(interface, &class)))
    {
      printk(KERN_INFO "usb register failed :(\n");
    }
  return retval;
}
/* This will be our private interface device data instead of NULL eventually */
/* usb_set_intfdata(interface, NULL); */
//return retval;
//}

static void pio_disconnect(struct usb_interface *interface)
{
  pr_debug("pio_disconnect\n");
  /* Give back our minor. */
  usb_deregister_dev(interface, &class);
}

static int __init pio_init(void)
{
  int result;
  pr_debug("pio_disconnect\n");

  /* Wire up our probe/disconnect */
  pio_driver.probe = pio_probe;
  pio_driver.disconnect = pio_disconnect;

  /* Register this driver with the USB subsystem */
  if ((result = usb_register(&pio_driver))) {
    pr_err("usb_register() failed. Error number %d", result);
  }
  return result;
}

static void __exit pio_exit(void)
{
  /* Deregister this driver with the USB subsystem */
  usb_deregister(&pio_driver);
}

module_init(pio_init);
module_exit(pio_exit);

MODULE_AUTHOR("The arse\n");
MODULE_LICENSE("GPL");
