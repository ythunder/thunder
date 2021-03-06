#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <malloc.h>

#define PORT 8888

//用户的ID,账户名,密码,存在number.txt中
struct person
{
    char	Id[8];
    char	username[32];
    char	passwd[32];
};

struct friends	
{
    char		Id[8];
    char 		username[32];
    char        state[4];
    struct friends 	*next;
};

struct chat			//发送的命令包
{
	int			option;
    char		cmd;
    char	time[30];
    char	from_id[8];
    char 	to_id[8];
    char	chat[500];

};

struct sockfd		//在线用户链表结点
{
    char   Id[8];
    int    conn_fd;
	struct sockfd *next;
};


struct my_friend	//自己好友名单
{
    char    Id[8];
    struct my_friend *next;

};


/*********************************************/
struct sockfd  *phead;

char  User_id[8];
/***********************************************/
/*添加上线用户*/
struct sockfd *add_person(struct sockfd *phead, struct sockfd *temp)
{
	temp -> next = NULL;
	if(phead != NULL) {
		temp -> next = phead;
	}
	return temp;
}


/*删除*/
struct sockfd *delet_person(struct sockfd *phead, struct sockfd *temp)
{
	struct sockfd *r, *p;
	
	r = phead;
	if (r -> conn_fd == temp -> conn_fd) {
		if(r->next == NULL) {
			free(phead);
			return NULL;
		}
		phead = r->next;
		return phead;
	}

	while(r != NULL) {
	 p = r -> next;
	 if (p -> conn_fd == temp ->conn_fd) {
		r -> next = p -> next;
		return phead;
	 }
	 r = r->next;
	}
}


/*查找*/
int search_person(char *user_id)
{
	struct sockfd *ptemp;
	int		conn_fd = 0;
	char a[8],b[8];

	ptemp = phead;
	while(ptemp != NULL) {
		if(strcmp(user_id, ptemp->Id) == 0) {
			conn_fd = ptemp->conn_fd;
		}
		ptemp = ptemp->next;
	}
	return conn_fd;
}

void my_err(const char *err_string, int line)
{
	fprintf(stderr, "line:%d ",line);
	perror(err_string);
	exit(1);
}


//所有功能及调用线程
void login_register();

//处理消息包线程
void *thread(int *fd);

//私聊
void private_chat(struct chat buf,int to_connfd);

//群聊
void group_chat(struct chat buf);

//添加好友
void add_friend(struct chat buf, int to_connfd);

//删除好友
void delet(struct chat buf);

//读用户文件,返回头指针
struct my_friend *read_file_friend(char *filename);

//用户登陆
int denglu(int conn_fd);

//查找核对用户名密码
int search(char *Id, char *msg);

//用户注册
void zhuce(int conn_fd);

//解析账户密码包
void jiexi(char *recv_buf, int id,struct person *message);

//系统分配ID
int fen_id(void);

//读注册过得用户信息
//struct friends *read_file_friends();

//改变用户登陆状态
void state_file_friends(struct friends *phead, char *id, char *string);

/*注册用户写入文件*/
void write_file_friends(char *user_id, char *username);

/*将在线名单送给客户*/
void show_online(struct chat buf);

void show_friend_menus(struct chat buf);


void exit_connfd(struct chat buf);


struct my_friend *read_file_friends(char *filename);

void get_time(char *time_buf)
{
	time_t   now;
	struct tm *timenow;

	time(&now);
	timenow = localtime(&now);
	strcpy(time_buf, asctime(timenow));

}

void main()
{
    int optval;
    int   		sock_fd;
    struct sockaddr_in  cli_addr, serv_addr;
    char   time_g[30];
	FILE   *fp2;

    if( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		fprintf(stderr, "line: %d\n", __LINE__);
    }

//	optval = 1;

//	if(setsockopt(sock_fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&optval,  sizeof(int)) < 0) {
//		fprintf(stderr, "line:%d ",__LINE__);
//	}

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     
   
    if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0) {
		if((fp2 = fopen("errnolog.txt", "at")) == NULL) {
			fprintf(stderr, "line:%d\n", __LINE__);
		}
		get_time(time_g);
		fprintf(fp2, "%s",time_g);
		fprintf(fp2, "%s\n", "打开文件errnolog.txt失败");
		fclose(fp2);
		my_err("bind", __LINE__);
    }


    if(listen(sock_fd, 13) < 0) {
		fprintf(stderr, "line:%d \n",__LINE__);
    }
 
	login_register(sock_fd);

}


