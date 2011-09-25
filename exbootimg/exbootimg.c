/* exbootimg.c
** Daniel DeFreez - September 2011
*/


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "bootimg.h"

#define IMGFILE "whispercore-0.5.5-boot.img"

// Adapted from write_padding in mkbootimg.c
int skip_padding(int fd, unsigned pagesize, unsigned itemsize)
{
	unsigned pagemask = pagesize - 1;
	unsigned count;

	if ((itemsize & pagemask) == 0) {
		return 0;
	}		

	count = pagesize - (itemsize & pagemask);

	return lseek(fd, count, SEEK_CUR);
}

int main(int argc, char **argv)
{
	int fd;			// file descriptor
	boot_img_hdr hdr;	// boot.img header
	void *kernel;	
	void *ramdisk;
	void *second;	

	fd = open(IMGFILE, O_RDONLY);

	// Read the contents of boot image header
	read(fd, &hdr, sizeof(hdr));
	skip_padding(fd, 2048, sizeof(hdr));

	kernel = malloc(hdr.kernel_size);
	read(fd, kernel, hdr.kernel_size);
	skip_padding(fd, 2048, hdr.kernel_size);	

	ramdisk = malloc(hdr.ramdisk_size);
	read(fd, ramdisk, hdr.ramdisk_size);
	skip_padding(fd, 2048, hdr.ramdisk_size);	

	close(fd);	

	fd = open("kernel", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	write(fd, kernel, hdr.kernel_size);	
	close(fd);	

	fd = open("ramdisk", O_CREAT | O_TRUNC | O_WRONLY, 0644);
	write(fd, ramdisk, hdr.ramdisk_size);	
	close(fd);	

	return 0;
}
