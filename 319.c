/*************************************************************************
	> File Name: 319.c
	> Author: yunting
	> Mail:1368282581@qq.com 
	> Created Time: 2015年12月03日 星期四 14时01分41秒
 ************************************************************************/

#include<stdio.h>

int main ()
{
    int j, year;
    int day = 0;
    int goday = 0;
    int m, cha;
    int month[12] = {1,2,3,4,5,6,7,8,9,10,11,12};
    int eyem[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    printf ("please input the year :n\n");
    scanf ("%d", &year);

    if ((year % 4) == 0) {
        eyem[1] = 29; 
    } else {
        eyem[1] = 28;
    }
    cha = year - 2001;

    if (cha < 4) {
        day = day + 365 * cha;
    } else if ( cha < 8) {
        day = day + 365 * cha + 1;
    } else {
        day = day + 365 * cha + 2;
    }

    for(m = 0; m < 12; m++) {
        printf ("\n\n\n%d月\n", month[m]);  
        printf ("一\t二\t三\t四\t五\t六\t日\n");
        
            for (j = 0; j < (day % 7); j++) 
                printf ("  \t");
    
        for (j = 1; j <= eyem[m]; j++) {
            day++;
            printf("%2d\t", j);
            if ((day % 7 == 0) && (day != 0)) printf ("\n");
                }
    }
    return 0;
}

