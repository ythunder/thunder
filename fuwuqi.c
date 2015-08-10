#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "my_recv.c"


#define PORT_NUM 9527	//端口号
#define LISTENQ	 13 	//可建立的最大连接数

struct person
{
    char	   Id[32];
    char 	   Msg[32];
    struct person  *next;
}


int main(void)
{	
    int		       optval;
    int		       sock_fd;					//服务器端套接字
    int		       conn_fd;					//客户端套接字
    int		       flag = 0;
    struct sockaddr_in serv_addr;				//定义服务器端套接字地址结构变量
    struct sockaddr_in cli_addr;				//定义客户端套接字地址结构变量
    struct person	*head;
    struct ptemp 	*temp;
    char 		*string;
      
    /*初始化服务器端地址结构*/
    memset (&serv_addr, 0, sizeof(struct sockaddr_in) );	//将结构体清零
    serv_addr.sin_family = AF_INET;				//地址类型,使用IPv4 TCP/IP协议
    serv_addr.sin_port = htons(PORT_NUM);			//端口号,宏定义
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);		//IP地址,INADDR_ANY监听所有

    /*创建一个TCP套接字*/
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);			//AF_INET指定套接字使用的协议,SOCK_STREAM指定套接字类型
    if (sock_fd == -1) {
	my_err("socket ",__LINE__);
    }

    /*绑定端口*/
    if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) == -1) {  
	my_err( "bind ",__LINE__);
    }

    /*转换为监听套接字*/
    if(listen(sock_fd, LISTENQ) < 0) {				//LISTENQ为链接的客户端个数上限
	my_err("listen ",__LINE__);
    }
    FD_ZERO(&readfds);
    FD_SET(server_fd, &readfd);

    /*设置套接字属性*/
    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval,  //optval用来存放获得的套接字选项
	sizeof(int)) < 0) {
	my_err("setsocket", __LINE__);
    }

    /*等待客户端链接*/
    while(1) {
	    conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr,		//通过accept接受客户端的请求
	    sizeof(struct sockaddr_in));
	    if(conn_fd < 0) {
		my_err("accept ", __LINE __);
	    }    
	    printf("Client from %s", inet_ntoa(cli_addr.sin_addr));		//cli_addr为客户端地址结构
	    //创建一个子进程处理刚刚收到的连接请求
	    if( (pid = fork()) == 0) {
		while(1) {
		    if((ret = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0) {	//通过conn_fd接受数据存入recv_buf中
			perror("recv");
			exit(1);
		    }
		    recv_buf[ret-1] = '\0';		
			
			head = read_file();

			/*判断从客户端收到的账户名是否已存在,存在返回'r',不存在返回'y'*/
			for(temp = head->next; temp != NULL; temp = temp->next) {
			    if(strcmp(temp->Id, recv_buf) == 0 ) {
				my_send(conn_fd, "r\n");
				flag = 1;
			    }
      		        }
			if(flag == 0) {
			    my_send(conn_fd,"y\n");
			}



/*******************************************
   conn_fd:处于连接状态的套接字
   string :要发出的字符串
   实现将string中的内容传递给连接的客户端
********************************************/
void my_send(int conn_fd, const char *string) 
{
    if(send(conn_fd, string, strlen(string), 0) < 0) {
	my_err("send",__LINE__);
    }
}


/********************************************
   number.txt文件:存储已注册客户的昵称密码
   struct person :存储用户昵称和密码
   返回值:返回链表头指针
   实现将文件中的信息存入链表
*********************************************/
struct person *read_file()
{
    struct person *phead,*r,*ptemp;
    FILE *fp;

    if((fp = fopen("number.txt", "rt")) == NULL) {
	my_err("fopen", __LINE__);
	exit(1);
    }

    phead = (struct person *)malloc(sizeof(struct person));
    phead->next = NULL;
    r = phead;
  
    while(!feof(fp)) {
	ptemp = (struct person *)malloc(sizeof(struct person));
	fscanf(fp,"%s %s", &ptemp->Id, ptemp->Msg);

	r->next == ptemp;
	r = ptemp;
    }
	
    r->next = NULL;
    fclose(fp);
    getchar();
    return phead;
}
	
