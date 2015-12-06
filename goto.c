/*************************************************************************
	> File Name: goto.c
	> Author: yunting
	> Mail:1368282581@qq.com 
	> Created Time: 2015年12月06日 星期日 15时54分22秒
 ************************************************************************/

#include<stdio.h>

int main(void)
{
    char ch;

    while ((ch = getchar()) != '#') {
        if (ch == '\n') {
            continue;
       }
        printf ("Step 1\n");
        if (ch == 'c') {
            continue;
        } else if (ch == 'b') {
            break;
        } else if (ch == 'g') {
            goto laststep;
        }
        printf ("Step 2\n");
        laststep:  printf ("Step 3\n");
    }
    printf ("Done\n");
    return 0;
}