void login_register(int sock_fd)
{
    int			conn_fd;
    socklen_t	cli_len;
    pthread_t   thid;
    char        buf[8];
    int			c;
    struct sockaddr_in cli_addr;
	FILE	*fp2;
	char     time_buf[30];

    cli_len = sizeof(struct sockaddr_in);
    while(1) {
    conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
    
	if(conn_fd < 0) {
		
		if((fp2 = fopen("errnolog.txt", "at")) == NULL) {
			fprintf(stderr, "line:%d\n", __LINE__);
		}
		get_time(time_buf);
		fprintf(fp2, "%s ",time_buf);
		fprintf(fp2, "%s\n", "与客户端建立连接失败");
		fclose(fp2);

		fprintf(stderr, "line: %d\n",__LINE__);
    }
    printf("accept a new client ,ip: %s\n", inet_ntoa(cli_addr.sin_addr));
    
    if(pthread_create(&thid, NULL, (void *)thread, &conn_fd) != 0) {
	printf("线程调用失败\n");
    }}

}




void *thread(int *fd)
{	
    int    c;
    char   c_buf[8];
    int conn_fd = *fd;
	char	recv_buf[800];
	struct chat buf;
	int  success;
	int		to_connfd;
	struct sockfd  *ptemp=NULL;
	int optval;
	char  time_buf[30];
	char  *string = "账户已上线";
	FILE *fp1;

	if(setsockopt(conn_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval,  sizeof(int)) < 0) {
		fprintf(stderr, "line:%d ",__LINE__);
	}
	
	
	if(recv(conn_fd, c_buf, sizeof(c_buf), 0) < 0) {
	    fprintf(stderr,"line:%d ",__LINE__);
	}

	c = atoi(c_buf);
           
	switch(c) {
	    case 1: zhuce(conn_fd);
		    break;
            
	    case 2:success = denglu(conn_fd);
	
				if(success == 1) {

					ptemp = (struct sockfd *)malloc(sizeof(struct sockfd));
					ptemp->conn_fd = conn_fd;
					strcpy(ptemp->Id,User_id);
					
					phead = add_person(phead, ptemp);

					if((fp1 = fopen("serverlog.txt", "at")) == NULL) {
						fprintf(stderr, "line:%d\n", __LINE__);		
					}
					get_time(time_buf);
					fprintf(fp1, "%s  ", time_buf);
					fprintf(fp1, "%s", User_id);
					fprintf(fp1, "%s\n", string);
					fclose(fp1);

					memset(recv_buf, 0, sizeof(recv_buf));
		while(1) {
			        int   ret;
					if((ret = recv(conn_fd, recv_buf, sizeof(recv_buf), 0)) < 0) {
					fprintf(stderr, "line:%d ",__LINE__);
					}
				
					memset(&buf, 0, sizeof(struct chat));
					memcpy(&buf, recv_buf, sizeof(struct chat));

					to_connfd = search_person(buf.to_id);
					

					switch(buf.option) {
		
					case 1:	//私聊
						private_chat(buf, to_connfd);
						break;

					case 2: //群聊
					 group_chat(buf);
						break;

					case 3://添加好友
						add_friend(buf, to_connfd);
						break;

					case 4://删除好友
						delet(buf);
						break;

					case 5://显示在线人

						show_online(buf);
						break;
					case 6:
						show_friend_menus(buf);
						break;

					case 8://退出
						exit_connfd(buf);
						pthread_exit(0);
						break;
					}		
					}}
			   break;
				
		case 3: close(conn_fd);
				break;
	}}



void exit_connfd(struct chat buf)
{

	struct sockfd  *temp;
	int			conn_fd;
	char		send_buf[800];
	char		*string = "账户已下线";
	FILE        *fp1;
	char         time_buf[30];

    conn_fd = search_person(buf.from_id);
	temp = (struct sockfd *)malloc(sizeof(struct sockfd));
	strcpy(temp -> Id, buf.from_id);
	temp -> conn_fd = conn_fd;
	temp -> next = NULL;
	phead = delet_person(phead, temp);
	buf.cmd = 'y';
	memcpy(send_buf, &buf, sizeof(struct chat));

	
	if(send(conn_fd, send_buf, sizeof(send_buf), 0) < 0) {
		fprintf(stderr, "line:%d\n", __LINE__);
	}

	close(conn_fd);
	if((fp1 = fopen("serverlog.txt", "at")) == NULL) {
		fprintf(stderr, "line:%d\n", __LINE__);
	}
	get_time(time_buf);
	fprintf(fp1, "%s ", time_buf);
	fprintf(fp1, "%s", buf.from_id);
	fprintf(fp1, "%s\n", string);
	fclose(fp1);

}

