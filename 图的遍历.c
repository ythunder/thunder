/*************************************************************************
	> File Name: 图的遍历.c
	> Author: yunting
	> Mail:1368282581@qq.com 
	> Created Time: 2015年12月04日 星期五 10时27分23秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define MAXVEX 20

int visited[MAXVEX] = {0};  //访问标志数组

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

/*
int i = 0;
void DFS (AdjMatrix *G, int c)
{

    int j, k = 0, m;
    
    for (m = 0; m < 5; m++) {
        printf ("%c  ", visited[m]);
    }
    printf ("\n\n\n");
    visited[i] = G -> vex[c];
    G -> vex[c] = 0;
    i++;

    if (i == G -> vexnum) {
        return ;
    }
    for (j = 0; j < G -> vexnum ; j++) {
        if ((G -> arcs[c][j] != 0) && (G->vex[j] != 0)) {
                G -> arcs[c][j] = 0;
                k = j;
                break;
        }
    }
    if (k == 0) {
        G -> vex[c-1] = visited[i-1];
        visited[i] = 0;
        i--;
        DFS(G, c-1);
    } else {
    DFS(G, k);
    }
}
*/

int FirstAdjVex(AdjMatrix *G, int v0) 
{
    int i;
    if (v0 != -1) {
        for (i = 0; i < G -> vexnum; i++) {
            if (G -> arcs[v0][i] != 0 && visited[i] != 1) {
                return i;
            }
        }
    }
    return -1;
}

void CountDegree(AdjMatrix *G)
{
    int i, j;
    int count[2][MAXVEX];
    for (i = 0; i < MAXVEX; i++) {
        for (j = 0; j < 2; j++) {
            count[j][i] = 0;
        }
    }
    for (i = 0; i < G->vexnum; i++) {
        for (j = 0; j < G->vexnum; j++) {
            if (G->arcs[i][j] != 0) {
                count[0][i]++;
            }
            if (G->arcs[j][i] != 0) {
                count[1][i]++;
            }
        }
    }
    for (i = 0; i < G->vexnum; i++) {
        printf ("\nvex:%c: chudu :%d  rudu:%d\n", G->vex[i], count[0][i], count[1][i]);
    }
    printf ("\n");
}

void DFS(AdjMatrix *G, int v0)
{
    int w;
    printf ("%c",G->vex[v0]);
    visited[v0] = 1;
    w = FirstAdjVex(G, v0);
    while(w != -1) {
        if (!visited[w]) DFS(G, w);
        w =FirstAdjVex (G, v0);
    }
}

int main()
{
    int i, j;
    AdjMatrix *G;

    G = (AdjMatrix *)malloc (sizeof (AdjMatrix));
    Create (G);
    DFS(G, 0);

    CountDegree(G);
    return 0;
}

