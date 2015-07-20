
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct student     
{
	int number;
/*	char name[20];
	*/	char password[20];
	struct student *next;
}stu;



struct manager     
{
	int gNumber;
	char gName[20];
	char gPassword[20];
}man[3];

struct manager man[3]={{1111111,"yuandashao","yunting"},    
		    	{2222222,"qiaomeiren","mingyue"},
		    	{3333333,"shiyeweiwu","munianci"}};





struct books	
{
	int bNumber;
	char bName[20];
	char author[20];
	int amount;
 	int borout;
	int oveplus;
	struct books *next;
}boo;



struct history
{
	char choice[4];
	int stu_num;
	int boo_num;
	char boo_name[20];
	int bor_num;
	struct history *next;
	
}his;


int show(){			

	int identity;
	do{	
		system("clear");
		printf("\n\t\t\t\t======================================");
		printf("\n\t\t\t\tcongratulation enter labrary system");     
		printf("\n\n\t\t\t\tplease chance:");
		printf("\n\n\t\t\t\t1.student login\n\t\t\t\t2.manager login\n\t\t\t\t3.register new number\n\t\t\t\t0.exit\n");
		printf("\n\t\t\t\t======================================");
		
	
	scanf("%d",&identity);
	}while(identity!=1 && identity!=2 && identity!=3);

	return identity;
	
}




int registe(){
	
	int i=0 , number;
	char pass[20],password[20],name[20];
	char filename[40];
	FILE *fp;

	
	fp=fopen("123.txt","wt");
	if(fp==NULL){

		printf("error open!press any key to back~");
		getchar();
		exit(1);
	}
	
	system("clear");
	printf("\n\n\t\t\t\tplease input you number firstly: ");
	scanf("%d",&number);		
/*	printf("\n\t\t\t\tplease input your name: ");
	scanf("%s",name);
*/	printf("\n\t\t\t\tplease input your password: ");
	scanf("%s",password);
	printf("\n\t\t\t\tinput again: ");
	scanf("%s",pass);
	while(strcmp(password, pass) != 0){
		
	if(i==3) return 0;
		printf("\n\t\t\t\terror, try again");
		scanf("%s", pass);
		i++;
				
	}	
	fprintf(fp,"%d %s %s\n",number,name,pass);
	fclose(fp);
	printf("\n\t\t\t\tregister successfully!\n");
	getchar();
	getchar();
	return 0;
}





	
struct student *read_in(){

	struct student *phead,*r,*ptemp;
	FILE *fp;
	char filename[40];

	
	if((fp=fopen("123.txt","rt"))==NULL){
		printf("\n\t\t\t\terror,press any key to exit");
		getchar();
		exit(1);
	}

	phead=(struct student *)malloc(sizeof(struct student));
	phead->next=NULL;
	r=phead;
	
	while(!feof(fp)){

		ptemp=(struct student *)malloc(sizeof(struct student));
		fscanf(fp,"%d %s",&ptemp->number,ptemp->password);

		r->next=ptemp;
		r=ptemp;
	
	}

	r->next=NULL;
	fclose(fp);
	getchar();
	return phead;
}



 int flag=0;
int login(struct student *phead){

	struct student *ptemp;
	int number;
	char password[40];
	int i=0;
	
	system("clear");
	printf("\n\t\t\t\tplease input your number:");
	scanf("%d",&number);
	printf("\n\t\t\t\tplease input your password:");
	scanf("%s",password);

	ptemp=phead->next;
	
	while(ptemp!=NULL){

		if(ptemp->number == number){
				
			while(strcmp(ptemp->password, password) != 0){
					
				printf("\n\t\t\t\terror, try again!");
				scanf("%s", password);
				i++;
				if(i == 3) return 0;	
			}
		printf("\n\t\t\t\tlogin successfully!");
		flag=1;	
		return number;
				
		}
		ptemp=ptemp->next;
	}
	printf("\n\t\t\t\tno have the number,please register!");
	getchar();
	getchar();
	return 0;
}