/*私聊处理*/
void private_chat(struct chat buf,int to_connfd)
{
	int		from_connfd;
	char	send_buf[800];
	
	//先看这货在不在线, 在的话送过去
	memset(send_buf, 0, sizeof(send_buf)); 

	from_connfd = search_person(buf.from_id);

	if(to_connfd == 0) {
		buf.cmd = 'f';
		memcpy(send_buf, &buf, sizeof(struct chat));
		if(send(from_connfd, send_buf, sizeof(send_buf), 0) < 0) {
			fprintf(stderr, "line:%d ", __LINE__);
		}
	
	}
	else {
		memcpy(send_buf, &buf, sizeof(struct chat));
		if(send(to_connfd, send_buf, sizeof(send_buf), 0) < 0) {
			fprintf(stderr, "line:%d ", __LINE__);
		}
		
	
	}
}

/*群聊消息处理*/
void group_chat(struct chat buf)
{
	int		to_fd;
	char	send_buf[800];
	int		i=0;
	struct sockfd  *ptemp;	
	int	from_fd;

	memset(send_buf, 0, sizeof(send_buf));
	memcpy(send_buf, &buf, sizeof(struct chat));
	for(ptemp = phead; ptemp != NULL; ptemp = ptemp->next) {
		if(strcmp(ptemp->Id, buf.from_id) == 0){
			continue;
		}
		to_fd = search_person(ptemp->Id);
		if(to_fd > 0) {
			send(to_fd, send_buf, sizeof(send_buf), 0);
			i++;
		}
		}
	if(i == 0) {
		buf.cmd == 'p';
		from_fd = search_person(buf.from_id);
		memset(send_buf, 0, sizeof(send_buf));
		memcpy(send_buf, &buf, sizeof(struct chat));
		send(from_fd, send_buf, sizeof(send_buf), 0);
	}

}
	
/*添加好友*/
void add_friend(struct chat buf, int to_connfd)
{
	FILE	*fp1, *fp2, *fp3;
	int		from_connfd;
	char	send_buf[800];
	char    time_buf[30];

	
	from_connfd = search_person(buf.from_id);

	if(to_connfd == 0) {
		buf.cmd = 'i';
		memcpy(send_buf, &buf, sizeof(struct chat));
		if(send(from_connfd, send_buf, sizeof(send_buf), 0) < 0) {
			fprintf(stderr, "line:%d ", __LINE__);
		}
		return;
	}
	if(buf.cmd == '?') {
		memcpy(send_buf, &buf, sizeof(struct chat));
		if(send(to_connfd, send_buf, sizeof(send_buf), 0) < 0) {
			fprintf(stderr,"line:%d ",__LINE__);
		}

	} else {

		from_connfd = search_person(buf.from_id);

		memcpy(send_buf, &buf, sizeof(struct chat));
		if(send(from_connfd, send_buf, sizeof(send_buf), 0) < 0) {
			fprintf(stderr, "line:%d ", __LINE__ );
		}
		if(buf.cmd == 'y') {
			fp1 = fopen(buf.from_id,"a+");
			if(fp1 == NULL) {
				if((fp3 = fopen("errnolog.txt", "at")) == NULL) {
					fprintf(stderr, "line:%d\n", __LINE__);
				}
				get_time(time_buf);
				fprintf(fp3, "%s", time_buf);
				fprintf(fp3, "%s", buf.from_id);
				fprintf(fp3, "%s\n", "文件打开失败");
				fclose(fp3);

				fprintf(stderr, "line:%d\n", __LINE__);
			}
			fprintf(fp1,"%s ", buf.to_id);
			fclose(fp1);
			fp2 = fopen(buf.to_id, "a+");
			if(fp2 == NULL) {
				
				if((fp3 = fopen("errnolog.txt", "at")) == NULL) {
					fprintf(stderr, "line:%d\n", __LINE__);
				}
				get_time(time_buf);
				fprintf(fp3, "%s", time_buf);
				fprintf(fp3, "%s", buf.to_id);
				fprintf(fp3, "%s\n", "文件打开失败");
				fclose(fp3);
			}
			fprintf(fp2,"%s ", buf.from_id);
			fclose(fp2);
		}
	}
}

