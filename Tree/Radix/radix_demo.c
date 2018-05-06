#include <linux/init.h>
#include <linux/module.h>
#include <linux/blkdev.h>
#include <linux/slab.h>
#include <linux/radix-tree.h>
#include <linux/bio.h>
#include <linux/kernel.h>

#define SECTOR_SHIFT    9
#define PAGE_SECTORS_SHIFT  (PAGE_SHIFT - SECTOR_SHIFT)                                                                          
#define PAGE_SECTORS        (1 << PAGE_SECTORS_SHIFT)

#define BLKS_VEC_SIZE 16


#define SIMP_BLKDEV_DISKNAME "simp_blkdev"
#define SIMP_BLKDEV_DEVICEMAJOR COMPAQ_SMART2_MAJOR
#define SIMP_BLKDEV_BYTES       (1*1024*1024*1024)

#define RADIX_TREE_TAG_DIRTY	0

static struct gendisk *simp_blkdev_disk;
static struct request_queue *simp_blkdev_queue;

struct kmem_cache *cache_blk;

struct ioblk {
	struct list_head list;
	struct bio *bio;

	unsigned long private;
	struct page *page;
	int len;
	int offset;
	sector_t sector;

	unsigned long flag;
	pgoff_t	index;	

	struct work_struct work;

	wait_queue_head_t wq;
	u8          mapped;
	u8          uptodate;
	u8          dirty;
	uint64_t bi_sector;

        struct dummy_device *ddev;
};

struct dummy_device {
        spinlock_t  dev_lock;
        struct block_device *bdev;

        rwlock_t tree_lock;
        struct radix_tree_root  dev_pages;

        atomic_t dirty_blks;
        atomic_t write_blks;
        atomic_t read_blks;
	sector_t size;
};

struct dummy_device *blk_dev;

static int simp_open(struct block_device *bdev, fmode_t mode)
{
        printk("In open ...\n");
        return 0;
}

static void simp_release(struct gendisk *disk, fmode_t mode)
{
        printk("In release ...\n");
        return;
}

static const struct block_device_operations simp_blkdev_fops = {
        .owner = THIS_MODULE,
        .open = simp_open,
        .release = simp_release,
};

static void io_end_bio(struct bio *bio, int err)
{
	//struct dummy_device *ddev = bio->bi_private;
        printk(KERN_INFO "in end bio\n");

	if (unlikely(!bio_flagged(bio,BIO_UPTODATE))) {
		if (err == 0) {
			printk(KERN_ERR "%s-%d: Not uptodata bio try again !%ld \n",
				 __func__, __LINE__, bio->bi_sector);
			return ;
		}
	}
	//__free_pages(page, 0);
	bio_put(bio);
}

int submit_readwrite(int rw,struct dummy_device *ddev, struct page *page, sector_t sector)
{
	struct bio *bio;
	bio = bio_kmalloc(GFP_KERNEL, 1);
	if (!bio) {
		printk(KERN_ERR "%s-%d: Cannot alloc bio\n", __func__, __LINE__);
		return -ENOMEM;
	}
	/* init bio */
	bio->bi_bdev = ddev->bdev;
        printk("bio->bi_sector = %lu\n", sector);
	bio->bi_private = ddev;

	bio->bi_rw |= REQ_FAILFAST_DEV |
		REQ_FAILFAST_TRANSPORT |
		REQ_FAILFAST_DRIVER;

	bio_add_page(bio, page, PAGE_SIZE, 0);

	bio->bi_end_io = io_end_bio;

	submit_bio(rw, bio);

	return 0;
}

struct ioblk *dev_lookup_blk(struct dummy_device *dev, sector_t sector)
{
        pgoff_t idx = sector >> PAGE_SECTORS_SHIFT;
        struct ioblk *blk = NULL;

        read_lock(&dev->tree_lock);
        blk = radix_tree_lookup(&dev->dev_pages, idx);
        read_unlock(&dev->tree_lock);
        return blk;
}

struct ioblk* dev_insert_page(struct dummy_device *ddev, sector_t sector)
{
        struct ioblk *blk = NULL;
        pgoff_t idx;
        blk = dev_lookup_blk(ddev, sector);
        if(blk)
                return blk;

        blk = kmem_cache_zalloc(cache_blk, 
                                               GFP_KERNEL);
        if (!blk){
                goto err_alloc;
        }
        blk->page = alloc_page(GFP_KERNEL);
        if (!blk->page) {
                kmem_cache_free(cache_blk, blk);
                goto err_alloc;
        }
        idx = sector >> PAGE_SECTORS_SHIFT;
        blk->index = idx;
        
        if(radix_tree_preload(GFP_NOIO))
                return NULL;

