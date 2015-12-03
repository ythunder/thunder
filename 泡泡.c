/*************************************************************************
	> File Name: 泡泡.c
	> Author: yunting
	> Mail: 1368282581@qq.com 
	> Created Time: 2015年11月10日 星期二 11时26分02秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 20

int main()
{
	int a[N];
	int b[N];
	int i, j, s, k,number;

	memset (b, -1, sizeof(b));
	printf ("please input the number you want to input :");
	scanf ("%d", &number);			//数的数量
	while (number > N || number <= 0) {
		printf ("error, please try again !");
		scanf ("%d", &number);
	}

	for (i = 0; i < number; i++) {
		scanf ("%d",&a[i]);
	}
	for (j = 0; j < number; j++) {
		s = 0;
		for (k = 0; k < number; k++) {
			if (k != j) {
				if (( a[j] - a[k] ) > 0 ) {
					s ++;
				}
			}
		}
	while (b [s] != -1) {
		s ++;
	}
	b[s] = a[j];
	}
	for (i = 0; i < number; i++) {
		printf ("%5d", b[i]);
	}
	return 0;
}


