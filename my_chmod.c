#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct stat buf;
	
	if(argc != 2) {						//检查参数个数
		printf("Usage: my_stat <filename>\n");
		exit(1);
	}

	if(stat(argv[1], &buf) == -1) {				//获取文件属性
		perror("stat");
		exit(1);	
	}

	printf("device is: %d\n", (int)buf.st_dev);
	printf("inode is: %d\n", (int)buf.st_ino);
	printf("mode is: %d\n", (int)buf.st_mode);
	printf("number of hard links is: %d\n",(int)buf.st_nlink);
	printf("user ID of owner is: %d\n", (int)buf.st_uid);
	printf("group ID of owner is:%d\n", (int)buf.st_gid);
	printf("device type (if inode device) is: %d\n",(int)buf.st_rdev);

	printf("total size, in is: %d\n",(int)buf.st_size);
	printf("blocksize for filesystem I/O is: %d\n", (int)buf.st_blksize);
	printf("number of blocks allocated is: %d\n", (int)buf.st_blocks);

	printf("time of last access is: %s\n", ctime(&buf.st_atime));
	printf("time of last modification is: %s\n",ctime(&buf.st_mtime));
	printf("time of last change is: %s\n",ctime(&buf.st_ctime));

	return 0;
}
