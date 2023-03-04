#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION "Driver for marr_hildreth"
#define DEVICE_NAME "marr_hildreth"
#define DRIVER_NAME "marr_hildreth"



#define BRAM1 0x400
#define BRAM2 0x800
#define BRAM3 0x800

static int marr_hildreth_probe(struct platform_device *pdev);
static int marr_hildreth_open(struct inode *i, struct file *f);
static int marr_hildreth_close(struct inode *i, struct file *f);
static ssize_t marr_hildreth_read(struct file *f, char __user *buf, size_t len, loff_t *off);
static ssize_t marr_hildreth_write(struct file *f, const char __user *buf, size_t count, loff_t *off);
static int _init marr_hildreth_init(void);
static void _exit marr_hildreth_exit(void);
static int marr_hildreth_remove(struct platform_device *pdev);



static struct file_operations my_fops = 
{
	.owner = THIS_MODULE,
	.open = marr_hildreth_open,
	.release = marr_hildreth_close,
	.read = marr_hildreth_read,
	.write = marr_hildreth_write
};

static struct of_device_id device_of_match[] = {
	//u comatibile poljima su imena svih uredjaja za koje je namenjen ovaj driver
	{.compatibile = "xlnx, marr_hlidreth",},
	{.compatibile = "xlnx,---------------- "} 
};

static struct platform_driver my_driver = {
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
		.of_match_table = device_of_match,
		},
	.probe = marr_hildreth_probe, //sluzi za inicijalizaciju uredjaja i alociranje potrebnih resursa
	.remove = marr_hildreth_remove //sluzi za zaustavljanje rada uredjaja i oslobadjanje zauzetih resursa
};
static dev_t my_dev_id;
static struct class *my_class;
static struct cdev *mycdev;

struct device_info {
	unsigned long mem_start,
	unsigned long mem_end,
	void _iomem *base_addr;
};
static struct device_info *di = NULL;
//*********************************************************
// PROBE AND REMOVE
//*********************************************************
static int marr_hildreth_probe(struct platform_device *pdev)
{
	struct resource *r_mem;
	int rc = 0;
	printk (KERN_INFO "Probing\n");
	r_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	//Ovaj poziv vraca strukturu koja ce kao polja imati podatke o adresnom prostoru uredjaja.
	
	if (!r_mem) {
		printk (KERN_INFO "Invalid address\n");
		return -ENODEV;
	} // proverava se da li su se dobili validni podaci.
	
	marr_hlidreth = (struct device_info *) kmalloc(sizeof(struct device_info), GFP_KERNEL);// zauzima se prostor za prethodno deklarisanu pomocnu strukturu 
	if(!marr_hlidreth)
	{
		printk(KERN_INFO "Cound not allocate marr_hildreth device \n"); 
		return -ENOMEM
	};
	marr_hildreth -> mem_start = r_mem ->start;
	marr_hildreth -> mem_end = r_mem->end;
/
	//Zauzimanje mmemorije
	if (!request_mem_region(marr_hildreth -> mem_start, marr_hildreth -> mem_end - marr_hildreth -> mem_start + 1, DRIVER_NAME))
	{
		printk(KERN_ALERT "Couldn't lock memory region at %p\n", (void*)mar_hildreth->mem_start);
		rc = -EBUSY;
		goto error1;
	}
	//Ukoliko smo zauzeli memoriju potrebno je fizicke adrese mapirati u virtuelne
	
	marr_hildreth ->base_addr = ioremap(marr_hildreth->mem_start, marr_hildreth->mem_end - marr_hildreth->mem_start + 1);

	if(!marr_hildreth ->base_addr)
	{
	printk(KERN_ALERT "[PROBE]: Could not allocated marr_hildreth iomem\n");
	rc = -EIO;
	goto error2;
	}
	return 0;
	
	error3: 
		iounmap(marr_hildreth->base_addr);
	error2:
	release_mem_region(marr_hildreth->mem_start, marr_hildreth->mem_end - marr_hildreth->mem_start + 1);
	error1:
	return rc;
};

//U remove funkciji se oslobadjaju svi resursi zauzeti u probe funkciji
static int marr_hildreth_remove(struct platform_device *pdev)
{
	printk(KERN_ALERT " Marr platform driver removed\n");
	iowrite32(0, marr_hildreth->base_addr);
	iounmap(marr_hildreth -> base_addr);
	release_mem_region(marr_hildreth -> mem_start, (marr_hildret-> mem_end- marr_hildreth->mem_srart + 1 ));
	kfree(marr_hildreth);
	return 0;
}

//*********************************************************
// OPEN AND CLOSE
//*********************************************************

static int marr_hildreth_open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Succesfully opened file\n");
	return 0;
}
static int marr_hildreth_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Succesfully closed file\n");
	return 0;
}

//*********************************************************
// READ AND WRITE
//*********************************************************
unsigned int width = 0; 
unsigned int height = 0;
unsigned int l1 = 0;
unsigned int border1 = 0;
unsigned int start1 = 0;
unsigned int start2 = 0;

#define BUFF_SIZE 800
int end_read = 0;
int i = 0;
int j = 0;

ssize_t marr_hildreth_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	char buff[BUFF_SIZE];
	long int len = 0;
	
	int val;
	if (end_read == 1)
	{
		end_read = 0;
		return 0;
	}

	copy_to_user(buffer, buf, len);




};

ssize_t led_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) 
{
	char buff[BUFF_SIZE];
	int ret = 0;
	int val1 = 0;
	int val2 = 0;

	copy_from_user(buff, buffer, length);

	sscanf(buf, "%u, %u, %u, %u", &width, &height, &border1, &l1);
	printk(KERN_INFO "[WRITE] %u, %u, %u, %u \n", width, height, border1 ,l1);
	iowrite32(width, di->base_addr + 0);
	iowrite32(height, di->base_addr + 4);
	iowrite32(border1, di->base_addr + 8);
	iowrite32(l1, di->base_addr + 12);

	//iowrite32(start1, di->base_addr + 16);
	//iowrite32(start2, di->base_addr + 20);
	
	//Write in BRAM1 and BRAM2

	sscanf(buf, "%d", &val1);
	//trebalo bi da upise sliku
	for (i = 0; i < 256; i++)
	{
		iowrite32(val, di->base_addr + BRAM1 + i);
	}
	sscanf(buf, "%d", &val2);
	//trebalo bi da upise masku
	for (i = 0; i < 25; i++)
	{
		iowrite32(val, di->base_addr + BRAM2 + i);
	}
	//start laplas
	iowrite32(1, di->base_addr + 16);
	udelay(1000);
	iowrite32(0, di->base_addr + 16);

	while(!ioread32(di->base_addr+24))
		udelay(1000);
	//start zero crossing
	iowrite32(1, di->base_addr + 20);
	udelay(1000);
	iowrite32(0, di->base_addr + 20);
	while(!ioread32(di->base_addr+24))
		udelay(1000);
}


