int  entry(){
		
	char password[20];
	int i,number,c=0;

	
	system("clear");
	printf("\n\n\t\t\t\tplease input your number :");
	scanf("%d", &number);
	printf("\n\t\t\t\tplease input your password:");
	scanf("%s",password);
	for(i=0 ; i<3 ; i++){

		if(man[i].gNumber == number){
		
			while(strcmp(man[i].gPassword,password) != 0){
				
				printf("\n\t\t\t\terror,please try again!");
				scanf("%s", password);
				c++;
				if(c == 3)  return 0;
			}
			
			printf("\n\t\t\t\tlogin successfully");
			return number;
		}
		
	}
 	printf("\n\t\t\t\tno have the number,please try it after check.");
	getchar();
	getchar();
	return -1;
}
	
	


struct books *read_book(){

	struct books *phead,*r,*ptemp;
         FILE *fp;
         char filename[40],bName[20],author[20];
 	int bNumber,amount,borout,oveplus;
	int i=0;

         fp=fopen("tushu.txt","rt");
         if(fp==NULL){
                 printf("error,press any key to exit");
                 getchar();
                 exit(1);
         }
 
         phead=(struct books *)malloc(sizeof(struct books));
         phead->next=NULL;
         r=phead;

         while(1){
 
		i++;
                 ptemp=(struct books *)malloc(sizeof(struct books));
                 fscanf(fp, "%d %s %s %d %d %d",&ptemp->bNumber,ptemp->bName,ptemp->author,&ptemp->amount,&ptemp->borout,&ptemp->oveplus);
 
                 r->next=ptemp;
                 r=ptemp;

		if (feof(fp) == 1)
			break;
		if(i==100) break;
         }
 
         r->next=NULL;
         fclose(fp);
         getchar();
         return phead;

}




int stu_show(){

	int choice;

	do{	
		system("clear");

		printf("\n\t\t\t\t===========================================");
		printf("\n\n\t\t\t\tBook Are The Ladder Of Human Progress");
		printf("\n\t\t\t\tplease choose what you need:");
		printf("\n\t\t\t\t1.found the book number across book number");
		printf("\n\t\t\t\t2.Check your own borrow and return history");
		printf("\n\t\t\t\t3.back!\n\t\t\t\t");
		printf("\n\t\t\t\t===========================================");
		scanf("%d",&choice);
	}while(choice != 1 && choice != 2 && choice != 3);
	
	return choice;	
	
}



int mena_show(){

	int choice;

	do{
		system("clear");
		printf("\n\t\t\t\t===========================================");
		printf("\n\n\t\t\t\tplease choose what you need:\n");
		printf("\t\t\t\t1.add the new books\n");
		printf("\t\t\t\t2.borrow out books\n");
		printf("\t\t\t\t3.back the books\n");
		printf("\t\t\t\t4.check the borrow and back history\n");
		printf("\t\t\t\t5.back!");
		printf("\n\t\t\t\t===========================================");
 		scanf("%d",&choice);	
	}while(choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice !=5);

	return choice;

}



int found(struct books *head){

	char name[20];
	struct books *ptemp;
	int flag=0;
	ptemp=head;

	printf("\n\t\t\t\tplease input the name:");
	scanf("%s",name);
	while(ptemp!=NULL){

	if(strcmp(name,ptemp->bName)==0){

		printf("\n\t\t\t\t================================================================\n");
		printf("\n\t\t\t\t boo_num   bo_name   bo_auth     boo_num     borout   overplus \n");
		printf("\n\t\t\t\t %4d %15s %11s %10d %10d %10d\n",ptemp->bNumber,ptemp->bName,ptemp->author,ptemp->amount,ptemp->borout,ptemp->oveplus);
		flag=1;
	}
	
	ptemp=ptemp->next;
			
	}	
	if(flag==0){
	printf("\n\t\t\t\tnot find the book,press any key to back");
	getchar();
	getchar();
	return 0;	}


	getchar();
	getchar();
	return 0;
}



	
int look(int stu_num,struct history *head){

	struct history *ptemp;
	int flag=0;

	ptemp=head->next;
	
	printf("\n\t\t\t\t====================================================\n");
	printf("\n\t\t\t\t state   stu_num   boo_num     boo_name     bor_num \n");

	while(ptemp != NULL){

		if(ptemp->stu_num == stu_num){

			printf("\t\t\t\t%5s %10d %6d %17s %10d\n",ptemp->choice,ptemp->stu_num,ptemp->boo_num,ptemp->boo_name,ptemp->bor_num);
			flag=1;
			
		}	
		ptemp=ptemp->next;
	}

	 if(flag==0)  printf("\n\t\t\t\tno have history ");
	
	getchar();
	return 0;
}





