/*************************************************************************
	> File Name: 约瑟夫环.c
	> Author: yunting
	> Mail: 1368282581@qq.com 
	> Created Time: 2015年09月16日 星期三 17时35分54秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define MAX 10

typedef struct NodeType	// 自定义结构体类型
{
	int		id;			//	编号
	int		password;	//  密码
	struct NodeType	 *next;	//	用于指向下一节点的指针
}NodeType;

void CreaList (NodeType **, int);	// 创建单向循环链表

void PrntList(NodeType *);			//打印循环链表

void JosephusOperate(NodeType **, int); //运行"约瑟夫环"问题

int main(void) 
{
	int  person_number = 0;
	int  password = 0;
	NodeType *phead = NULL;
	int  i = 0;

	printf("请输入环中人数(最多输入%d个): ", MAX);
	scanf("%d", &person_number);

	while(person_number <= 0 || person_number > MAX) {
	
		printf("输入数字有误, 请重新输入: ");
		scanf("%d", &person_number);

		i++;				//控制错误输入次数
		if(i == 3) {
			printf("错误输入次数过多,按任意键退出...");
			getchar();
			getchar();
			return 0;
		}
	
	}

	printf("请输入初始密码m: ");
	scanf("%d", &password);

	CreaList(&phead, person_number);		//创建单向链表

	printf("\n------------------打印循环链表--------------------------\n");
	
	PrntList(phead);

	printf("\n------------------打印出对情况---------------------------\n");

	JosephusOperate(&phead, password);			//运行约瑟夫环问题

	return 1;

}

void CreaList(NodeType **pphead, int n) //创建有n个节点的循环链表pphead
{
	int  i = 0;
	int  password = 0;
	NodeType *pnew = NULL;
	NodeType *pcur = NULL;

	for(i = 1; i <= n; i++) {

		printf("输入第%d个人的密码: ", i);
		scanf("%d", &password);

		pnew = (NodeType *) malloc (sizeof(NodeType));   
		pnew -> id = i;
		pnew -> password = password;

		if(*pphead == NULL) {
			
			*pphead = pcur = pnew;
			pcur -> next = *pphead;
		
		}
		else {
			
			pnew -> next = pcur -> next;
			pcur -> next = pnew;
			pcur = pnew;
				
		}
		pnew = NULL;
	}
	printf("完成单向循环链表的创建!\n");	
	}


void PrntList(NodeType *phead)
{
	NodeType  *pcur = phead;

	if(phead != NULL) {    //调用IsEmptyList()函数来判断if语句是否执行, 若phead为空则执行

		printf("\n--ID-- --PASSWORD--\n");
		
		do {
			printf("%3d, %7d\n", pcur -> id, pcur -> password);
			pcur = pcur -> next;
		}while(pcur != phead);
	}
}



int IsEmptyList(NodeType *phead)
{
	if(!phead) {
		
		printf("the list is empty!\n");
		return 1;
	}
	return 0;
	
}



void JosephusOperate(NodeType **pphead, int password)
{
	int count = 0;
	int flag = 1;
	NodeType  *pprv = NULL;
	NodeType  *pcur = NULL;
	NodeType  *pdel = NULL;

	pprv = pcur = *pphead;
	while(pprv -> next != *pphead) {
		pprv = pprv -> next;
	}
	while(flag) {
		for(count = 1; count < password; count++) {
			pprv = pcur;
			pcur  = pcur -> next;
		}
	
		if(pprv == pcur) {
			flag = 0;
		}
		pdel = pcur;
		pprv -> next = pcur -> next;
		pcur = pcur -> next;
		printf("第%d 个人出列, 密码为%d\n", pdel -> id, pdel -> password);
		free(pdel);
	}
	getchar();
	*pphead = NULL;
}
