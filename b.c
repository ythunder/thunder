#include <stdio.h>

struct student
{
  int number;
  char name[20];
  char sex;
  int age;
  char addr[30];
};

void print_struct(struct student *p){

	p->age ++;
	printf("%d,%s,%c,%d,%s\n",p->number,p->name,p->sex,p->age,p->addr);
}

int main()
{
	struct student s[3]={ {10000,"zhang",'m',21,"shang hai"},
				{10001,"li",'f',20,"bei jing"},
				{10002,"liu",'m',22,"guang dong"} };
	printf("%d,%d,%d\n",s[0].age,s[1].age,s[2].age);
 	struct student *p;
	for(p=s;p<s+3;p++)
	print_struct(p);
	printf("%d,%d,%d\n",s[0].age,s[1].age,s[2].age);
	return 0;
}