/*删除好友*/
void delet(struct chat buf)
{
	FILE	*fp1,*fp2;
	struct my_friend  *head, *ptemp, *head2;
	char	delet_id[8];
	int   fromfd, tofd;
	char   send_buf[800];
	int ting=0, tings=0;

	head = read_file_friend(buf.from_id);
	head2 = read_file_friend(buf.to_id);


		fromfd = search_person(buf.from_id);
		tofd = search_person(buf.to_id);
	
		if(head2 == NULL) {

		buf.cmd = 'c';
		memset(send_buf, 0, sizeof(send_buf));
		memcpy(send_buf, &buf, sizeof(struct chat));
		send(fromfd, send_buf, sizeof(send_buf), 0);
		return;
	}

	if((fp1 = fopen(buf.from_id, "w")) == NULL) {
		fprintf(stderr, "line:%d ", __LINE__);
	}
	for(ptemp = head->next; ptemp->next != NULL; ptemp = ptemp->next) {
		if(strcmp(ptemp->Id, buf.to_id) != 0 ) {
			fprintf(fp1, "%s ", ptemp->Id);
		}
		else {
			ting = 1;    //存在这个账号
		}
	}
	if(ting == 1) {
		buf.cmd = 'y';
		memcpy(send_buf, &buf, sizeof(struct chat));
		send(fromfd, send_buf, sizeof(send_buf), 0);
	
	}
	if(ting == 0) {
		buf.cmd = 'w';
		memset(send_buf, 0, sizeof(send_buf));
		memcpy(send_buf, &buf, sizeof(struct chat));
		send(fromfd, send_buf, sizeof(send_buf), 0);
	}
	fclose(fp1);

	if((fp2 = fopen(buf.to_id, "w")) == NULL) {
		fprintf(stderr, "line:%d ",__LINE__);
	}
	for(ptemp = head2->next; ptemp->next != NULL; ptemp = ptemp->next) {
		if(strcmp(ptemp->Id, buf.from_id) != 0) {
			fprintf(fp2, "%s ", ptemp->Id);
		}
		else {
			tings = 1;
		}
	}
	if(tings == 1) {
		buf.cmd = 'u';
		memset(send_buf, 0, sizeof(send_buf));
		memcpy(send_buf, &buf, sizeof(struct chat));
		send(tofd, send_buf, sizeof(send_buf), 0);
	}
	fclose(fp2);
}




/*读出myfriend中的文件，返回头指针*/
struct my_friend *read_file_friend(char *filename)	
{
    FILE	*fp;
    struct my_friend  *head, *ptemp, *r;
   if(( fp = fopen(filename, "r")) == NULL){	//只读，打开文件friends, 中存结构体(id ,username)
	return NULL;
   }
    head = (struct my_friend *)malloc(sizeof(struct my_friend));
    head->next = NULL;
    r = head;

    while(!feof(fp)) {
	
	ptemp = (struct my_friend *)malloc(sizeof(struct my_friend));
	fscanf(fp, "%s", ptemp->Id);

	r->next = ptemp;
	r = ptemp;
    }
    r->next = NULL;
    fclose(fp);
    return head;
   
 }



/*处理登陆请求*/
int denglu(int conn_fd)
{
    char  recv_buf[128];
    int   len, i, t, result;
    char  use_msg[32], use_name[32];
    char  *string4 = "on";
	struct sockfd  *ptemp;
	int success = 0;
    memset(&recv_buf, 0, sizeof(recv_buf));
    if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0) {
	fprintf(stderr, "line:%d ",__LINE__);
    }
   
    len = strlen(recv_buf); 
    
    for(i=0; i<len; i++) {
	if(recv_buf[i] == '#') {
	    t = i;
	    break;
	 }
    }

     memset(&User_id, 0, sizeof(User_id));
     for(i=0; i<t; i++) 
        User_id[i] = recv_buf[i];  
	User_id[t] = '\0';

    memset(&use_msg, 0, sizeof(use_msg));
    for(i=t+1; i<len; i++)
	use_msg[i-t-1] = recv_buf[i];
	use_msg[len-t-1] = '\0';
 
    result = search(User_id, use_msg);

	for(ptemp = phead; ptemp != NULL; ptemp = ptemp -> next)
	{
		if(strcmp(ptemp -> Id, User_id) == 0) {
			send(conn_fd, "2", 1, 0);
			return 0;
		}
	}
    switch(result) {

	case -1: 
		if(send(conn_fd, "-1",1, 0) < 0) {
		    fprintf(stderr, "line:%d\n", __LINE__);
		}
		break;

	case 0: 
		if(send(conn_fd, "0",1, 0) < 0) {
		    fprintf(stderr, "line:%d\n", __LINE__);
		}
		break;

	case 1: 
		if(send(conn_fd, "1",1, 0) < 0) {
		    fprintf(stderr, "line:%d\n", __LINE__);
		}
		
		success = 1;
		break;
    }
	return success;
}


