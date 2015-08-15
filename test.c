#include <stdio.h>
#include <string.h>

struct student
{
    char  number[5];
    char name[10];
};

int main()
{
    struct student stu,stu1;
    char buf[100];

    memset(&stu, 0, sizeof(struct student));

    strcpy(stu.number, "yun");
    strcpy(stu.name, "yunting");

    memcpy(buf, &stu, sizeof(struct student));
    printf("%s\n", buf);
   
    memcpy(&stu1, buf, sizeof(struct student));
    printf("%s %s\n", stu1.number, stu1.name);
    
    return 0;
}

