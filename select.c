#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    char buffer[128];
    int result, nread;

    fd_set inputs, testfds;
    struct timeval timeout;

    FD_ZERO(&inputs);
    FD_SET(0, &inputs);

    while(1) {
	testfds = inputs;
	timeout.tv_sec = 2;
	timeout.tv_usec = 500000;

	result = select(FD_SETSIZE, &testfds, (fd_set *)NULL, (fd_set *)NULL,//调用测试文件描述符集合中,是否有一个文件处于可读可写错误状态,											他将阻塞以等待某个文件描述符进入上述状态
	    &timeout);

	switch(result) {
	case 0:
	    printf("time out\n");
	    break;
	case -1:
	    perror("select ");
	    exit(1);
	default:
	    if(FD_ISSET(0, &testfds)) {		//判断描述符0是否testfds中,因为用完select后,必须判断该文件描述符是否被清空了
		ioctl(0, FIONREAD, &nread);
		if(nread == 0) {
		    printf("keyboard down\n");
		    exit(0);
		}
		nread = read(0, buffer, nread);
		buffer[nread] = 0;
		printf("read %d from keyboard :%s", nread, buffer);
	    }
	    break;
	}
    }
}

