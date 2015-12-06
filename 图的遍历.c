/*************************************************************************
	> File Name: 图的遍历.c
	> Author: yunting
	> Mail:1368282581@qq.com 
	> Created Time: 2015年12月04日 星期五 10时27分23秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define MAXVEX 20


typedef struct
{
    int   arcs[MAXVEX][MAXVEX];//边信息
    char  vex[MAXVEX];//顶点信息
    int   vexnum;//顶点数目
    int   arcnum;//边数目
} AdjMatrix;


void Create(AdjMatrix  *G)
{
    int i, j, a, b, c;
    printf ("please input the arcs's number :");
    scanf ("%d", &G -> arcnum);

    printf ("please input the vex's number :");
    scanf ("%d", &G -> vexnum);

    getchar() ;
    printf ("please input the all vexs:");
    for (i = 0; i < G -> vexnum; i++) {
       scanf ("%c", &G -> vex[i]);
    getchar() ;
    }
    
    for (i = 0; i < G -> vexnum; i++) {
        for (j = 0; j < G -> vexnum; j++) {
            G -> arcs[i][j] = 0;
        }
    } 

    for (i = 0; i < G -> arcnum; i++) {
        getchar();
        printf ("please input the vexs and arcs:");
        scanf ("%d %d %d",&a, &b, &c);
        G -> arcs[a][b] = c;
    }
}

int main()
{
    int i, j;
    AdjMatrix *G;

    G = (AdjMatrix *)malloc (sizeof (AdjMatrix));
    Create (G);

    for(i = 0; i < G ->vexnum; i++) {
        for (j = 0; j < G -> vexnum; j++) {
            printf ("%5d", G -> arcs[i][j]);
        }
        printf ("\n");
    }
    return 0;
}

