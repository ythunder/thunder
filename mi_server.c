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
	struct sockfd  *r;
	r = phead;

	if(phead = temp) {
		if(phead -> next != NULL) {
			free(temp);
			return NULL;
		}
	} else {
		while(r != NULL) {
			if(r->next == temp) {
				r -> next = temp -> next;
				free(temp);
				break;
			}
			r = r->next;
		}
	}
	return phead;
}


/*查找*/
int search_person(char *user_id)
{
	struct sockfd *ptemp;
	int		conn_fd = 0;

	ptemp = phead;
	while(ptemp != NULL) {
		if(strcmp(ptemp -> Id, user_id) == 0) {
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
struct friends *read_file_friends();

//改变用户登陆状态
void state_file_friends(struct friends *phead, char *id, char *string);

/*注册用户写入文件*/
void write_file_friends(char *user_id, char *username);

/*将在线名单送给客户*/
void show_online(struct chat buf);



void main()
{
    int optval;
    int   		sock_fd;
    struct sockaddr_in  cli_addr, serv_addr;
    
    if( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		fprintf(stderr, "line: %d\n", __LINE__);
    }

	optval = 1;

	if(setsockopt(sock_fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&optval,  sizeof(int)) < 0) {
		fprintf(stderr, "line:%d ",__LINE__);
	}

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7777);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     
   
    if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in)) < 0) {
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

    cli_len = sizeof(struct sockaddr_in);
    while(1) {
    conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
    
	if(conn_fd < 0) {
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
	
	if(setsockopt(conn_fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&optval,  sizeof(int)) < 0) {
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
					memset(recv_buf, 0, sizeof(recv_buf));
		while(1) {
					if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0) {
					fprintf(stderr, "line:%d ",__LINE__);
					}

					printf("-----%s----\n",recv_buf);
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
					}		
					}}
			   break;
				
		case 3: close(conn_fd);
				break;
	}}


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
		printf("已发出");
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
		if(to_fd != 0) {
			i++;
			send(to_fd, send_buf, sizeof(send_buf), 0);
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
	FILE	*fp1, *fp2;
	int		from_connfd;
	char	send_buf[800];


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
				printf("打开文件%s失败!", buf.from_id);
			}
			fprintf(fp1,"%s\n", buf.to_id);
			fclose(fp1);
			fp2 = fopen(buf.to_id, "a+");
			if(fp2 == NULL) {
				printf("打开文件%s失败!", buf.from_id);
			}
			fprintf(fp2,"%s\n", buf.to_id);
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

	head = read_file_friend(buf.from_id);
	head2 = read_file_friend(buf.to_id);

	if((fp1 = fopen(buf.from_id, "w")) == NULL) {
		fprintf(stderr, "line:%d ", __LINE__);
	}
	for(ptemp = head; ptemp != NULL; ptemp = ptemp->next) {
		if(strcmp(ptemp->Id, buf.to_id) != 0 ) {
			fprintf(fp1, "%s\n", ptemp->Id);
		}
	}
	fclose(fp1);

	if((fp2 = fopen(buf.to_id, "w")) == NULL) {
		fprintf(stderr, "line:%d ",__LINE__);
	}
	for(ptemp = head2; ptemp != NULL; ptemp = ptemp->next) {
		if(strcmp(ptemp->Id, buf.from_id) != 0) {
			fprintf(fp2, "%s\n", ptemp->Id);
		}
	}
	fclose(fp2);
}




/*读出myfriend中的文件，返回头指针*/
struct my_friend *read_file_friend(char *filename)	
{
    FILE	*fp;
    struct my_friend  *head, *ptemp, *r;
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
    FILE	  *fp;	
    struct person message;

    if(recv(conn_fd, &recv_buf, sizeof(recv_buf), 0) < 0) { 
       fprintf(stderr, "line:%d\n",__LINE__);
    }
   printf("%s ", recv_buf);
	
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
      
    write_file_friends(message.Id, message.username); 
    	
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

/*读出friends中的文件，返回头指针
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
	fscanf(fp, "%s %s %s", ptemp->Id, ptemp->username, ptemp->state);

	r->next = ptemp;
	r = ptemp;
    }
    r->next = NULL;
    fclose(fp);
    return head;
   
 }

*/

/*改变用户登陆状态*/
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

/*将所有注册账号存入*/
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
