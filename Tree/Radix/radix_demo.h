/* *  radix_demo.h interface
*
*  Copyright(C)  2018
*  Contact: JeCortex@yahoo.com
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*  GNU General Public License for more details.
*/

#ifndef __RADIX_DEMO_H__
#define __RADIX_DEMO_H__

#include <linux/types.h>

struct _ioblk {
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
        atomic_t tsync_blks;
        atomic_t write_blks;
        atomic_t read_blks;
	sector_t size;
};

#endif
