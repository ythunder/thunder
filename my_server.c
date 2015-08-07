#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include "my_recv.h"		//自定义头文件


#define  SERV_PORT		4507	//服务器端的端口
#define  LISTENQ		12	//连接请求队列的最大长度

#define  INVALID_USERINFO	'n'	//用户信息无效
#define  VALID_USERINFO		'y'	//用户信息有效

#define  USERNAME		0	//接到的是用户名
#define  PASSWORD		1	//接到的是密码

struct userinfo {			//保存用户名和密码的结构体
	char username[32];
	char password[32];
};

struct userinfo users[] = {		//结构体里存有的账户密码信息
	{"linux", "unix"},
	{"4507", "4508"},
	{"clh", "clh"},
	{"xl", "xl"},
	{" ", " "}			//以只含一个空格的字符串作为数组的结束标志
};


//查找用户名是否存在, 存在则返回该用户名的下标, 不存在则返回 -1, 出错则返回-2
int find_name(const char *name)
{
    int i;

    if (name == NULL) {
	printf("in find_name, NULL pointer");
	return -2;
     }
    for(i=0; users[i].username[0] != ' '; i++) {
	if(strcmp(users[i].username, name) == 0) {
	    return i;
	}
    }
    return -1;
}

//发送数据
void send_data(int conn_fd, const char *string)
{
    if(send(conn_fd, string, strlen(string), 0) < 0) {
        my_err("send", __LINE__);
    }
}

int main()
{
    int				sock_fd, conn_fd;	//sock_fd为新创建的套接字, conn_fd为套接字描述符
    int				optval;			//
    int				flag_recv = USERNAME; 	//标识接收到的是用户名还是密码
    int				ret;
    int				name_num;
    pid_t			pid;
    socklen_t			cli_len;
    struct sockaddr_in		cli_addr, serv_addr;
    char			recv_buf[128];

    /*创建一个TCR套接字*/
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0) {
	my_err("sock", __LINE__);
    }

    /*设置该套接字使之可以重新绑定端口*/
    optval = 1;
    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int)) < 0) {
        my_err("setsockopt", __LINE__);
    }

    /*初始化服务器端地址结构*/
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));		//将结构体中的信息置为0
    serv_addr.sin_family = AF_INET;				//AF_INET 使用IPv4 TCP/IP协议
    serv_addr.sin_port = htons(SERV_PORT);			//htons用于将主机的unsigned short型数据转换成网络字节顺序
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);		//htonl用于将主机的unsigned int型数据转换成网络字节顺序

    /*将套接字绑定到本地端口*/
    if(bind(sock_fd, (struct sockaddr *)&serv_addr,		//sock_fd为之前创建的套接字,serv_addr为套接字要绑定的地址
        sizeof(struct sockaddr_in)) < 0) {			//
        my_err("bind", __LINE__);
    }
  
    /*将套接字转换为监听套接字*/
    if(listen(sock_fd, LISTENQ) < 0) {				//listen将该套接字转换为监听套接字,等待来自客户端的连接请求
        my_err("lsten", __LINE__);  				//服务器建立链接队列,记录建立的连接,LISTENQ指定了该连接队列的最大长度
    }   
    cli_len = sizeof(struct sockaddr_in);			//计算该结构体字节数
    
    while(1) {
        /*通过accept接收客户端的连接请求,并返回连接套接字用于收发数据*/
	conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len); //cli_addr发起请求的主机的地址和端口,cli_len为addr指向结构体大小
	if(conn_fd < 0) {						//conn_fd为accept成功执行时返回的一个新的客户端的套接字
	    my_err("accept",__LINE__);
	}

	/*inet_ntoa()将值为int的网络字节顺序形式的二进制IP地址转换为以"."分隔的字符串形式*/
	printf("accept a new client,ip :%s\n", inet_ntoa(cli_addr.sin_addr));

 	/*创建一个子进程处理刚刚收到的连接请求*/
	if( (pid = fork() ) == 0) {				//子进程
	    while(1) {
		if( (ret = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0) { //从参数conn_fd上所指的套接字描述符上接收数据并保存到buf所										   //指缓存区,然后为缓存区长度,最后一参置为0
     		    perror("recv");
		    exit(1);
		}
		recv_buf[ret - 1] = '\0';				//将数据结束标志'\n'替换成'\0'
	
	         if(flag_recv == USERNAME) { 				//接收到的是用户名
		     name_num = find_name(recv_buf);			//调用find_name查找用户名,返回相对应的返回值
		     switch(name_num) {
		         case -1:
 			     send_data(conn_fd, "n\n");			//调用之前定义的函数,执行结果,若find_name返回-1,则传n给此函数
			     break;

		 	 case -2:					//此时输入name为空, 退出
			     exit(1);
		             break;
	
			 default:
			    send_data(conn_fd,"y\n");			//此时输入name符合要求,传y给此函数
			    flag_recv = PASSWORD;			//flag_recv置为PASSWORD使其接受密码
			    break;

		    }
 		} else if (flag_recv == PASSWORD) {			//若输入name可寻到,flag_recv置为PASSWORD,使其接收到的是密码
		    if(strcmp(users[name_num].password, recv_buf) == 0) {
			send_data(conn_fd,"y\n");			//密码匹配时,发送数据'y'
			send_data(conn_fd,"welcome login my tcp server\n");//发送数据登陆成功
			printf("%s login\n",users[name_num].username);	 
			break;
		    } else 
			send_data(conn_fd,"n\n");				//不匹配时,发送'n'
		      }
		}
		close(sock_fd);							//关闭套接字及套接字描述符
		close(conn_fd);
		exit(0);						//结束子进程
	    } else {	
		close(conn_fd);	
	    }
	}
	return 0;
}	            	    
