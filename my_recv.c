#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

void my_err(const char *err_string, int line)
{
    fprintf(stderr,"line:%d ", line);
    perror(err_string);
    exit(1);
}

/*****************************************
    conn_fd:从该连接套接字上获取数据
    data_buf:读取到数据保存到此缓存
    len :data所指的空间长度
    实现从套接字上读取一次数据
*****************************************/
int my_recv(int conn_fd, char *data_buf, int len)
{
    static char recv_buf[1024];   //自定义缓存区
    static char *pread;		  //指向下一次读取数据的位置
    static int len_remain = 0;    //自定义缓存区中剩余字节数
    int i;

    //如果自定义缓存区中没有数据,则从套接字中读取数据
    if(len_remain <= 0) {
	if( (len_remain = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0) {
	    my_err("recv", __LINE__);
	} else if(len_remain == 0) {
	    return 0;
	}
	pread = recv_buf;   //重新初始化pread指针
    }
    //从自定义缓存取中读取一次数据
    for(i=0; *pread != '\n'; i++) {
	if(i > len) {
	    return -1;
	}
	data_buf[i] = *pread++;
	len_remain--;
    }
    len_remain--;
    pread++;

    return i;		//读取成功
}

