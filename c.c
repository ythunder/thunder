#include<stdio.h>
#include<string.h>
#include<stdlib.h>
//#include<conio.h>
typedef struct Telephone
{ 
  char name[50];
  char sex[30];
  char city[30];
  char tel[30];
  char group[30];
  struct Telephone *next;
}TEL;
TEL *head;
void Initiate();
void Menu();
void Create();
void Add();
int Find();
void Alter();
void Delete();
void List(); 
int findname();
int findtel(); 
int main()
{  
  int  choice;
  char  YN;
  Initiate();
  do
  {  
    system("cls");
    Menu();
    scanf("%d",&choice);
    printf("\n");
    switch(choice)
    {  case 1:Create();break;
       case 2:Add();break;
       case 3:Find();break;
       case 4:Alter();break;
       case 5:Delete();break;
       case 6:List();break;
       case 0:printf("************感谢您的使用************\n");
       exit(0);break;
	   default:printf("输入有误!请重新输入\n");break;
    }
    printf("是否继续  Y  or   N?  \n");
    do
    { scanf("%c",&YN);
    }while(YN!='Y'&&YN!='y'&&YN!='N'&&YN!='n');
  }while(YN=='Y'||YN=='y');
}
//初始化
void  Initiate()
{   
  if((head=(TEL *)malloc(sizeof(TEL)))==NULL)  
  exit(1);
  head->next=NULL;
}
//显示菜单
void Menu()
{
	printf("\t\t\t★---您好，欢迎进入学生通讯录信息管理系统！---★\n\n");
	printf("\t================================================================================\n\n");
	printf("\t\t\t--------☆1.创建联系人信息 2.插入联系人信息☆--------\n\n");                   		
	printf("\t\t\t--------☆3.查询联系人信息 4.修改联系人信息☆--------\n\n");
	printf("\t\t\t--------☆5.删除联系人信息 6.打印联系人信息☆--------\n\n");
	printf("\t\t\t--------☆7.保存联系人信息 0.退出通讯录系统☆--------\n\n");
	printf("\t================================================================================\n\n");
    printf("\t\t\t--------☆    请选择（0-7）    ☆--------\n\n");	
}
//创建通讯录
void Create()
{
  TEL *p1[100],*p2;
  int  m,i;
  printf("\t\t\t--------☆请输入要添加的人数：");
  scanf("%d",&m);
  for(i=1;i<=m;i++)
  { p1[i]=(TEL *)malloc(sizeof(TEL));
    printf("\n\t\t\t--------☆请输入第%d条信息!☆--------\n\n",i);
    printf("\t\t\t--------☆请输入联系人姓名：");
    scanf("%s",&p1[i]->name);
    printf("\n\t\t\t--------☆请输入联系人性别：");
    scanf("%s",&p1[i]->sex);
    printf("\n\t\t\t--------☆请输入联系人电话：");
    scanf("%s",&p1[i]->tel);
    printf("\n\t\t\t--------☆请输入联系人地址：");
    scanf("%s",&p1[i]->city);
    printf("\n\t\t\t--------☆请输入联系人班级：");
    scanf("%s",&p1[i]->group);
    p1[i]->next=NULL;
    if(head->next==NULL)
      head->next=p1[i];
    else
    { for(p2=head;p2->next!=NULL;p2=p2->next);      
      p2->next=p1[i];
    }
  }
  printf("信息已添加！\n");
  return;                                    
}
//添加通讯录信息
void Add()
{ TEL *p,*q;
  if((q=(TEL *)malloc(sizeof(TEL)))==NULL) exit(1);
  printf("\t\t\t--------☆请输入要添加的信息!☆--------\n"); 
  printf("\t\t\t--------☆请输入联系人姓名：");             
  scanf("%s",&q->name);
  printf("\n\t\t\t--------☆请输入联系人性别：");
  scanf("%s",q->sex);
  printf("\n\t\t\t--------☆请输入联系人电话：");
  scanf("%s",q->tel); 
  printf("\n\t\t\t--------☆请输入联系人地址：");
  scanf("%s",q->city);
  printf("\n\t\t\t--------☆请输入联系人班级：");
  scanf("%s",q->group);
  for(p=head;p->next!=NULL;p=p->next) ;
  p->next=q;
  q->next=NULL;
  printf("此信息已添加!");
  return;
}
//查找通讯录信息
int Find()
{  char t;
   if(head->next==NULL)
   {  printf("此通讯录为空!\n");
      return 0;
   }
   printf("\n1.按姓名查找\n2.按电话查找\n0.返回主菜单\n");
   printf("请选择要输入的序号(0~3):");
   fflush(stdin);
   getchar();
   t=getchar();
   switch(t)
 {
  case '1':findname(); break;
  case '2':findtel();  break;
  case '0':return 0;
  default:printf("\n你输入的数字无效!!将返回主菜单...");getchar();
 }
 return 0;
}
 
