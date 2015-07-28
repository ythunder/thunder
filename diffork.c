#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int globVar = 5;

int main()
{
	pid_t	pid;
	int	var = 1, i;

	printf("fork is diffirent with vfork\n");

	//pid = fork();		//创建子进程,子进程完全复制父进程资源,独立于父进程,良好的并发性
	pid = vfork();	//同上调用一次,两个返回值,与父进程共享地址空间,子进程的任何修改为父进程所见,保证所有子进程先运行
	switch(pid) {
		case 0:
	   	  i = 3;
		  while(i-- > 0) {
			printf("child process is running\n");
			globVar++;    	//测试子进程的此值的改变会不会影响父进程
			var++;
			sleep(1);
		}
		printf("child's globVar = %d, var = %d\n", globVar, var);
		break;
	
		case -1:
		  perror("process creation failed\n");
		  exit(0);
		
		default:
		  i = 5;
			while(i-- > 0) {
			     printf("parent process is running\n");
			     globVar++;
			     var++;
			     sleep(1);
			}
			printf("parent's globVar = %d, var = %d\n",globVar, var);
			exit(0);
		}
}
