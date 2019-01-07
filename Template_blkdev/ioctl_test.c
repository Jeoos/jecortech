/* *  ioctl_us.c interface
*
*  Copyright(C)  2018
*  Contact: JeCortex@yahoo.com
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*  GNU General Public License for more details.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "ioctl_us.h"

//Simple test, find out the device MAJOR manual.
#define SIMP_BLKDEV_DEVICEMAJOR 72

int main ()
{
        int fd;
        fd = open("/dev/simp_blkdev", O_RDONLY);

        ioctl(fd, TEST_01, NULL);
        ioctl(fd, TEST_02, NULL);
        close(fd);
        return 0;

}
