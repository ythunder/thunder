#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

/*自定义错误处理函数*/
void my_err(const char *err_string , int line)
{
	fprintf(stderr , "line:%d" , line);
	perror(err_string);
	exit(1);
}

/*得到文件头到文件指针所指数据长度*/
int my_read(int fd)    //fd 为文件描述符
{
	int	len;
	int	ret;
	int 	i;
	char	read_buf[64];


	if((lseek(fd, 0, SEEK_END)) == -1){   //将文件指针移到文件尾
	
		my_err("lseek", __LINE__);
	}
	if((len = lseek(fd, 0, SEEK_CUR)) == -1){   //lseek获取文件当前指针（文件尾处）相对于文件开头的偏离，len表示文件长度

		my_err("lseek",__LINE__);
	}
	if((lseek(fd, 0, SEEK_SET)) == -1){   //指针移到文件头

		my_err("lseek", __LINE__);
	}

		printf("len:%d", len);

	if((ret = read(fd, read_buf, len)) < 0){   //读取文件所有数据

		my_err("read", __LINE__);
	}

	for(i=0; i<len; i++){      //输出文件头中所有的字符
	
		printf("%c", read_buf[i]);
	}
	printf("\n");
	
	printf("%d\n", ret);
	return ret;    //返回所读取数据字节数
	
}

int main()
{
	int 	fd;
	char	write_buf[32] = "hello  world!";

	if(fd = creat("example_63.c", S_IRUSR) == -1){
//umask(0);
//	if((fd = open("example_63.c", O_RDWR|O_CREAT|O_TRUNC, S_IRWXU)) == -1){
		
		my_err("open" , __LINE__);
	} else {
	    printf("create file success");
	}

	if(write(fd, write_buf, strlen(write_buf)) != strlen(write_buf)){  //将write_buf 中的数据写到fd所指文件中，返回字节数值

		my_err("write", __LINE__);
	}

	my_read(fd);   //得到文件example_63.c 长度

	printf("/*-------------------------*/\n");
	
	if(lseek(fd, 10, SEEK_END) == -1) {    //使文件指针移动到距文件结尾处距离为10的地方

		my_err("lseek", __LINE__);
	}

	if(write(fd, write_buf, strlen(write_buf)) != strlen(write_buf)) {  //写入，因为问价指针在倒10处，存入数据超范围但不出错，读取长度值变小 
	
		my_err("write", __LINE__);
	}

	my_read(fd);

	close(fd);
	return 0;
}
