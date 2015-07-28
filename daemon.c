#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <time.h>
#include <syslog.h>
#include <stdlib.h>

int init_daemon(void)
{
	int	pid;
	int	i;

	/*忽略终端I/O信号,stop信号*/
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	signal(SIGHUP,SIG_IGN);

	pid = fork();
	if(pid > 0) {
	   exit(0);		//结束父进程,使得子进程成为后台进程
	} else if(pid < 0) {
          return -1;
	}

	setsid(); //建立一个新的进程组,调用他的子进程成为新的会话的领头进程,并与父进程的会话组和进程组脱离

	pid = fork();   //新建子进程,
	if(pid > 0) {		//退出父进程,保证该进程不是进程组长,同时无法打开一个新的终端
		exit(0);
	}
	else if(pid < 0) {
		return -1;
	}

	for(i=0; i< NOFILE;close(i++));  	//关闭从父进程继承的不再需要的文件描述符
	
	chdir("/");	//改变工作目录至当前根目录,使得进程不与任何文件系统联系
	
	umask(0);	//文件屏蔽字设为0

	signal(SIGCHLD,SIG_IGN);	//忽略SIGCHLD信号

	return 0;
}

int main()
{
	time_t	 now;
	init_daemon();
	syslog(LOG_USER|LOG_INFO,"测试守护进程!\n");
	while(1) {
		sleep(8);
		time(&now);
		syslog(LOG_USER|LOG_INFO,"系统时间: \t%s\t\t\n",ctime(&now));
	}
}
		