int search(char *Id, char *msg)
{
    struct person  ptemp;
    FILE 	   *fp = NULL;
    int		   id,id_max;
    int		    count;
    memset(&ptemp, 0, sizeof(struct person));
    id = atoi(Id);
    if((fp = fopen("number.txt", "rb+")) == NULL) {
	return -1;
    } 
    if(fread(&ptemp, sizeof(struct person), 1, fp) <= 0) {
	return -1;
    }
    id_max = atoi(ptemp.Id);
    if(id > id_max || id < 10000) {
	return 0;
    }
    else {
	count =  id - 9999;
	if(fseek(fp, count*sizeof(struct person), SEEK_SET) < 0) {
	    return -1;
	}
	if(fread(&ptemp, sizeof(struct person), 1, fp) <= 0) {
	    return -1;
	}
	if((strcmp(ptemp.Id, Id) == 0) && (strcmp(ptemp.passwd,msg) == 0)) {
	    return 1;
	}    
	return 0;
    }
}





void zhuce(int conn_fd)
{
    char 	  recv_buf[128];
    int           new_id;
    FILE	  *fp, *fp1, *fp3;	
    struct person message;
	char   time_buf[30];
	char   *string = "账户注册成功";

    if(recv(conn_fd, &recv_buf, sizeof(recv_buf), 0) < 0) { 
       fprintf(stderr, "line:%d\n",__LINE__);
    }
	
    new_id = fen_id();
    jiexi(recv_buf, new_id, &message);

    fp = fopen("number.txt", "ab");
    if(fp == NULL) {
        fprintf(stderr, "line:%d\n", __LINE__);
    }
    if(fwrite(&message, sizeof(struct person), 1, fp) == -1 ) {
       fprintf(stderr,"line:%d", __LINE__);
    }
    if(send(conn_fd, message.Id, sizeof(message.Id), 0) < 0) {
	fprintf(stderr, "line:%d ", __LINE__);
    }
	
    fclose(fp);

	if((fp1 = fopen("serverlog.txt", "at")) == NULL) {
		fprintf(stderr, "line:%d\n", __LINE__);
	}
		get_time(time_buf);
		fprintf(fp1, "%s  ",time_buf);
		fprintf(fp1, "%d", new_id);
		fprintf(fp1, "%s\n", string);
		fclose(fp1);
    write_file_friends(message.Id, message.username); 
    
	fp3 = fopen(message.Id, "wt");
	fclose(fp3);
}

void jiexi(char *recv_buf, int id,struct person *message)
{
    int t;   //记录#所在位置
    int len;  
    int i;
    char username[32],passwd[32];
 
    len = strlen(recv_buf);
    for(i=0;i<len;i++) {
	if(recv_buf[i] == '#') {
	    t = i;
	    break;
	}
    }

    for(i=0; i<t; i++) 		//解析账号
	username[i] = recv_buf[i];
	username[t] = '\0';

    for(i=t+1; i<len; i++)	//解析密码
 	passwd[i-t-1] = recv_buf[i];
	passwd[len-t-1] = '\0';

    
    memset(message, 0, sizeof(struct person));
    sprintf(message -> Id, "%d", id);
    strcpy(message -> username, username);
    strcpy(message -> passwd, passwd);

}