int add_in(struct books *phead){
	
   	char choice; 
	struct books *ptemp;	
	int number=0,bNumber,amount,borout,oveplus;
	char filename[40],bName[20],author[20];
	FILE *fp;

		
	fp=fopen("tushu.txt","at");
	if(fp == NULL){
	
		printf(" open error ");
		getchar();
		return 0;
	}

	ptemp=phead->next;
	while(ptemp != NULL){
		
		number++;
		ptemp=ptemp->next;
	}
	 
	do{

		bNumber=number;

	 	printf("\n\t\t\t\tplease input the new book's name:");
		scanf("%s",bName);

		printf("\n\t\t\t\tplease input the author:");
		scanf("%s",author);
	
		printf("\n\t\t\t\tplease input the amount:");
		scanf("%d",&amount);

		getchar();
		borout = 0;
		oveplus = amount;
		
		fprintf(fp,"%d %s %s %d %d %d\n",number,bName,author,amount,borout,oveplus);
		
		number++;

		printf("\n\t\t\t\tcontinue to add?  (Y/N)");
		scanf("%c",&choice);

	}while(choice == 'y' || choice == 'Y');

	printf("\n\t\t\t\tadd successful！");
	fclose(fp);
	getchar();
	return number;
}



int bor_out(){
	
	struct books *ptemp,*phead;
   	struct student *head,*r;
	char name[20],choice[4],stu_pas[20];
	int number,bor_num,stu_num,i=0,right=0,u=0,flag=0;
	FILE *fp,*fp1;
	
	fp=fopen("history.txt","at");
	if(fp == NULL){
	
		printf(" open error ");
		getchar();
		return 0;
	}

	phead=read_book();
	head=read_in();
	r=head->next;

	printf("\n\t\t\t\tplease input the student's number:");
	scanf("%d",&stu_num);
	printf("\n\t\t\t\tplease input the student's password:");
	scanf("%s",stu_pas);

	while(r!=NULL){

		if(r->number == stu_num){
				
			while(strcmp(r->password,stu_pas) != 0){
					
				printf("error, try again!");
				scanf("%s", stu_pas);
				i++;
				if(i == 3) return 0;
			}
	       		right=1;
		}
		r=r->next;
	}
	

	if(right == 0){ 
   		printf("\n\t\t\t\tno have the number");
		getchar();
		return 0;
	}
	
	fp1=fopen("tushu.txt","wt");

	if(fp1 == NULL){
	
		printf(" open error ");
		getchar();
		return 0;
	}

	printf("\n\t\t\t\tplease input the book's number that you will borrow out:");
	scanf("%d",&number);

	ptemp=phead->next;
	while(ptemp->next != NULL){
		
		if(ptemp->bNumber == number){
			
			printf("\n\t\t\t\tthe book's number is %d", ptemp->bNumber);
			printf("\n\t\t\t\tthe book's name is %s",ptemp->bName);
			printf("\n\t\t\t\tall amount is %d", ptemp->amount);
			printf("\n\t\t\t\tcould borrow number is %d", ptemp->oveplus);

			printf("\n\n\t\t\t\tplease input the number you will to borrow:");
			scanf("%d",&bor_num);

			if(bor_num>ptemp->oveplus || bor_num<=0){

				printf("\n\t\t\t\terror!");

			}
			else{
				ptemp->borout += bor_num;
				ptemp->oveplus -= bor_num;
				strcpy(choice,"out");
				fprintf(fp,"%s %d %d %s %d\n",choice,stu_num,number,ptemp->bName,bor_num);
				flag=1;
				fclose(fp);
				printf("\n\t\t\t\tborrow successful!");
			
	             }
	
	}
		fprintf(fp1,"%d %s %s %d %d %d\n",ptemp->bNumber,ptemp->bName,ptemp->author,ptemp->amount,ptemp->borout,ptemp->oveplus);
		ptemp = ptemp->next;
	}

	fclose(fp1);

	if(flag == 0)
	 printf("\n\t\t\t\tborrow fall!");

	getchar();
	getchar();
	return 0;
}





struct history *read_hi(){
		
	struct history *head , *r , *stu;
	FILE *fp;
	char filename[40];
	int i=0;

