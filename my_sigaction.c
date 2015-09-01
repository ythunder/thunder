/*************************************************************************
	> File Name: my_sigaction.c
	> Author: yunting
	> Mail: 1368282581@qq.com 
	> Created Time: 2015年09月01日 星期二 10时51分47秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int temp = 0;

void handler_sigint(int signo)
{
	printf("recv SIGINT\n");
	sleep(5);
	temp += 1;
	printf("the value of temp is : %d \n", temp);
	printf("in handler_sigint, after sleep\n");
}


int main()
{
	struct sigaction  act;

	act.sa_handler = handler_sigint;
	//act.sa_flags = SA_NOMASK;	//在处理此信号之前允许此信号再次递送

	sigaction(SIGINT, &act, NULL);

	while(1)
		;

	return 0;

}
