#include <stdio.h>
#include <stdlib.h>

typedef struct Item{

  double  coef;
  int     expn;
  struct Item *next;

}Item,*Polyn;


#define CreateItem(p) p=(Item *)malloc(sizeof(Item));
#define DeleteItem(p) free((void *)p);

    

int InsertLocate(Polyn h,int expn,Item **p)
{ 
Item *pre,*q;
    pre=h;
    q=h->next;
    while(q != NULL && q->expn < expn) 
      { pre=q;
        q=q->next;
    }
    if(!q)
      { *p=pre;
    return(1);
    }
    else if(q->expn==expn)
      { *p=q;
    return(0);
    }
    else
      { *p=pre;
    return(-1);
        }
}

void insert(Item *pre,Item *p)
{
    p->next=pre->next;
    pre->next=p;
    }



Polyn Input()
{
  double  coef;
  int     expn;
  Item *h, *p, *pp;
  h=(Item *)malloc(sizeof(Item));
  h->next=NULL;
  printf("请输入系数指数，若想结束输入，将系数置为0\n");
  while(1)
  {
    scanf("%lf %d",&coef,&expn); 
    if(coef==0) break;        
    if(InsertLocate(h,expn,&pp))
	{
	   p=(Item *)malloc(sizeof(Item));
       p->coef=coef;
       p->expn=expn;
       insert(pp,p);
      }
     }
  return h;
  }



void Output(Polyn h,char *title)
{
  int flag=1;
  Item *p=h->next;
  printf("%s=",title);
  while(p)
  {
    
   if(p->expn==0)  printf("%.2lf",p->coef);
   else printf("%.2lfx^%d",p->coef,p->expn);
   if(p -> next !=	NULL) {
		printf("+");
   }
   
    p=p->next;
    }
 printf("\n");
 }




int ItemComp(Item x,Item y)
{ if(x.expn<y.expn)
     return(-1);
  else if(x.expn==y.expn)
     return(0);
  else return(1);
}




Polyn AddPolyn(Polyn h1,Polyn h2) 
{
  Item *head,*last,*pa=h1->next,*pb=h2->next,*s,*s0;
  double coef;
  head=(Item *)malloc(sizeof(Item));
  last=head;
  last->next=NULL;
  while(pa&&pb)
  {
  s=(Item *)malloc(sizeof(Item));
  last->next=s;
  s->next=NULL;
  switch(ItemComp(*pa,*pb))
  {
  case -1:
  s->coef=pa->coef;
  s->expn=pa->expn;
  pa=pa->next;   
  last=s;   
  break;
      case  0:
  coef=pa->coef+pb->coef;  
  s->expn=pa->expn;
 if(coef==0)
  {
  DeleteItem(s);
  last->next=NULL;
  }
 else
 s->coef=coef;
  pa=pa->next;   
  pb=pb->next;
  if(coef!=0)
    last=s;
      break;
  case 1:
  s->coef=pb->coef;
  s->expn=pb->expn;
  pb=pb->next;   
  last=s;
      break;
  }
  }
  if(!pa&&pb)
  while(pb)
  {
   s0 = (Item *)malloc(sizeof(Item));
      last->next=s0;
      s0->next=NULL;
  s0->coef=pb->coef;
  s0->expn=pb->expn;
          last=s0;
  pb=pb->next;
  }
  else if(pa&&!pb)
  while(pa)
  {
      s0=(Item *)malloc(sizeof(Item));
      last->next=s0;
      s0->next=NULL;
  s0->coef=pa->coef;
  s0->expn=pa->expn;
          last=s0;
  pa=pa->next;
  }
    last->next=NULL;
    return head;
    }

