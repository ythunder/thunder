/*************************************************************************
	> File Name: 小马快跑.c
	> Author: yunting
	> Mail: 1368282581@qq.com 
	> Created Time: 2015年10月09日 星期五 14时26分58秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define N 8//棋盘大小
#define Stack_SIZE 100  //存储空间初始分配量
#define STACKINCREMENT 10 //存储空间分配增量
	
int weight[N][N];
typedef struct 
{
	int		x;
	int		y;
}PosType;

typedef struct 
{
	int	    ord;
	PosType	seat;
	int		di;
}ElemType;

typedef struct Stacknode
{
	ElemType data;
	struct Stacknode *next;
}StackNode;

typedef struct
{
	StackNode *top;	
}LinkStack;


//初始化一个空栈
Set_Empty_Stack(LinkStack *S)
{	
	S -> top = NULL;	
}


//判断栈空操作
int Is_Empty_Stack(LinkStack *S)
{
	if(S -> top == NULL) {
		return 1;
	} else {
		return 0;	
	}
}


//进栈操作
int Push(LinkStack *S, ElemType *node)
{
	StackNode *p;

	p = (StackNode *) malloc (sizeof(StackNode));

	if(p == NULL) {
		fprintf(stderr, "line:%d\n", __LINE__);
		return 0;
	} else {
		p->data = *node;
		p->next = S -> top;
		S -> top = p;
		return 1;
	}
	
}

//出栈操作
int Pop(LinkStack *S, ElemType *node)
{
	StackNode *p;

	if(Is_Empty_Stack(S)) {
		printf("the stack is empty!pop\n");
		return 0;
	} else {
		p = S -> top;
		*node = p -> data;
		S -> top = p -> next;
		free(p);
		return 1;
	}
}

//取栈顶的值
int Get_Stack_Top(LinkStack *S, ElemType *node)
{
	if(Is_Empty_Stack(S)) {
		printf("the stack is empty!get_stack_top\n");
		return 0;
	} else {
		*node = S -> top -> data;
		return 1;
	}
}


//8个候选方向
PosType NextPos(PosType curpos, int direction)  
{
	switch(direction)
	{
	case 1: 
		curpos.x += 1; curpos.y -= 2; break;
	case 2:
		curpos.x += 2; curpos.y -= 1; break;
	case 3:
		curpos.x += 2; curpos.y += 1; break;
	case 4:
		curpos.x += 1; curpos.y += 2; break;
	case 5:
		curpos.x -= 1; curpos.y += 2; break;
	case 6:
		curpos.x -= 2; curpos.y += 1; break;
	case 7:
		curpos.x -= 2; curpos.y -= 1; break;
	case 8:
		curpos.x -= 1; curpos.y -= 2; break;
	}
	return curpos;
}


//判断位置是否合法
int Can_Walk(PosType curpos)
{
	if(curpos.x<0 || curpos.x>(N-1) || curpos.y<0 || curpos.y>(N-1)) {
		return 0;
	}
	return 1;	
}


PosType Get_Start_Seat()
{
	PosType seat;

	printf("please input the seat:\n\tX:");
	scanf("%d", &seat.x);
	printf("\tY:");
	scanf("%d", &seat.y);
	while(Can_Walk(seat) == 0) {
		printf("\nthe seat is error ! please input again\n");
		printf("\tX:");
		scanf("%d",&seat.x);
		printf("\n\tY:");
		scanf("%d", &seat.y);
	} 
	return seat;
	
}

//计算某点权值
int Seat_Weight(PosType curpos) 
{	
	int i, qw[N][N];
	PosType next;

	qw[curpos.x][curpos.y] = 0;
	for(i=0; i<8; i++) {
		next = NextPos(curpos, i+1);
		if(weight[next.x][next.y] == 0 && Can_Walk(next) == 1) {
			qw[curpos.x][curpos.y]++;
		}
	}
	return qw[curpos.x][curpos.y];
}

int Seat_Min(int *a)
{
	int i, min = 9;
	int di = 9;
	for(i=0; i<8; i++) {
		if(a[i] < min && a[i] > 0) {
			min = a[i];
			di = i;
		}
	}
	if(di == 9) {
		return -1;
	} else {
	return di; 
	}
}


int main()
{
	LinkStack  *S = NULL;  	PosType start, next, st;	int g, i, j,horsestep = 0, test;	ElemType  *p, *ptemp, *top, *a, *b, *c, *d;
	int step[N][N];	int ords = 0; 	int  ff[8];	int  min_weight;	int  quan[N][N]; int  flag=0 ,off;
	memset(step, 0, sizeof(step));

	S = (LinkStack *) malloc (sizeof(LinkStack));
	Set_Empty_Stack(S);		//初始化一个空栈
	start = Get_Start_Seat();   //得到起始点

	p = (ElemType *) malloc (sizeof(ElemType));
	p->ord = ++horsestep;
	p->seat.x = start.x;
	p->seat.y = start.y;
	p->di = 0;
	if(Push(S, p) == 0) {								//将初始位置压入栈
		fprintf(stderr, "line:%d\n", __LINE__);
	};

	weight[p->seat.x][p->seat.y] = 1;					//weight 表示该点走没走过
	do{ 
		flag = 0;
		memset(ff, 0, sizeof(ff));
		for(i=0; i<8; i++) {
			next = NextPos(start, i+1);
			if(Can_Walk(next) == 1 && weight[next.x][next.y] == 0) {
				quan[next.x][next.y] = Seat_Weight(next);
				ff[i] = quan[next.x][next.y];
				flag = 1;
				off = i;
			}	
		}
		min_weight = Seat_Min(ff);
		if(horsestep == N*N-1 && flag==1) {
			next = NextPos(start, i+1);
		} else {
			next = NextPos(start, min_weight + 1);
		}
		while(min_weight < 0 && flag ==0) {
			c = (ElemType *) malloc (sizeof(ElemType));
			if(Pop(S, c) == 0 ) {							//取栈顶a
				fprintf(stderr, "line:%d\n", __LINE__);
			}
			horsestep--;
			weight[c->seat.x][c->seat.y] = 0;
			d = (ElemType *) malloc (sizeof(ElemType));
			if(Get_Stack_Top(S, d) == 0) {					//得到栈顶元素
				fprintf(stderr, "line:%d\n", __LINE__);
			}
			st.x = d -> seat.x;								//栈顶元素
			st.y = d -> seat.y;				
			for(i = 0; i < 8; i++) {
				next = NextPos(st, i+1);
				if(Can_Walk(next) == 1 && weight[next.x][next.y] == 0) {
				quan[next.x][next.y] = Seat_Weight(next);
				ff[i] = quan[next.x][next.y];
			}
			}	min_weight = Seat_Min(ff);
					next = NextPos(st, min_weight + 1);

			free(c);free(d);
			}
			ptemp = (ElemType *) malloc (sizeof(ElemType));
			ptemp -> ord = ++horsestep;
			ptemp -> seat.x = next.x;
			ptemp -> seat.y = next.y;
			ptemp -> di = min_weight;
		
			if(Push(S, ptemp) == 0) {
				fprintf(stderr, "line:%d\n", __LINE__);
			}
			weight[ptemp->seat.x][ptemp->seat.y] = 1;
		
			top = (ElemType *) malloc (sizeof(ElemType));	//为指针top分配空间
			Get_Stack_Top(S, top);							//取得栈顶值存入top
			start.x = top->seat.x;
			start.y = top->seat.y;

			free(top);
			free(ptemp);
	} while(horsestep < N*N);
	printf("\n");
	for(i=0; i<horsestep; i++) {
		a = (ElemType *) malloc (sizeof(ElemType));
		Pop(S, a);
		weight[a->seat.x][a->seat.y] = a->ord;
		free(a);
	}
	for(i=0; i<N; i++) {
		for(j=0; j<N; j++) {
			printf("%5d", weight[i][j]);
		}
		printf("\n");
	}
}
