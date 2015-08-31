/*************************************************************************
	> File Name: my_signal.c
	> Author: yunting
	> Mail: 1368282581@qq.com 
	> Created Time: 2015年08月31日 星期一 09时05分44秒
 ************************************************************************/

#include <stdio.h>
#include <signal.h>

/*信号处理函数*/
void handler_sigint(int signo)
{
	printf("recv SIGINT!\n");
}

int main()
{
	signal(SIGINT, handler_sigint);	//接受到SIGINT执行函数

	while(1);

	return 0;
}