        write_lock_irq(&ddev->tree_lock);
        if (radix_tree_insert(&ddev->dev_pages, idx, blk)){
                printk("err insert.\n");
        }
        write_unlock_irq(&ddev->tree_lock);

	radix_tree_preload_end();
	blk->ddev = ddev;

        return blk;

err_alloc:
        printk("err alloc ioblk.\n");
        return NULL;
}

/* if already dirty, return 0, else dirty it and return 1 */
int set_blk_dirty(struct ioblk *blk)
{
	struct dummy_device *ddev = blk->ddev;

	atomic_inc(&ddev->dirty_blks);		

	write_lock_irq(&ddev->tree_lock);
	radix_tree_tag_set(&ddev->dev_pages, blk->index, RADIX_TREE_TAG_DIRTY);
	write_unlock_irq(&ddev->tree_lock);

	return 1;
}


static int blk_dev_write(struct dummy_device *ddev, struct page *page, unsigned int len, 
                unsigned int off, sector_t sector, uint64_t bi_sector, uint32_t bi_size)
{
        int nr,i, ret;
	unsigned int offset = (sector & (PAGE_SECTORS -1)) << SECTOR_SHIFT;
	unsigned int copy = min_t(unsigned int, len, PAGE_SIZE - offset);
        char sectors;
        struct ioblk *blk;
        void *dst = NULL, *src = NULL;
        src = page_address(page)+off;
        nr = (len>(PAGE_SIZE-offset))?2:1;

	for (i=0; i<nr; i++) {
                blk = dev_insert_page(ddev, sector);
                if (!blk){
                        ret = -ENOMEM;
                        goto err_get;
                }
                sectors = copy >> SECTOR_SHIFT;
                src += i*(len-copy);
                dst = page_address(page)+offset;
                memcpy(dst, src, copy);
                set_blk_dirty(blk);
                atomic_inc(&blk->ddev->write_blks);

                //submit_readwrite(WRITE, ddev, page, sector);

		sector += sectors;
		copy = len-copy;
		offset = 0;
        }
        return 0;
err_get:
        printk("in[%s]: err get blk.\n", __func__);
        return ret;
}
static int blk_dev_read(struct dummy_device *ddev, struct page *page, unsigned int len, 
                        unsigned int off, sector_t sector)
{
	int nr, i, ret;
	unsigned int offset = (sector & (PAGE_SECTORS -1)) << SECTOR_SHIFT;
	unsigned int copy = min_t(unsigned int, len, PAGE_SIZE - offset);
	char sectors;
        struct ioblk *blk;
	void *dst = NULL, *src = NULL;
	dst = page_address(page)+off;
	nr = (len>(PAGE_SIZE-offset))?2:1;

        for(i=0;i<nr;i++){
                blk = dev_insert_page(ddev, sector);
                if (!blk){
                        ret = -ENOMEM;
                        goto err_get;
                }
                sectors = copy >>SECTOR_SHIFT;          
		dst += i*(len-copy);

	        src = page_address(page)+offset;
                //submit_readwrite(READ, ddev, page, sector);

	        memcpy(dst, src, copy);
                atomic_inc(&blk->ddev->read_blks);
		sector += sectors;
		copy = len-copy;
		offset = 0;
        }
        return 0;
err_get:
        return ret;
}

static void simp_make_request(struct request_queue *q, struct bio *bi)
{
        int rw, i, err;
        struct bio_vec *bvec;
        sector_t sector = bi->bi_sector;
        //static int incnt = 0;
        rw = bio_rw(bi);

        //printk("incnt = %u \n", incnt++);
        bio_for_each_segment(bvec, bi, i) {
                if(rw == WRITE){
                        err = blk_dev_write(blk_dev, bvec->bv_page, bvec->bv_len,
                                               bvec->bv_offset, sector,
                                               bi->bi_sector, bi->bi_size);
                }else{
                        err = blk_dev_read(blk_dev, bvec->bv_page, bvec->bv_len,
                                              bvec->bv_offset, sector);
                }
                if (err) break;
                sector += bvec->bv_len >> SECTOR_SHIFT;
        }
        bio_endio(bi, 0);

        return ;
}

