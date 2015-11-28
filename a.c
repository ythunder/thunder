/*************************************************************************
	> File Name: a.c
	> Author: yunting
	> Mail:1368282581@qq.com 
	> Created Time: 2015年11月28日 星期六 19时11分10秒
 ************************************************************************/

#include<stdio.h>

int main()
{
    int n, i;
    int a[1000];
    int sis = 0, cat = 0;

    printf ("请输入pk总场数：");
    scanf ("%d", &n);
    printf ("请输入每场pk结果：");
    for (i = 0; i < n; i++) {
        scanf ("%d", &a[i]);
    }

    for (i = 0; i < n; i++) {
        if (a[i] == 1)  sis++;
        if (a[i] == 0)  cat++;
        if (sis == 10) {
            printf ("%d : %d\n", sis, cat);
            sis = 0; cat = 0;
        } 
        if (cat == 10) {
            printf ("%d : %d\n",sis, cat);
            sis = 0; cat = 0;
        }   
    }
    printf ("%d : %d\n", sis, cat);


    return 0;

}
