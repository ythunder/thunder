#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[], char **environ)
{
	pid_t	pid;
	int	stat_val;
	printf("exec example!\n");
	pid = fork();							//创建新的子进程
	switch(pid) {
		case -1:
		  perror("process creation failed!");
		  exit(1);
		case 0:
		  printf("child process is running\n");
		  printf("my pid = %d, gid = %d\n", getpid(), getppid());//获得此进程ID, 获得父进程ID
		  execve("processimage", argv, environ);		//在编译运行此程序前先编译另一个文件,当遇到execve()时运行另一个程序
		  printf("process never go to here!\n");		//因为上一步取运行另一个程序了,所以执行时永远到不了这一步
		  exit(0);
		default:
		  printf("parent process is running\n");
		  break;
	}
	
	wait(&stat_val);
	exit(0);
}