static int __init radix_block_init(void)
{
        int ret;
        printk("in init.\n");
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
        blk_queue_flush(simp_blkdev_disk->queue, REQ_FLUSH | REQ_FUA);
        
        add_disk(simp_blkdev_disk);
	cache_blk = kmem_cache_create("ioblk", sizeof(struct ioblk),
			0, 0, NULL);
	if (!cache_blk) {
		ret = -ENOMEM;
		goto err_kmem_cache;
	}
        blk_dev = (struct dummy_device *)kzalloc(sizeof(*blk_dev), GFP_KERNEL);
        if(!blk_dev){
                pr_err("Cannot alloc dev.\n");
                goto err_blk_dev;
        }
        spin_lock_init(&blk_dev->dev_lock);
        INIT_RADIX_TREE(&blk_dev->dev_pages, GFP_ATOMIC);
        rwlock_init(&blk_dev->tree_lock);
        blk_dev->size = SIMP_BLKDEV_BYTES >> 9;

        atomic_set(&blk_dev->dirty_blks, 0);
        atomic_set(&blk_dev->write_blks, 0);
        atomic_set(&blk_dev->read_blks, 0);

        return 0;

err_blk_dev:
        kfree(blk_dev);
err_kmem_cache:
        del_gendisk(simp_blkdev_disk);
        put_disk(simp_blkdev_disk);
err_alloc_disk:
        blk_cleanup_queue(simp_blkdev_queue);

err_alloc_queue:
        return ret;
}

unsigned find_get_blks_tag(struct dummy_device *ddev, pgoff_t *index, 
		int tag, unsigned int nr_pages, struct ioblk **blks)
{
	struct radix_tree_iter iter;
	void **slot;
	struct ioblk *blk;
	unsigned ret = 0;

	//rcu_read_lock();
	read_lock(&ddev->tree_lock);
restart:
	radix_tree_for_each_tagged(slot, &ddev->dev_pages, &iter, *index, tag) {
repeat:
		blk = radix_tree_deref_slot(slot);
		if (unlikely(!blk))
			continue;

		if (radix_tree_exception(blk)) {
			if (radix_tree_deref_retry(blk)) {
				goto restart;
			}
			continue;
		}
		if (unlikely(blk != *slot)) {
			goto repeat;
		}

		blks[ret] = blk;
		if (++ret == nr_pages)
			break;
	}
	//rcu_read_unlock();
	read_unlock(&ddev->tree_lock);

	if (ret)
		*index = blks[ret -1]->index + 1;

	return ret;
}

void find_dirty_blks(struct dummy_device *ddev)
{
	struct ioblk *blks[BLKS_VEC_SIZE];
	unsigned int i, nr_gets;
	pgoff_t index=0, end;
        int cnt = 0;
	end = ddev->size >> PAGE_SECTORS_SHIFT;

	while (index <= end && (nr_gets = find_get_blks_tag(ddev, 
		&index, RADIX_TREE_TAG_DIRTY, BLKS_VEC_SIZE, blks)) > 0) {
		for (i = 0; i < nr_gets; i++) {
			struct ioblk *blk = blks[i];

			if (blk->index > end)
				continue;
		}
                cnt += nr_gets;
	}
        printk("find dirty blks: %u\n", cnt);
        printk("dirty blks cnt = %u\n", atomic_read(&ddev->dirty_blks));
        printk("dirty write cnt = %u\n", atomic_read(&ddev->write_blks));
        printk("dirty read cnt = %u\n", atomic_read(&ddev->read_blks));
}

#define FREE_BATCH  16
void free_all_blks(struct dummy_device *ddev)
{
	unsigned long pos = 0;
	struct ioblk *blks[FREE_BATCH];
	int nr_pages = 0;
	unsigned long flags;
        int blk_cnt=0;

	write_lock_irqsave(&ddev->tree_lock, flags);
	while (1) {
		int i;
		nr_pages = radix_tree_gang_lookup(&ddev->dev_pages,
				(void **)blks, pos, FREE_BATCH);
		if (!nr_pages)
			break;
		for (i = 0; i < nr_pages; i++) {
			void *ret;
			BUG_ON(blks[i]->index < pos);
			pos = blks[i]->index;
			ret = radix_tree_delete(&ddev->dev_pages, pos);
			//BUG_ON(!ret || ret != blks[i]);
			if (!ret || ret != blks[i]) {
				pr_err("[%s]-[%d], pos:\n", 
						__func__, __LINE__);
			}
	                __free_pages(blks[i]->page, 0);
                        kmem_cache_free(cache_blk, blks[i]);
		}
		pos++;
                blk_cnt += nr_pages;
	};
        printk("blk_cnt = %u\n", blk_cnt);
	write_unlock_irqrestore(&ddev->tree_lock, flags);
}
        
static void __exit radix_block_exit(void)
{
        printk("In exit\n");
        find_dirty_blks(blk_dev);
        free_all_blks(blk_dev);
	kmem_cache_destroy(cache_blk);
        kfree(blk_dev);
        del_gendisk(simp_blkdev_disk);
        put_disk(simp_blkdev_disk);
        blk_cleanup_queue(simp_blkdev_queue);
        return;
}

module_init(radix_block_init);
module_exit(radix_block_exit);

MODULE_LICENSE("GPL");