Polyn SubtractPolyn(Polyn h1,Polyn h2) 
{ 
 
  Item *head,*last,*pa=h1->next,*pb=h2->next,*s,*s0;
  double coef;
  head=(Item *)malloc(sizeof(Item));
  last=head;
  last->next=NULL;
  while(pa&&pb)
  {
  s=(Item *)malloc(sizeof(Item));
  last->next=s;
  s->next=NULL;
  switch(ItemComp(*pa,*pb))
  {
  case -1:
  s->coef=pa->coef;
  s->expn=pa->expn;
  pa=pa->next;   
  last=s;   
  break;
      case  0:
  coef=pa->coef-pb->coef;  
  s->expn=pa->expn;
  if(coef==0)
  {
  DeleteItem(s);
  last->next=NULL;
  }
  else
  s->coef=coef;
  pa=pa->next;   
  pb=pb->next;
  if(coef!=0)
  last=s;
      break;
  case 1:
  s->coef=pb->coef;
  s->expn=pb->expn;
  pb=pb->next;   
  last=s;
      break;
  }
  }
  if(!pa&&pb)
  while(pb)
  {
  s0 = (Item *)malloc(sizeof(Item));
      last->next=s0;
      s0->next=NULL;
  s0->coef=pb->coef;
  s0->expn=pb->expn;
          last=s0;
  pb=pb->next;
  }
  else if(pa&&!pb)
  while(pa)
  {
  s0=(Item *)malloc(sizeof(Item));
      last->next=s0;
      s0->next=NULL;
  s0->coef=pa->coef;
  s0->expn=pa->expn;
          last=s0;
  pa=pa->next;
  }
    last->next=NULL;
    return head;
    }



Polyn MultPolyn(Polyn h1,Polyn h2)  
{ int m = 0;
  Item *head,*head2,*pa=h1->next,*pb=h2->next,*s,*pp,*last,*last2,*Q;
  head=(Item *)malloc(sizeof(Item));
  head->next=NULL;
  last=head;
  head2=(Item *)malloc(sizeof(Item));
  head2->next=NULL;
  last2=head2;

  while(pa)
  {
  pp=pb;
      while(pp)
  {
     if(m==0)
 {
  s=(Item *)malloc(sizeof(Item));
     last->next=s;
 s->next=NULL;
 s->coef=pa->coef*pp->coef;   
     s->expn=pa->expn+pp->expn;
     pp=pp->next;   
     last=s;
 }
 else
 { 
  s=(Item *)malloc(sizeof(Item));
     last2->next=s;
 s->next=NULL;
 s->coef=pa->coef*pp->coef;   
     s->expn=pa->expn+pp->expn;
     pp=pp->next;   
     last2=s;
 }  
  }
     if(m!=0)
 {
       head=AddPolyn(head,head2);
       while(head2)
   {
   Q=head2;
   head2=head2->next;
   DeleteItem(Q);          
   }
  head2=(Item *)malloc(sizeof(Item));
           head2->next=NULL;
           last2=head2;    
 }
     pa=pa->next;
 m=1;
  }
    return head;
    }


//菜单，获得用户选择功能
int menu(void)
{ 
int num;
    
    system("cls");
    printf("\t1、创建多项式P(X)和Q(X)\n");
    printf("\t2、两式相加\n");
    printf("\t3、两式相减\n");
    printf("\t4、退出\n");
    printf("请选择：");
      do{
  scanf("%d",&num);
  }while(num<1 || num>11);
    return(num);
 }




int PolynNotEmpty(Polyn h,char *p)
  { if(h==NULL)
      { printf("%s is not exist!\n",p);
        getchar();return 0;
    }
     else return(1);
     }


void main()
{ 
	int num;
	Polyn  h1=NULL; //p(x)
	Polyn  h2=NULL; //Q(x)
	Polyn  h3=NULL; //P(x)+Q(x)
	Polyn  h4=NULL; //P(x)-Q(x)
	Polyn  h5=NULL; //P(x)*Q(x)
  while(1)
    { num=menu();
      getchar();
      switch(num)
       {

  case 1: 
		printf("请输入P(x)");
		h1 = Input();
		Output(h1,"P(x)");
		printf("请输入Q(X):");
		h2 = Input();
	    Output(h2,"Q(x)");
		printf("按任意键继续...");
		getchar();
	    getchar();
		break;
 
 case 2:  

    if(h1 != NULL && h2 != NULL) 
	{
		h3=AddPolyn(h1,h2);
        Output(h3,"P(x)+Q(X)");
        printf("按任意键继续...\n");
        getchar();
    }
    break;


 case 3:  
     
 if(h1 != NULL && h2 != NULL)
 { h4=SubtractPolyn(h1,h2);
        Output(h4,"P(x)-Q(x)");
                printf("按任意键继续...\n");
getchar();
}
    break;

 case 4: 
	exit(1);
   break;
}
}  getchar();
  getchar();
}

