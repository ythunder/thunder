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
	int	   option;
    char   cmd;
    char time[30];
    char from_id[8];
    char to_id[8];
    char chat[500];

};

/*自己的好友名单*/
struct my_friend
{
	char	Id[8];
	struct my_friend *next;

}

/*==================================================*/
int   conn_fd;
int   serv_port;
struct sockaddr_in  serv_addr;           // 全局变量
char	User_id[8];		//用户自己的账户
struct chat buf_q;
/*===================================================*/

/*接受消息处理线程*/
void *thread();

/*处理参数*/
void parameter(int argc, char **argv);

/*登陆注册函数*/
void sock_login_register();

/*登陆函数*/
void login(int conn_fd);

/*申请*/
void shenqing(int conn_fd);

/*从键盘获得账户密码*/
void get_message(char *buf, int len);  //得到信息存入buf, buf长度Len

/*私聊消息发送*/
void private_chat_send(char *User_id);


char show_menus();
/*群聊*/
void group_chat_send(char *User_id);

/*群聊接收*/
int group_chat_recv(struct chat buf);


/*私聊接收*/
int  private_chat_recv(struct chat buf);

/*获取当前系统时间*/
void get_time(char *time_buf);

/*请求添加好友函数*/
struct chat add_friend_send(char *User_id);

/*好友请求处理函数*/
void addfriend_recv(struct chat buf);

/*删除好友请求*/
void delete_friend_send(char *User_id);

void addfriend_y();

void addfriend_n();

/*请求在线好友名单*/
void ask_online_person(char *User_id);

/*处理服务器送还的在线好友ID*/
void print_online(struct chat buf);



int flag = 0;

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

	if(flag == 0) {
		return;
	}
	
	if(pthread_create(&thid, NULL, (void *)thread, NULL) != 0) {
		fprintf(stderr, "line:%d ", __LINE__);
	}

	while(1) {

		c = show_menus();

	switch(c) {
		case '1':   //发送私聊包
			private_chat_send(User_id);
			break;

		case '2':		//发送群聊包
			group_chat_send(User_id);
			break;

		case '3':
			 add_friend_send(User_id);
			break;

		case '4':
			delete_friend_send(User_id);
			break;

		case '5': //发送请求在线名单包
			ask_online_person(User_id);
			break;

		case 'y':
			addfriend_y();
			   break;

		case 'n':
			 addfriend_n();
				break;

	}

	}
}

void *thread()
{
	char	recv_buf[800];
	struct chat  buf;
	int c;
	while(1) {
	memset(recv_buf, 0, sizeof(recv_buf));
	if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0) {
		fprintf(stderr, "line:%d ", __LINE__);
	}

	memset(&buf, 0, sizeof(struct chat));
	memcpy(&buf, recv_buf, sizeof(struct chat));


	switch(buf.option) {

		case 1://私聊
			 c = private_chat_recv(buf);
			 if(c == 0) {
				 return;
			}
			break;

		case 2://群聊
			c = group_chat_recv(buf);
			if(c == 0) {
				return;
			}
			break;

		case 3://添加好友
			 addfriend_recv(buf);
			break;

		case 4://删除好友
			if(buf.cmd == 'y') {
				printf("删除成功");
			}
			break;

		case 5://显示在线人
			print_online(buf);
			break;
	}

}



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

void addfriend_y()
{
	
	char	send_buf[800];

			buf_q.cmd = 'y';
			printf("AAA");
			strcpy(buf_q.chat,"用户同意了您的添加请求.");
			memset(&send_buf, 0, sizeof(send_buf));
			memcpy(send_buf, &buf_q, sizeof(struct chat));
			if(send(conn_fd, send_buf, sizeof(send_buf), 0) < 0) {
				fprintf(stderr, "line:%d ", __LINE__);
			}
			printf("添加成功");
		
}

void addfriend_n()
{
	char	send_buf[800];

			buf_q.cmd = 'n';
			strcpy(buf_q.chat, "用户拒绝了你的添加请求.");
			memset(&send_buf, 0, sizeof(send_buf));
			printf("%s\n", buf_q.from_id);

			memcpy(send_buf, &buf_q, sizeof(struct chat));
			if(send(conn_fd, send_buf, sizeof(send_buf), 0) < 0) {
				fprintf(stderr, "line:%d ",__LINE__);
			}
			printf("已拒绝");
}	



void sock_login_register()
{
    char	choice;
//	pthread_t thid;


    conn_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(conn_fd < 0) {
	    my_err("socket", __LINE__);
	}

    if(connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0) {
 	 my_err("connect", __LINE__);
    }

    printf("1.申请 2.登陆, 3.退出"); 
    choice = getchar();
    while(choice != '1' && choice != '2' && choice != '3') {
	printf("请重新选择");
	choice = getchar();
    }
	 
    switch(choice) {
	case '1': shenqing(conn_fd);
		  break;
		  
	case '2': login(conn_fd);
 		  break;

	case '3':send(conn_fd, "3", 1, 0);
			 exit(1);
			 break;
    }
    
 }


