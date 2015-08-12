#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>

struct person
{
    int		Id;
    char	username[32];
    char	passwd[32];
};

void jiexi(char *recv_buf, int id,struct person message);
int fen_id(void);

int main()
{    
    int   		sock_fd, conn_fd;
    socklen_t   	cli_len;
    char		choice;
    struct person	 message;
    struct person 	mess[80];
    FILE 		 *fp;
    char 		   *string = "注册成功";
    struct sockaddr_in  cli_addr, serv_addr;
    pid_t 		  pid;
    int			new_id;
    char 		recv_buf[128];

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9527);
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

	if(recv(conn_fd, &recv_buf, sizeof(recv_buf), 0) < 0) {
	    fprintf(stderr, "line:%d\n",__LINE__);
        }
	
	new_id = fen_id();
	jiexi(recv_buf, new_id, message);	 
	fp = fopen("number", "at");
	if(fp == NULL) {
	    fprintf(stderr, "line:%d\n", __LINE__);
	}
	if(fwrite(&message, sizeof(struct person), 1, fp) == -1 ) {
	    fprintf(stderr,"line:%d", __LINE__);
	}
	fclose(fp);
	
	if(send(conn_fd, string, strlen(string), 0) < 0) {
	    fprintf(stderr, "line:%d\n", __LINE__);
	}	
	
} wait(NULL);
}
}

void jiexi(char *recv_buf, int id,struct person message)
{
    int t;   //记录#所在位置
    int len;  
    int i;
    char username[32],passwd[32];
 
    len = strlen(recv_buf);
    for(i=0;i<len;i++) {
	if(recv_buf[i] != '#') {
	    t = i;
	    break;
	}
    }

    for(i=0; i<t; i++) 		//解析账号
	username[i] = recv_buf[i];
	username[t] = '\0';

    for(i=t+1; i<len; i++)	//解析密码
 	passwd[i] = recv_buf[i];
	passwd[len] = '\0';

    memset(&message, 0, sizeof(struct person));
    message.Id = id;
    strcpy(message.username, username);
    strcpy(message.passwd, passwd);
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

    if( (fp = fopen("number.txt", "rb+")) == NULL ) {
	 fprintf(stderr, "line:%d\n", __LINE__);
    }
     
    memset(&message, 0, sizeof(struct person));
    message.Id == 10000;
	
    if((fwrite(&message, sizeof(struct person), 1, fp)) == -1) {
	fprintf(stderr, "line:%d\n", __LINE__);
    }

    if(fread(&message,sizeof(struct person), 1, fp) == 0) {
	fprintf(stderr,"line:%d\n", __LINE__);
    }
    
    id = message.Id;
    id++;

    itoa(id, string, 5);	    

    if(fseek(fp, 0, SEEK_SET) < 0) {
	fprintf(stderr,"line:%d\n",__LINE__);
    }

    if(fwrite(&message, sizeof(struct person), 1, fp) == 0) {
	fprintf(stderr,"line:%d\n",__LINE__);
    }

    fclose(fp);
    return id;

}	    
	

	
		

	
	