//姓名查询 
int findname()
{TEL *p;
  char name[20];
  if(head->next==NULL)
  {
    printf("此通讯录为空!\n");
    getchar();
    return 0;
   }
   printf("\n请输入要查找的联系人姓名：");
   fflush(stdin); 
   scanf("%s",name);
   for(p=head->next;p!=NULL;p=p->next)
   {
      if(strcmp(p->name,name)==0)
		  printf("找到拉！\n");
      {   printf("姓名\t性别\t电话\t城市\t班级\n");
          printf("%s\t%s\t%s\t%s\t%s\n",p->name,p->sex,p->tel,p->city,p->group);
      }
      else  if(p->next==NULL)
         printf("无此信息!\n");
         getchar();
		 return 0; 
   }
} 
//电话查找
int findtel()
{TEL *p;
  char tel[20];
  if(head->next==NULL)
  {
    printf("此通讯录为空!\n");
    getchar();
    return 0;
   }
   printf("\n请输入要查找的联系人电话：");
   fflush(stdin); 
   scanf("%s",tel);
   for(p=head->next;p!=NULL;p=p->next)
   {
      if(strcmp(p->tel,tel)==0)
      {   printf("姓名\t性别\t电话\t城市\t班级\n");
          printf("%s\t%s\t%s\t%s\t%s\n",p->name,p->sex,p->tel,p->city,p->group);
      }
      else  if(p->next==NULL)
         printf("无此信息!\n");
         getchar();
		 return 0; 
   }
} 
 
//修改通讯录信息
void Alter()
{  char name[50];                                    
  TEL *p, *p1;
  if (head->next == NULL)
  {  printf("此通讯录为空!\n");
     return;
  }
   printf("请输入要修改的联系人姓名：");
  scanf("%s",name);
  for(p=head->next;p!=NULL;p=p->next)
  {   if(strcmp(p->name,name)==0)
break;
      else   if(p->next==NULL)
      {   printf("无此信息!\n");
          return;
      }
  }
  p1=(TEL *)malloc(sizeof(TEL)); 
  printf("\n请输入修改后的联系人姓名：");                       
  scanf("%s",p1->name);
  strcpy(p->name,p1->name);
  printf("\n请输入修改后的联系人性别：");
  scanf("%s",p1->sex);
  strcpy(p->sex,p1->sex);
  printf("\n请输入修改后的联系人电话：");
  scanf("%s",p1->tel);
  strcpy(p->tel,p1->tel);
  printf("\n请输入修改后的联系人地址：");
  scanf("%s",p1->city);
  strcpy(p->city,p1->city);
  printf("\n请输入修改后的联系人班级：");
  scanf("%s",p1->group);
  strcpy(p->group,p1->group);
  printf("此信息已修改！\n");
  printf("姓名\t性别\t电话\t城市\t班级\n");
  printf("%s\t%s\t%s\t%s\t%s\n",p->name,p->sex,p->tel,p->city,p->group);
  free(p1);
}
//删除通讯录信息
void Delete()
{
  char  name[50];                                   
  TEL *p = head->next, *p1 =head->next,*p2;
  if (head->next == NULL)
  {  printf("此通讯录为空！\n");
    return;
  }
  scanf(" %s",name);
  while ((strcmp(p->name,name)!=0 )&& p->next != NULL)
  {  p1=p;
     p =p->next;
  }
  if (strcmp(name, p->name)==0)                  
  {   if (p == head->next&&p->next!=NULL)
        head->next = p->next;
      else  if(p==head->next&&p->next==NULL)
      {   head->next=p->next;
          printf("信息已删除，现此通讯录为空！！\n");
          return;
      }
      else
        p1->next = p->next;
  }
  else
  {   printf("此信息不存在！！！\n");
      return;
  }
  printf("此信息已删除！");
  printf("姓名\t性别\t电话\t地址\t班级\n");
  for(p2=head->next;p2!=NULL;p2=p2->next)
  printf("%s\t%s\t%s\t%s\t%s\n",p2->name,p2->sex,p2->tel,p2->city,p2->group);
}
//显示所有记录
void List()
{  TEL *p;
  if(head->next==NULL)
  {   printf("此通讯录中无记录!\n");
      return;
  }
  printf("姓名\t性别\t电话\t地址\t班级\n");
  for(p=head->next;p!=NULL;p=p->next)
  printf("%s\t%s\t%s\t%s\t%s\n",p->name,p->sex,p->tel,p->city,p->group);
}
