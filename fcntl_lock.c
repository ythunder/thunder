#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

void my_err(const char *err_string, int line)				//标准错误输出函数
{
	fprintf(stderr, "line:%d ", line);
	perror(err_string);
	exit(1);
}

int lock_set(int fd, struct flock *lock)
{
	if(fcntl(fd, F_SETLK, lock) == 0) {   				 //设置锁
		if(lock->l_type == F_RDLCK) {    			//读锁
			printf("set read lock,pid:%d\n",getpid()); 	//getpid()取得进程识别码
		} else if(lock->l_type == F_WRLCK) {      		//写锁
			printf("set write lock,pid:%d\n",getpid());
		} else if(lock->l_type == F_UNLCK) {			//两种锁
			printf("release lock,pid:%d\n",getpid());
		}
	} else {
		perror("lock operation fail\n");   			//设置锁失败
		return -1;
	}
	return 0;
}

/*测试锁，只有当测试发现参数lock制定的锁能被设置时，返回0*/
int lock_test(int fd, struct flock *lock)
{
	if(fcntl(fd, F_GETLK, lock) == 0) {  				//使用F_GETLK,若锁能被设置，则置1_type为F_UNLCK
		if(lock->l_type == F_UNLCK) {				//测试1_type的值，
			printf("lock can be set in fd\n");		//设置成功
			return 0;
		} else {
			if(lock->l_type == F_RDLCK) {
				printf("can't set lock, read lock have been set by:%d\n", lock->l_pid); //lock->l_pid 锁所属进程ID
			}else if(lock->l_type == F_WRLCK) {
				printf("can't set lock, write lock have been set by:%d\n", lock->l_pid);
			}
			return -2;
		}
	}else {								//执行fcntl失败，返回-1
		printf("get incompatible locks fail");
		return -1;
	}

}

int main()
{
	int		fd;
	int 		ret;
	struct flock 	lock;
	char		read_buf[32];

	/*打开或者创建文件*/
	if((fd = open("example_65", O_CREAT|O_TRUNC|O_RDWR, S_IRWXU)) == -1) {   //打开文件
		my_err("open", __LINE__);
	}
	if(write(fd, "test lock", 10) != 10) {  				//写入数据
		my_err("write", __LINE__);	
	}
	/*初始化lock的结构*/
	memset(&lock, 0, sizeof(struct flock));					//清空结构体
	lock.l_whence = SEEK_SET;						//锁整个文件
	lock.l_start = 0;
	lock.l_len = 0;

	/*设置读锁*/
	lock.l_type = F_RDLCK;							//先为设置一个可写锁，再进行下一步
	if(lock_test(fd, &lock) == 0) {						//测试是否可以设置锁
		lock.l_type = F_RDLCK;				
		lock_set(fd, &lock);						//设置锁
	}

	/*读数据*/
	lseek(fd, 0, SEEK_SET);							//将文件指针值指向文件头
	if((ret = read(fd, read_buf, 10)) < 0) {				//从fd中读取10个字节数到read_buf
		my_err("read", __LINE__);
	}
	read_buf[ret] = '\0';							//添加结束符
	printf("%s\n", read_buf); 						//输出读取的10个字符

 	/*等待任意按键*/
	getchar();
	
	/*设置写锁*/
	lock.l_type = F_WRLCK;							//初始化中可能没锁，为其赋值，再进行下一步测试
	if(lock_test(fd, &lock) == 0) {
		lock.l_type = F_WRLCK;
		lock_set(fd, &lock);
	}

	/*释放锁*/
	lock.l_type = F_UNLCK;
	lock_set(fd,&lock);
	close(fd);
	return 0;
}
