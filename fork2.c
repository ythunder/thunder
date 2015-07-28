#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
	pid_t	pid;
	char*	msg;
	int 	k;

	printf("process creation study\n");
	pid = fork();				//创建一个进程,返回两个值
	switch(pid) {
		case 0:				//子进程返回0
			msg = "child process is running";	
			k = 3;
			break;
		case -1:			//创建出错返回-1
			perror("process creation failed\n");
			break;
		default:			//父进程返回值为子进程ID
			msg = "parent process is running";
			k = 5;
			break;
		}

		while(k > 0)
		{
			puts(msg);
			sleep(1);		//页面停顿一秒
			k--;
		}
		exit(0);
}
