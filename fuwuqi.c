#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

/*用户的ID,账户名,密码,存在number.txt中*/
struct person
{
    char	Id[8];
    char	username[32];
    char	passwd[32];
};

/*将从客户端收到的账号密码及服务器分配的ID存入结构体*/
void jiexi(char *recv_buf, int id,struct person *message);

/*系统分配ID*/
int fen_id(void);

/*负责与客户端的通信,将结构体存入文件number.txt*/
void zhuce(int conn_fd);

/*解析传来的账户密码包，处理后返回id校验结果*/
void denglu(int conn_fd);

/*根据用户输入的ID密码在文件中查找校验，返回相应值*/
int search(char Id[8], char msg[32]);

int main()
{    
    int   		sock_fd, conn_fd;
    socklen_t   	cli_len;
    char		choice;
    struct sockaddr_in  cli_addr, serv_addr;
    pid_t 		  pid;
    char		  buf[3];
    int			c;
    
    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9523);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     
  
    if( (sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	fprintf(stderr, "line: %d \n", __LINE__);
    }

   
    if(bind(sock_fd, (struct sockaddr *)&serv_addr, 
	sizeof(struct sockaddr_in)) < 0) {
	fprintf(stderr, "line: %d \n",__LINE__);
    }


    if(listen(sock_fd, 13) < 0) {
	fprintf(stderr, "line:%d \n",__LINE__);
    }
/**********************************************************************************/
    cli_len = sizeof(struct sockaddr_in);
  
    while(1) {
    conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &cli_len);
    if(conn_fd < 0) {
	fprintf(stderr, "line: %d\n",__LINE__);
    }
    printf("accept a new client ,ip: %s\n", inet_ntoa(cli_addr.sin_addr));
   
    pid = fork();
    if(pid == 0) {
	if(recv(conn_fd, buf, sizeof(buf), 0) < 0) {
	    fprintf(stderr,"line:%d ",__LINE__);
	}
	c = atoi(buf);
	printf("%d\n", c);
	switch(c) {
	    case 1: zhuce(conn_fd);
		    break;
            case 2: denglu(conn_fd);
		    break;
	}
}wait(NULL);
}}

void denglu(int conn_fd)
{
    char  recv_buf[128];
    int   len, i, t, result;
    char  use_id[8], use_msg[32], use_name[32];
    char  *string1 = "登录失败";	
    char  *string2 = "账户或者密码不正确";
    char  *string3 = "登录成功";
    
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

     memset(&use_id, 0, sizeof(use_id));
     for(i=0; i<t; i++) 
        use_id[i] = recv_buf[i];  
	use_id[t] = '\0';

    memset(&use_msg, 0, sizeof(use_msg));
    for(i=t+1; i<len; i++)
	use_msg[i-t-1] = recv_buf[i];
	use_msg[len-t-1] = '\0';
 
    result = search(use_id, use_msg);
    switch(result) {

	case -1: 
		if(send(conn_fd, string1, strlen(string1), 0) < 0) {
		    fprintf(stderr, "line:%d\n", __LINE__);
		}
		break;

	case 0: 
		if(send(conn_fd, string2, strlen(string2), 0) < 0) {
		    fprintf(stderr, "line:%d\n", __LINE__);
		}
		break;

	case 1: 
		if(send(conn_fd, string3, strlen(string3), 0) < 0) {
		    fprintf(stderr, "line:%d\n", __LINE__);
		}
		break;
    }	
}

int search(char Id[8], char msg[32])
{
    struct person  ptemp;
    FILE 	   *fp = NULL;
    int		   id,id_max;
    int		    count;
    memset(&ptemp, 0, sizeof(struct person));
    id = atoi(Id);
    if((fp = fopen("number.txt", "rb+")) == NULL) {
        fprintf(stderr, "line:%d ", __LINE__);
	return -1;
    } 
    if(fread(&ptemp, sizeof(struct person), 1, fp) <= 0) {
	fprintf(stderr, "line:%d ", __LINE__); 
	return -1;
    }
    id_max = atoi(ptemp.Id);
    if(id > id_max || id < 10000) {
	return 0;
    }
    else {
	count =  id - 9999;
	if(fseek(fp, count*sizeof(struct person), SEEK_SET) < 0) {
	    fprintf(stderr, "line:%d ",__LINE__);
	    return -1;
	}
	if(fread(&ptemp, sizeof(struct person), 1, fp) <= 0) {
	    fprintf(stderr, "line:%d ", __LINE__);
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
    char          *string="登陆成功";

    if(recv(conn_fd, &recv_buf, sizeof(recv_buf), 0) < 0) { 
       fprintf(stderr, "line:%d\n",__LINE__);
    }
	printf ("%s", recv_buf) ;
	
    new_id = fen_id();
    jiexi(recv_buf, new_id, &message);
printf ("asdadasdad:%s,%s\n", message.Id, message.passwd) ; 
    fp = fopen("number.txt", "ab");
    if(fp == NULL) {
        fprintf(stderr, "line:%d\n", __LINE__);
    }
    if(fwrite(&message, sizeof(struct person), 1, fp) == -1 ) {
       fprintf(stderr,"line:%d", __LINE__);
	}
    fclose(fp);
	
    if(send(conn_fd, string, sizeof(string), 0) < 0) {
       fprintf(stderr, "line:%d\n", __LINE__);
    }	
	
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
	printf ("%s,%s,%s\n", message -> Id, message -> username, message -> passwd) ;
}

/*******************************
        服务器分配ID
********************************/
int fen_id(void)
{
    FILE *fp;
    struct person message;
    int id;
    char string[8];

    if( (fp = fopen("number.txt", "rb")) == NULL ) {
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
    printf("id:%d\n", id);
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
    return id;
	    
}

	
		

	
	