void login(int conn_fd) 
{
    char    passwd[32];
    char    recv_buf[128];
    int	    len;
   

    if(send(conn_fd, "2", 1, 0) < 0) {
	fprintf(stderr,"line:%d ",__LINE__);
    }
    printf("ID:");
    scanf("%s", User_id);
    printf("密码");
    scanf("%s", passwd);
 
    memset(&recv_buf, 0, sizeof(recv_buf));   
    strcpy(recv_buf, User_id);
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
	if(strcmp(recv_buf,"-1") == 0) {
		printf("登陆失败\n");
	}
	if(strcmp(recv_buf, "2") == 0) {
		printf("此账号已登陆");
	}
	if(strcmp(recv_buf, "0") == 0) {
		printf("账号或者密码不正确\n");
	}
	if(strcmp(recv_buf, "1") == 0) {
		printf("登陆成功\n");
		flag = 1;
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
	if(send(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0) {
	    fprintf(stderr, "line:%d\n", __LINE__);
	}


        memset(&rec_buf, 0, sizeof(rec_buf));
	if(recv(conn_fd, rec_buf, sizeof(rec_buf), 0) < 0) {
 	    fprintf(stderr,"line:%d ", __LINE__);
	}
	printf("您的账号为%s, 请妥善保管", rec_buf);
	
	if(fopen(rec_buf, "wb+") == NULL) {
	    fprintf(stderr,"line:%d ",__LINE__);
	}
    }	


char  show_menus()
{
    char	choice;

 //   do {

	//getchar();
	while(1) {
    printf("1.私聊\n");
    printf("2.群聊\n");
    printf("3.添加好友\n");
    printf("4.删除好友\n");
    printf("5.在线人\n");
    printf("6.所有好友\n");
    printf("7.服务器日志\n");
    printf("8.退出\n");
	printf("请选择:\n");
	scanf("%c", &choice);
		if((choice >='1' && choice <= '8') || (choice == 'y') || choice == 'n') {
			break;
		}

   }
	
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
void private_chat_send(char *User_id) 
{   
    struct chat private;
    char time_buf[30];
    char chat_buf[500];
    char buf[800];
	char to_id[8];

    memset(&private, 0, sizeof(struct chat));
	printf("请输入您要发送人的Id:");
	getchar();
	get_message(to_id, 8);
	while(strcmp(to_id, User_id) == 0) {
		printf("您不能和自己聊天,请重新输入:");
		memset(&to_id, 0, sizeof(to_id));
		get_message(to_id, 8);
	}
    private.option = 1;
	private.cmd = 's';
    get_time(time_buf);		//得到当前时间
    strcpy(private.time, time_buf); 
    strcpy(private.from_id, User_id);
    strcpy(private.to_id, to_id);
 
    while(1) {
	printf("%s %s ", User_id, private.time);
	memset(chat_buf, 0, sizeof(chat_buf));
	printf("你说:");
	get_message(private.chat, 500);

    memset(&buf, 0, sizeof(buf));
    memcpy(buf, &private, sizeof(struct chat));

	send(conn_fd, buf, sizeof(buf), 0);
	if(strcmp(private.chat, "bye") == 0) {
		return;
	}
	}
}


/*2.群聊*/
void group_chat_send(char *User_id) 
{   
    struct chat group;
    char time_buf[30];
    char chat_buf[500];
    char buf[800];
    
    memset(&group, 0, sizeof(struct chat));

    group.option = 2;
    get_time(time_buf);		//得到当前时间
    strcpy(group.time, time_buf); 
    strcpy(group.from_id, User_id);
    
    while(1) {
	printf("%s  %s \n",User_id, time_buf);
	printf("你说:");
	get_message(group.chat, 500);

    memset(&buf, 0, sizeof(buf));
    memcpy(buf, &group, sizeof(struct chat));

	if(send(conn_fd, buf, sizeof(buf), 0) < 0) {
		fprintf(stderr, "line:%d ", __LINE__);
	}
	if(strcmp(group.chat,"bye") == 0) {
		return;
	}
	}
}


/*送出在线好友请求*/
void ask_online_person(char *User_id) 
{
	struct chat online;
	char        buf[800];

	memset(&online, 0, sizeof(struct chat));

	online.option = 5;
	strcpy(online.from_id, User_id);

	memset(&buf, 0, sizeof(buf));
	memcpy(buf, &online, sizeof(struct chat));

	if(send(conn_fd, buf, sizeof(buf), 0) < 0) {
		fprintf(stderr, "line;%d\n", __LINE__);
	}
}

/*收到在线好友消息*/
void print_online(struct chat buf)
{
	printf(" ----------\n");
	printf("|   %s   |\n", buf.to_id);
}


/*私聊消息接收*/
int  private_chat_recv(struct chat buf)
{
	if(buf.cmd == 'f') {
		printf("该好友不在线或此号不存在,你不能和他聊");
		return 0;
	}
	
	printf("%s  %s\n", buf.from_id, buf.time);
	printf("他说:%s\n", buf.chat);
	return 1;
}



/*群聊消息接收*/
int group_chat_recv(struct chat buf)
{
	if(buf.cmd == 'p') {
		printf("当前无人在线,你别发了");
		return 0;
	}
	printf("%s %s\n", buf.from_id, buf.time);
	printf("他说:%s\n",buf.chat);
	return 1;
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
struct chat  add_friend_send(char *User_id)
{
    struct chat addfriend;
    char        time_buf[30];
    char	check_buf[50];
    char        *ask_buf = "新朋友请求添加你为好友\n";
    char	chat_buf[500];
	char    buf[800];
	char	request_buf[20];
	char	to_id[8];

    memset(&addfriend, 0, sizeof(struct chat));
	printf("输入您要添加的用户");
	scanf("%s",to_id);
	while(strcmp(to_id, User_id) == 0) {
		printf("您不能添加自己为好友, 请重新输入:");
		memset(&to_id, 0, sizeof(to_id));
		scanf("%s", to_id);
	}
    s = find_friend(to_id);
	if(s == 1) {
			
	
	}
    addfriend.option = 3;
	addfriend.cmd = '?';
    get_time(time_buf);
    strcpy(addfriend.time, time_buf);
    strcpy(addfriend.from_id, User_id);
    strcpy(addfriend.to_id, to_id);
    
    printf("请输入验证信息: ");
	getchar();
	get_message(check_buf, 50);
    
    memset(&chat_buf, 0, sizeof(chat_buf));

    strcpy(chat_buf, ask_buf);
    strcat(chat_buf, check_buf);

    strcpy(addfriend.chat, chat_buf);

    memset(&buf, 0, sizeof(buf));
    
    memcpy(buf, &addfriend, sizeof(struct chat));

    if(send(conn_fd, buf, sizeof(buf), 0) < 0) {
	fprintf(stderr, "line:%d ", __LINE__);
    }
	return addfriend;
}



/*好友请求处理函数*/
void addfriend_recv(struct chat buf)
{	
	char	send_buf[800];
	char		choice;
	if(buf.cmd == '?')		//buf 为 struct chat buf,被调用时的全局变量, "?"为服务器处理接受请求信息做的处理
	{
		printf("用户%s 请求添加您为好友\n", buf.from_id);
		printf("验证消息:%s\n", buf.chat);
		buf_q = buf;
		printf("同意(y) 拒绝(n)\n");
	//	scanf("%c", &choice);
	}

	else if(buf.cmd == 'y') {
			printf("您已成功添加 %s 为好友", buf.to_id);
			
		} else if(buf.cmd == 'n') {
			printf("添加失败.");
		
		}	
}

/*删除好友信息*/
void delete_friend_send(char *User_id)
{
	char	dele_buf[8];
	struct chat  buf;
	char	send_buf[800];
	char	time_buf[30];
	char	to_id[8];

	memset(&buf, 0, sizeof(struct chat));

	buf.option = 4;
	printf("输入要删除的好友Id");
	scanf("%s",to_id);
	get_time(time_buf);
	strcpy(buf.time, time_buf);
	strcpy(buf.from_id, User_id);
	strcpy(buf.to_id, to_id);
	
	memset(send_buf, 0, sizeof(send_buf));
	memcpy(send_buf, &buf, sizeof(struct chat));

	if(send(conn_fd, send_buf, sizeof(buf), 0) < 0) {
		fprintf(stderr, "line:%d ",__LINE__);
	}
	printf("删除成功!");
}



/*读出自己的好友账号, 返回头指针*/
struct my_friend *my_friend_link(char *filename)
{
	FILE *fp;
	struct my_friend *head, *temp, *r;
	if((fp = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "line:%d\n", __LINE__);
	}

	head = (struct my_friend *)malloc(sizeof(struct my_friend));
	head -> next = NULL;
	r = head;

	while(!feof(fp)) {
	
		temp = (struct my_friend *)malloc(sizeof(struct my_friend));
		fscanf(fp, "%s", temp -> Id);

		r -> next = temp;
		r = temp;
	}
	r -> next =	NULL;
	fclose(fp);
	return head;
	
}

/*查找是否有此好友*/
int find_friend(char *add_id)
{
	struct my_friend *head, temp;
	
	head = my_friend_link(User_id);

	for(temp = head; temp != NULL; temp = temp -> next) {
	
		if(strcmp(temp -> Id, add_id) == 0) {
			return 1;
		}
	}

	return 0;
}
