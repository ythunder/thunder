#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>


struct chat
{
    char   cmd;
    char time[30];
    char from_id[8];
    char to_id[8];
    char chat[500];

};



/*==================================================*/
int   conn_fd;
int   serv_port;
struct sockaddr_in  serv_addr;           // 全局变量
/*===================================================*/


void my_err(const char *err_string, int line) 
{
    fprintf(stderr, "line:%d ",line);
    perror(err_string);
    exit(1);
}

  
void main(int argc, char **argv)
{
	pthread_t	thid;
    char		c;

	parameter(argc, argv);  
    
	sock_login_register();   
    
	c = show_menus();

	if(pthread_create(&thid, NULL, (void *)thread, c) != 0) {
		fprintf(stderr, "line:%d ", __LINE__);
	}

	function_all(char c);
}

void thread(char c)
{
	char	recv_buf[800];
	struct chat  buf;

	while(1) {
	memset(recv_buf, 0, sizeof(recv_buf));
	if(recv(conn_fd, buf, sizeof(buf), 0) < 0) {
		fprintf(stderr, "line:%d ", __LINE__);
	}

	memset(&buf, 0, sizeof(buf));
	memcpy(&buf, recv_buf, sizeof(buf));

	switch(c) {

		case '1'://私聊
			private_chat_recv(buf);

		case '2'://群聊
			private_chat_recv(buf);

		case '3'://添加好友
			add_friend_recv(buf);

		case '4'://删除好友
			delete_friend_recv(buf);
	}

}
}


void function(char c) 
{
	switch(c) {
		case '1':
			private_chat_send();







}
void parameter(int argc, char **argv)
{
    int   i;

    if(argc != 5) {
	printf("Usage: [-p] [serv_port] [-a] [serv_address]\n");
	exit(1);
    }

    
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));	
    serv_addr.sin_family = AF_INET;
    
    for(i=1; i<argc; i++) {
	if(strcmp("-p", argv[i]) == 0) {
	    serv_port = atoi(argv[i+1]);		
	    if(serv_port < 0 || serv_port > 65535) {	
         	printf("invalid serv_addr.sin_port\n");
		exit(1);
	    } else {
		serv_addr.sin_port = htons(serv_port); 
	    }
	    continue;
	}
    
	if(strcmp("-a",argv[i]) == 0) {
	    if(inet_aton(argv[i+1], &serv_addr.sin_addr) == 0) {
		printf("invalid server ip address\n");
		exit(1);
	    }
	    continue;
	}
    }
	if(serv_addr.sin_port == 0 || serv_addr.sin_addr.s_addr == 0) {
	    printf("Usage:[-p] [serv_addr.sin_port] [-a][serv_address]\n");
	    exit(1);
	}
}




void sock_login_register()
{
    char	choice;

    conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(conn_fd < 0) {
	    my_err("socket", __LINE__);
	}

    if(connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0) {
 	 my_err("connect", __LINE__);
    }

    printf("1.申请 2.登陆"); 
    choice = getchar();
    while(choice != '1' && choice != '2') {
	printf("请重新选择");
	choice = getchar();
    }
	 
    switch(choice) {
	case '1': shenqing(conn_fd);
		  break;
	case '2': login(conn_fd);
 		  break;
    }
    
 }

void login(int conn_fd) 
{
    char    userid[8];
    char    passwd[32];
    char    recv_buf[128];
    int	    len;
   

    if(send(conn_fd, "2", 1, 0) < 0) {
	fprintf(stderr,"line:%d ",__LINE__);
    }
    printf("ID:");
    scanf("%s", userid);
    printf("密码");
    scanf("%s", passwd);
 
    memset(&recv_buf, 0, sizeof(recv_buf));   
    strcpy(recv_buf, userid);
    len = strlen(recv_buf);
    recv_buf[len] = '#';
    len++;
    recv_buf[len] = '\0';
    strcat(recv_buf, passwd); 
   
    if(send(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0) { //送出打包过的ID和密码
	fprintf(stderr, "line:%d ",__LINE__);
    }
    
       memset(&recv_buf, 0 ,sizeof(recv_buf));
    if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0) { //收到返回信息
	fprintf(stderr, "line:%d ", __LINE__);
    }
   
 }    