int fen_id(void)
{
    FILE *fp;
    struct person message;
    int id;
    char string[8];
	memset (&message, 0, sizeof (struct person)) ;
	if((fp = fopen("number.txt", "rb+")) == NULL) {
	if((fp = fopen("number.txt", "wb+")) == NULL) { 
		fprintf(stderr, "line:%d ",__LINE__);
        }
        strcpy(message.Id, "10000");     
        if((fwrite(&message, sizeof(struct person), 1, fp)) == -1) {
	fprintf(stderr, "line:%d\n", __LINE__);
    }
    fclose(fp);
   }
    

    memset(&message, 0, sizeof(struct person));
    if((fp = fopen("number.txt", "rb+")) == NULL) {
	fprintf(stderr,"line:%d ", __LINE__);
    }

    if(fread(&message,sizeof(struct person), 1, fp) == 0) {
	fprintf(stderr,"line:%d\n", __LINE__);
    }
    
    id = atoi(message.Id);
    id++;

     if(sprintf(message.Id, "%d", id) <= 0) {
	fprintf(stderr,"line:%d ",__LINE__); 
    }   

    if(fseek(fp, 0, SEEK_SET) < 0) {
	fprintf(stderr,"line:%d\n",__LINE__);
    }

    if(fwrite(&message, sizeof(struct person), 1, fp) == 0) {
	fprintf(stderr,"line:%d\n",__LINE__);
    }
    fclose(fp);
    return id-1;
	    
}


struct my_friend *read_file_friends(char *filename)	
{
    FILE	*fp;
    struct my_friend *head, *ptemp, *r;

    if((fp = fopen(filename, "rt")) == NULL) {	//只读，打开文件friends, 中存结构体(id ,username)
	fprintf(stderr, "line:%d ", __LINE__);
    }
  
    head = (struct my_friend *)malloc(sizeof(struct my_friend));
    head->next = NULL;
    r = head;

    while(!feof(fp)) {
	
	ptemp = (struct my_friend *)malloc(sizeof(struct my_friend));
	fscanf(fp, "%s", ptemp->Id);

	r->next = ptemp;
	r = ptemp;
    }
    r->next = NULL;
    fclose(fp);
    return head;
   
 }



//改变用户登陆状态
void state_file_friends(struct friends *phead, char *id, char *string)
{
    FILE    *fp;
    struct friends *ptemp;
    
    if((fp = fopen("friends.txt", "wt")) == NULL) {
	fprintf(stderr, "line:%d ", __LINE__);
    }

    for(ptemp = phead->next; ptemp != NULL; ptemp = ptemp->next) {
	if(strcmp(id, ptemp->Id) == 0) {
		strcpy(ptemp->state,string);	
	}
	fprintf(fp, "%s %s %s\n", ptemp->Id, ptemp->username, ptemp->state);
    }
    fclose(fp);
}

//将所有注册账号存入
void write_file_friends(char *user_id, char *username)
{
    FILE	*fp;

    if((fp = fopen("friends.txt", "at")) == NULL) {
	fprintf(stderr, "line:%d ", __LINE__);
    }
    
    fprintf(fp, "%s %s %s\n", user_id, username, "off");
    fclose(fp);
}



/*显示在线用户*/
void show_online(struct chat buf)
{
	char  send_buf[800];
	struct sockfd *ptemp;
	int	  from_connfd;

	from_connfd = search_person(buf.from_id);
	for(ptemp = phead; ptemp != NULL; ptemp = ptemp -> next)
	{
		strcpy(buf.to_id, ptemp -> Id);		
	memset(&send_buf, 0, sizeof(send_buf));
	memcpy(send_buf, &buf, sizeof(struct chat));
	if(send(from_connfd, send_buf, sizeof(send_buf), 0) < 0) {
		fprintf(stderr, "line:%d\n", __LINE__);
	}
	}
}

/*显示自己好友*/
void show_friend_menus(struct chat buf)
{
	char   send_buf[800];
	struct my_friend  *ptemp;
	int  from_connfd;
	struct my_friend *head;
	int    frien = 0;

	head = read_file_friends(buf.from_id);

	from_connfd = search_person(buf.from_id);
	for(ptemp = head -> next; ptemp -> next != NULL; ptemp = ptemp -> next) 
	{
		strcpy(buf.to_id, ptemp->Id);
		memset(&send_buf, 0, sizeof(send_buf));
		memcpy(send_buf, &buf, sizeof(send_buf));
		send(from_connfd, send_buf, sizeof(send_buf), 0);
		frien = 1;
	}

	if(frien == 0) {
		buf.cmd = 't';
		memset(&send_buf, 0,sizeof(send_buf));
		memcpy(send_buf, &buf, sizeof(struct chat));
		send(from_connfd, send_buf, sizeof(send_buf), 0);
	}



}
