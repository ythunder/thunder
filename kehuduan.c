#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void my_err(const char *err_string, int line) 
{
    fprintf(stderr, "line:%d ",line);
    perror(err_string);
    exit(1);
}

int main(int argc, char **argv)
{
    int  i;
    int len;
    int  conn_fd;
    char choice;
    char username[32];
    char passwd1[32], passwd2[32];
    char recv_buf[128];   
    struct sockaddr_in serv_addr;
    int  serv_port;

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

	conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(conn_fd < 0) {
	    my_err("socket", __LINE__);
	}

	if(connect(conn_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0) {
	    my_err("connect", __LINE__);
	}

	printf("1.申请 2.登陆");
	choice = getchar();
	while(choice != '1' && choice != 2) {
	    printf("请重新选择");
	    choice = getchar();
	}
	
	switch(choice) {
	    case '1':
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

		strcpy(recv_buf, username);
		len = strlen(recv_buf);
		recv_buf[len] = '#';
		recv_buf[len++] = '\0';
		strcat(recv_buf, passwd1);
		    
		if(send(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0) {
	            fprintf(stderr, "line:%d\n", __LINE__);
		}
		printf("i have send the zhanghumima");
		memset(&recv_buf, 0, sizeof(recv_buf));
		if(recv(conn_fd, recv_buf, sizeof(recv_buf), 0) < 0) {
	  	    fprintf(stderr, "line:%d\n", __LINE__);
  		}
	        for(i=0; i<9;i++) {
		    printf("%c", recv_buf[i]);
		}
		break;
	    case '2': printf(" ");
		      break;
}	}
