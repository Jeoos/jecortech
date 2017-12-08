#include <linux/init.h>
#include <linux/module.h>
#include <linux/blkdev.h>
#include <linux/bio.h>

#define SIMP_BLKDEV_DISKNAME "simp_blkdev"
#define SIMP_BLKDEV_DEVICEMAJOR COMPAQ_SMART2_MAJOR
#define SIMP_BLKDEV_BYTES       (1*1024*1024*1024)

static struct gendisk *simp_blkdev_disk;
static struct request_queue *simp_blkdev_queue;

struct block_device_operations simp_blkdev_fops = {
        .owner = THIS_MODULE,
};

static blk_qc_t simp_make_request(struct request_queue *q, struct bio *bio)
{
        printk("In ...\n");
        blk_queue_split(q, &bio, NULL);

        return BLK_QC_T_NONE;
}

static int __init radix_block_init(void)
{
        int ret;
        simp_blkdev_queue = blk_alloc_queue(GFP_KERNEL);
        if(!simp_blkdev_queue)
                return -ENOMEM;
        blk_queue_make_request(simp_blkdev_queue, simp_make_request);
        if(!simp_blkdev_queue){
                ret = -ENOMEM;
                goto err_alloc_queue;
        }
        simp_blkdev_disk = alloc_disk(1);
        if(!simp_blkdev_disk){
                ret = -ENOMEM;
                goto err_alloc_disk;
        }
        strcpy(simp_blkdev_disk->disk_name, SIMP_BLKDEV_DISKNAME);
        simp_blkdev_disk->major = SIMP_BLKDEV_DEVICEMAJOR;
        simp_blkdev_disk->first_minor = 0;
        simp_blkdev_disk->fops = &simp_blkdev_fops;
        simp_blkdev_disk->queue = simp_blkdev_queue;
        set_capacity(simp_blkdev_disk, SIMP_BLKDEV_BYTES >> 9);
        
        add_disk(simp_blkdev_disk);
        return 0;

err_alloc_disk:
        blk_cleanup_queue(simp_blkdev_queue);

err_alloc_queue:
        return ret;
}

static void __exit radix_block_exit(void)
{
        del_gendisk(simp_blkdev_disk);
        printk("...1...\n");
        put_disk(simp_blkdev_disk);
        printk("...2...\n");
        blk_cleanup_queue(simp_blkdev_queue);
        return;
}

module_init(radix_block_init);
module_exit(radix_block_exit);

MODULE_LICENSE("GPL");
