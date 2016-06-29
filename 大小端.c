/*************************************************************************
	> File Name: 大小端.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2016年06月26日 星期日 09时24分54秒
 ************************************************************************/

#include<stdio.h>

void byteorder() {

	union {
		short value;
		char union_bytes[sizeof(short)];

	}text;

	text.value=0x0102;

	if((text.union_bytes[0] == 1) && (text.union_bytes[1] == 2)) {
		printf ("big endian!\n");
	} else if((text.union_bytes[0] == 2) && (text.union_bytes[1] == 1)) {
		printf ("little endian!\n");
	} else {
		printf ("unknown...\n");
	}
}


int main()
{
	byteorder();
	return 0;
}
