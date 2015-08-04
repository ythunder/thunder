#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	pid_t pid;

	printf("process creation study\n");
	pid = fork();					//fork()函数创建一个进程,返回俩次值赋给Pid
	switch(pid) {
	case 0:
		printf("child process is running, curpid is %d, parentpid is %d\n",pid,getppid());
		/*子进程中fork函数返回0, 通过getppid(id)函数获得进程父进程ID*/
		break;
	case -1:
		printf("process creation failed\n");
		/*进程创建失败,返回-1*/
		break;
	default:
		printf("parent process is runing, childpid is %d, parentpid is %d\n",pid, getpid());
		/*父进程调用fork的返回值,返回0,再通过getpid()获得进程id*/
		break;
	}
	exit(0);				//在头文件<stdlib.h>中
}
