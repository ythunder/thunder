#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>



int main(int argc, char **argv)
{
    int		       i;
    int		       serv_port;
    struct sockaddr_in serv_addr;
    char	       recv_buf[BUFSIZE];

    /*初始化服务器端地址结构*/
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;

    /*从命令行获取服务器端的端口和地址*/
    for(i=1; i<argc; i++) {				
	if(strcmp("-p", argv[i]) == 0) {
	    serv_port = atoi(argv[i+1]);				//取出服务端地址
	    if(serv_port <0 || serv_port > 65535) {
		printf("invalid serv addr.sin_port\n");
		exit(1);
	    } else {
		serv_addr.sin_port = htons(serv_port);
	    }
	    continue;
	}

    if(strcmp("-a", argv[i]) == 0) {
	if (inet_aton(argv[i+1], &serv_addr.sin_addr) == 0) {		//将-a 后的IP地址转换为二进制的网络字节形式,存入serv_addr.sin_add中
	    printf("invalid server ip address\n");
 	    exit(1);
	}
	continue;
    }

    if(serv_addr.sin_port == 0 || serv_addr.sin_addr.s_addr == 0) {	//判断,确保IP地址和端口号都存在
	printf("Usage:[-p] [serv_addr.sin_port] [-a] [serv_address]\n");
	exit(1);
    }

    /*创建一个TCP套接字*/
    conn_fd = socket(AF_INET, SOCK_STREAM, 0);	
    if(conn_fd == 0) {
	my_err("socket ", __LINE__);
    }

    /*向服务器发送连接请求*/
    if(connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
	my_err("connet ", __LINE__);
    }

    do {
        printf("申请或者注册");
        printf("1.申请 2.注册");
	choice = getchar();
    }while(choice != '1' && choice != '2');	//选择申请或者注册

    switch(choice) {
	case '1': input_apply(conn_fd, "账户:");
		  input_apply(conn_fd, "密码:");  
		if(ret = my_recv(conn_fd, recv_buf, sizeof(recv_buf)) < 0) {
		      my_err("recv", __LINE__);
		   }
		  
		 
		  

/****************************************
    buf:暂时用来存放数据的缓存
    len:buf长度
    实现从键盘获取账户存入buf中
****************************************/
void get_apply(char *buf, int len)	
{
    int i=0;
    char c;
		
    if(buf == NULL) {
	return -1;
    }
 
    c = getchar();
    while(c != '\n' && i < len-2) {
	buf[i] = c;
	i++;
	c = getchar();
    } 
    buf[i] == '\n';
    buf[i++] == '\0';

    return 0;
}
/****************************************************
    conn_fd:客户端创建的套接字
    string :有账户,密码两种形式,用于提示用户输入
    调用函数:get_apply:从键盘获取字符串存入input_buf
    实现将input_buf中的数据服务器
****************************************************/

void input_apply(int conn_fd, const char *string)
{
    char input_buf[32];
    char recv_buf[BUFSIZE];
    
    do {
	printf("%s", string);
	if(get_apply(input_buf, 32) < 0) {
	    printf("error from get_apply\n");
	    exit(1);
	}

	if(send(conn_fd, input_buf, strlen(input_buf), 0) < 0) {	//送粗去
	    my_err("send",__LINE__);
	}