	fp=fopen("history.txt","rt");
	if(fp == NULL){
	
		printf("error open !\n");
		getchar();
		return 0;
	}

        head =(struct history *)malloc(sizeof(struct history));
        head->next=NULL;
        r=head;
	while(1){
		
		i++;
		stu = (struct history *)malloc(sizeof(struct history));
		fscanf(fp,"%s %d %d %s %d",stu->choice,&stu->stu_num,&stu->boo_num,stu->boo_name,&stu->bor_num);
	
		if(feof(fp)==1)	break;
	
		if(i==100) break;
		r->next = stu;
		r=stu;
	}
	
	r->next = NULL;
	fclose(fp);
	getchar();
	return head;
}





int bac_in(struct books *phead){

	int number , bac_num,c=0;
	struct books *ptemp;
	char filename[40],choice[4];
	int stu_num,flag=0;
	FILE *fp,*fp1;


	fp=fopen("history.txt","at");
	fp1=fopen("tushu.txt","wt");

	if(fp==NULL){
		
		printf("error open!press any key to return!");
		getchar();
		return 0;
	}
	if(fp1==NULL){
		
		printf("error open!press any key to return!");
		getchar();
		return 0;
	}

	printf("\n\t\t\t\tplease input the student's number");
	scanf("%d",&stu_num);

	printf("\n\t\t\t\tplease input the book's number");
	scanf("%d",&number);

	
	ptemp = phead->next;
	while(ptemp->next != NULL){
		
		if(ptemp->bNumber == number){
			
			printf("\n\t\t\t\tthe book's number is %d", ptemp->bNumber);
			printf("\n\t\t\t\tthe book's name is %s",ptemp->bName);
			printf("\n\t\t\t\tall amount is %d", ptemp->amount);
			printf("\n\t\t\t\tcould borrow number is %d", ptemp->oveplus);
			printf("\n\t\t\t\tplease input the number you will to back:");
			scanf("%d",&bac_num);
			if(bac_num <= 0 || bac_num > (ptemp->amount - ptemp->oveplus)){

				printf("\n\t\t\t\terror,please input again!");
		
			}
			else{

				ptemp->borout -= bac_num;
				ptemp->oveplus += bac_num;
				strcpy(choice,"int");
				fprintf(fp,"%s %d %d %s %d\n",choice,stu_num,number,ptemp->bName,bac_num);
				flag=1;
				fclose(fp);
				printf("\n\t\t\t\tback successful!");
			}
	}		
	             
		fprintf(fp1,"%d %s %s %d %d %d\n",ptemp->bNumber,ptemp->bName,ptemp->author,ptemp->amount,ptemp->borout,ptemp->oveplus);
		ptemp = ptemp->next;
}

	fclose(fp1);
	if(flag == 0)
	{
		printf("\n\t\t\t\tback fall! ");
		getchar();
		getchar();
		return 0;
	}
	getchar();
	getchar();
	return 0;
}




int insert(struct history *head){

	struct history *ptemp;
 	int number,choice,boo_number,flag=0;


	ptemp=head->next;
	printf("\n\t\t\t\tplease selete insert way:\n");
	printf("\n\t\t\t\t1.across student's number\n\t\t\t\t2.across book's number\n\t\t\t\t 3.tuichu\n");
	scanf("%d",&choice);
	switch(choice){
		case 1:printf("\n\t\t\t\tplease input the number:");
			scanf("%d",&number);
			
			printf("\n\t\t\t\t====================================================\n");
			printf("\t\t\t\t state   stu_num   boo_num     boo_name     bor_num \n");
		
			while(ptemp != NULL){

				if(ptemp->stu_num == number){

			printf("\t\t\t\t%5s %10d %6d %17s %10d\n",ptemp->choice,ptemp->stu_num,ptemp->boo_num,ptemp->boo_name,ptemp->bor_num);
			
			flag=1;
				}
				
				ptemp=ptemp->next;
			}
			if(flag==0){
				
				printf("\n\t\t\t\the student no have return or back book history.\n\n");
				getchar();
				getchar();
				return 0;
			}
			getchar();
			getchar();
			break;

	    case 2:printf("\n\t\t\t\tplease input the book's number:");
		   scanf("%d",&boo_number);

		printf("\n\t\t\t\t====================================================\n");
		printf("\n\t\t\t\t state   stu_num   boo_num     boo_name     bor_num \n");
	
		while(ptemp != NULL){

			if(ptemp->boo_num == boo_number){

			printf("\t\t\t\t%5s %10d %6d %17s %10d\n",ptemp->choice,ptemp->stu_num,ptemp->boo_num,ptemp->boo_name,ptemp->bor_num);
			flag=1;
			}

			ptemp=ptemp->next;
		}
		if(flag==0){
			printf("\n\t\t\t\tthe book noe have been borrow out!\n\n");
			getchar();
			getchar();
			return 0;
		}
		getchar();
		getchar();
		break;

	case 3:return 0;
	}
	
}


