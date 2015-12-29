/*************************************************************************
	> File Name: test.c
	> Author: yunting
	> Mail:1368282581@qq.com 
	> Created Time: 2015年12月28日 星期一 16时38分17秒
 ************************************************************************/

#include<stdio.h>

int main()
{
    char c1, c2;
    char s1[30], s2[30];
    char same[30];
    int j = 0, i = 0, k  =0; 

    scanf("%s", s1);
    scanf("%s", s2);
    c1 = s1[i++];
    c2 = s2[k++];
    while (c1 == c2) {
        same[j++] = c1;
        c1 = s1[i++];
        c2 = s2[k++];
    }
    printf("the common preix is%s\n", same);
}