void shenqing(int conn_fd)
{	
    char   username[32], passwd1[32], passwd2[32];
    char   recv_buf[64], rec_buf[8];
    int    len,i;	

	if(send(conn_fd, "1", 1, 0) < 0) {
	   fprintf(stderr, "line:%d ", __LINE__);
	}
	printf("账户名:");
	scanf("%s", username);
	while(strlen(username) > 32 ) {
	    printf("错误,请重新输入:");
	    scanf("%s", username);
	}
		
	do {
	    printf("密码:");
	    scanf("%s", passwd1);
	    printf("确认密码:");
	    scanf("%s", passwd2);
	    while(strcmp(passwd1,passwd2) != 0) {
	    printf("确认密码:");
	    scanf("%s", passwd2);
	    }
	} while(strlen(passwd1) > 32);
	memset(&recv_buf, 0, sizeof(recv_buf));

	strcpy(recv_buf, username);
	len = strlen(recv_buf);
	recv_buf[len] = '#';
	len++;
	recv_buf[len] = '\0';
	strcat(recv_buf, passwd1);
		printf("%s\n", recv_buf);    
	if(send(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0) {
	    fprintf(stderr, "line:%d\n", __LINE__);
	}
	printf("%s\n",recv_buf );


        memset(&rec_buf, 0, sizeof(rec_buf));
	if(recv(conn_fd, rec_buf, sizeof(rec_buf), 0) < 0) {
 	    fprintf(stderr,"line:%d ", __LINE__);
	}
	printf("您的账号为%s, 请妥善保管", rec_buf);
	
	if(fopen(rec_buf, "wb+") == NULL) {
	    fprintf(stderr,"line:%d ",__LINE__);
	}
    }	


int  show_menu()
{
    char	choice;

    do {
    system("reset");
    printf("1.私聊");
    printf("2.群聊");
    printf("3.添加好友");
    printf("4.删除好友");
    printf("5.在线好友");
    printf("6.查看好友资料");
    printf("7.服务器日志");
    printf("8.退出");
    choice = getchar();
   }while(choice < '1' || choice > '7');

	return choice;
}



void get_message(char *buf, int len)  //得到信息存入buf, buf长度Len
{
    int   i=0;
    char  c;

    c = getchar();
    while((c != '\n')&&(i <= len-2)) {
	buf[i] = c;
	i++;
	c = getchar();
    }
    buf[i] = '\0';
	
}

/*1.私聊*/
void private_chat_send(char *from_id, char *to_id) 
{   
    struct chat private;
    char time_buf[30];
    char chat_buf[500];
    char buf[800];

    memset(&private, 0, sizeof(struct chat));

    private.cmd = '1';
    get_time(time_buf);		//得到当前时间
    strcpy(private.time, time_buf); 
    strcpy(private.from_id, from_id);
    strcpy(private.to_id, to_id);
    
    get_message(chat_buf, 500);
    strcpy(private.chat, chat_buf);
    
    memset(&buf, 0, sizeof(buf));
    memcpy(buf, &private, sizeof(struct chat));

	if(send(conn_fd, buf, sizeof(buf), 0) < 0) {
		fprintf(stderr, "line:%d ", __LINE__);
	}
	printf("%s  %s \n",from_id, time_buf);
	printf("%s<<<<<", chat_buf);

}


/*私聊消息接收*/
void private_chat_recv(struct chat buf)
{
		printf("%s  %s", buf.from_id, buf.time);
		printf(">>>>>%s", chat_buf);
}



/*获取系统当前时间*/
void get_time(char *time_buf)
{
    time_t   now;
    struct tm *timenow;

    time(&now);
    timenow = localtime(&now);
    strcpy(time_buf, asctime(timenow));
}



/*3.添加好友*/
void add_friend_send(char *from_id, char *to_id)
{
    struct chat addfriend;
    char        time_buf[30];
    char	check_buf[50];
    char        *ask_buf = "新朋友请求添加你为好友\n";
    char	chat_buf[500];
	char    buf[800];
	char	request_buf[20];

    memset(&addfriend, 0, sizeof(struct chat));

    addfriend.cmd = '1';
    get_time(time_buf);
    strcpy(addfriend.time, time_buf);
    strcpy(addfriend.from_id, from_id);
    strcpy(addfriend.to_id, to_id);
    
    printf("请输入验证信息: ");
    get_message(check_buf, 50);
    
    memset(&chat_buf, 0, sizeof(struct chat));

    strcpy(chat_buf, ask_buf);
    strcat(chat_buf, check_buf);

    strcpy(addfriend.chat, chat_buf);

    memset(&buf, 0, sizeof(buf));
    
    memcpy(buf, &addfriend, sizeof(struct chat));

    if(send(conn_fd, buf, sizeof(buf), 0) < 0) {
	fprintf(stderr, "line:%d ", __LINE__);
    }
}

/*好友请求处理函数*/
void_addfriend_recv(struct chat buf)
{	
	char	send_buf[800];
	if(strcmp(buf.cmd, '?') == 0)		//buf 为 struct chat buf,被调用时的全局变量, "?"为服务器处理接受请求信息做的处理
	{
		printf("用户%s 请求添加您为好友\n", buf.from_id);
		printf("%s", buf.chat);

		printf("同意(y)或拒绝(n)\n");
		choice = getchar();
		
		if(choice == 'y') {		
			strcpy(buf.cmd, 'y');
			strcpy(buf.chat,"用户同意了您的添加请求.");
			memset(&send_buf, 0, sizeof(buf));
			memcpy(send_buf, &buf, sizeof(struct chat));
			if(send(conn_fd, send_buf, sizeof(send_buf), 0) < 0) {
				fprintf(stderr, "line:%d ", __LINE__);
			}
			printf("添加成功");
			getchar();
		}
		else {
			strcpy(buf.cmd, 'n');
			strcpy(buf.chat, "用户拒绝了你的添加请求.");
			memset(&send_buf, 0, sizeof(buf));
			memcpy(send_buf, &buf, sizeof(struct chat));
			if(send(conn_fd, send_buf, sizeof(send_buf), 0) < 0) {
				fprintf(stderr, "line:%d ",__LINE__);
			}
			printf("已拒绝");
			getchar();
		}
	}
	else if(strcmp(buf.cmd, 'y') == 0) {
			printf("您已成功添加 %s 为好友", buf.to_id);
		} else {
			printf("对方拒绝了您的添加请求,请节哀顺变.");
		}
		getchar();
			
}


void delete_friend_send(char from_id, char to_id)
{
	char	dele_buf[8];
	struct chat  buf;
	char	send_buf[800];
	char	time_buf[30];

	memset(&buf, 0, sizeof(struct chat));

	buf.cmd = '4';
	
	gettime(time_buf);
	strcpy(buf.time, time_buf);
	strcpy(buf.from_id, from_id);
	strcpy(buf.to_id, to_id);
	
	memset(send_buf, 0, sizeof(send_buf));
	memcpy(send_buf, &buf, sizeof(struct chat));

	if(send(conn_fd, send_buf, sizeof(buf), 0) < 0) {
		fprintf(stderr, "line:%d ",__LINE__);
	}
}


void delete_friend_recv(struct chat buf)
{
	if(buf.cmd == 'y') {
		printf("您已删除该好友\n");
	} else {
		printf("删除失败\n");
	}
}
