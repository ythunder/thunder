#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(void)
{
	pid_t pid;

	printf("process creation study\n");
	pid = fork();
	switch(pid) {
	case 0:
		printf("child process is running, curpid is %d, parentpid is %d\n",pid,getppid());
		break;
	case -1:
		printf("process creation failed\n");
		break;
	default:
		printf("parent process is runing, childpid is %d, parentpid is %d",pid, getpid());
		break;
	}
	exit(0);
}