int manager_sum(){
	
	struct books *head;
	struct history *phead;
	int u;
	u=entry();
	if(u==-1){
		printf("\n\t\t\t\tlogin error");
		getchar();
		getchar();
		 return 0;
	}
	for(;;){
	
		switch(mena_show(man)){

			case 1:head=read_book();
				add_in(head);   
				break;

			case 2:bor_out();
				break;

			case 3:head=read_book();
				bac_in(head);
				break;
			
			case 4:phead=read_hi();
				insert(phead);
				break;

			case 5:return 0;
		}
	}
}



int stude_sum(){
	
	struct student *p;
	struct books *head;
	struct history *phead;
	int t;

	p=read_in();
 	t=login(p);
	
	if(flag==0){

		printf("\n\n\t\t\t\tlogin error !\n\n");
		getchar();
		return 0;
	}

	for(;;){

		switch(stu_show()){
		
			case 1:head=read_book();
				found(head);
				break;
				
			case 2:phead=read_hi();
				look(t,phead);
            			break;

			case 3:return 0;
		}
	}
}



int main(){

	for(;;){
		
		switch(show()){
		
			case 1:stude_sum(); break;

			case 2:manager_sum(); break;

			case 3:registe(); break;
			
			case 0:return 0;
		}
	}
}

			

struct books *xiugai(struct books *head)
{
	struct books *ptemp;
	int number;
	int i=0,choice;
	 printf("please input the tushu number");
	 scanf("%d",&number);
	for(ptemp=head->next;ptemp!=NULL;ptemp=ptemp->next){

		if(number == ptemp->bNumber){

		printf("the book's xinxi is:");
		printf("1、the book's number is  %d",ptemp->bNumber);
		printf("2、the book's name is  %s",ptemp->bName);
		printf("3、the book's author is %s",ptemp->author);
		printf("4、the book's amount is %d",ptemp->amount);
		printf("5、the book have borrowed %d ",ptemp->borout);
		printf("6、the book have %d could be borrow.",ptemp->oveplus);
		printf("please choose the number you want to xiugai ");
		scanf("%d ",&choice);
		while(choice<=0||choice>=7){
			
			i++;
			printf("error ,try again!");
			scanf("%d",&choice);
			if(i==3) return 0;
		}
		switch(choice){
		
		case 1:printf("please input the new message:");
			scanf("%d",&ptemp->bNumber);
			break;
		case 2:printf("please input the new message:");
			scanf("%s",ptemp->bName);
			break;
		case 3:printf("please input rhe new message");
			scanf("%s",ptemp->author);
			break;
			
		case 4:printf("please input rhe new message");
			scanf("%d",&ptemp->amount);
			break;

		case 5:printf("please input rhe new message");
			scanf("%d",&ptemp->borout);
			break;

		case 6:printf("please input rhe new message");
			scanf("%d",&ptemp->oveplus);
			break;
        
		}
		flag=1;
	}
	if(flag==0){
		printf("cha wu ci shu");
		getchar();
		return 0;
	}

     return head;
}

 
}


save_in(struct books *phead){

	struct books *stu;
	FILE *fp;
	fp=fopen("tushu.txt","wt");
	if(fp==NULL){
		printf("du wenjian chucuo ");
		getchar();
		return 0;
	}
	for(stu=phead->next;stu!=NULL;stu=stu->next){

		fprintf(fp,"%d %s %s %d %d %d\n",stu->bNumber,stu->bName,stu->author,stu->amount,stu->borout,stu->oveplus);
		printf("successful!");
		getchar();
		return 0;
	}
}

	


