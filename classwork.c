/*************************************************************************
	> File Name: 课设.c
	> Author: yunting
	> Mail:1368282581@qq.com 
	> Created Time: 2015年12月28日 星期一 00时54分36秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <memory.h>
#include <string.h>

#define MAX_WEIGHT 999999

typedef struct
{
    int weight;
    int parent;
    int lchild;
    int rchild;
}HuffmanTree;

int count = 257;
HuffmanTree HuffNode[512];
char code_tree[256][256];
int leaf = 0;

void mean_show()
{
    printf ("\n");
    printf ("\n************************");
    printf ("\n  2、编码");
    printf ("\n  3、译码");
    printf ("\n  4、退出");
    printf ("\n**********************\n");
}


//读文件
int read_file(char *sourcefile)
{
    int fd;
    char ch;
    int ret, i = 0, j;
    if ( (fd = open(sourcefile, O_RDONLY )) < 0 ) {
        fprintf(stderr,"line :%d\n", __LINE__);
        return 0;
    } else {
        for (i = 0; i < 512; i++) {
            HuffNode[i].parent = -1;
            HuffNode[i].lchild = -1;
            HuffNode[i].rchild = -1;
        }
        while ((read (fd, &ch, sizeof(char))) > 0 ) {
            HuffNode[ch].weight++;
            i++;
        }
        close(fd);

        for (i = 0; i < 256; i++) {
            if(HuffNode[i].weight > 0) {
                leaf++;
            }
        }
    }
    return 0;
}

//找到最小数的下标
int Min_bit()
{   
    int min = MAX_WEIGHT;
    int i, min_bit;
    for (i = 0; i < count; i++) {
        if (HuffNode[i].parent == -1) {
            if (HuffNode[i].weight < min && HuffNode[i].weight != 0) {
                min = HuffNode[i].weight;
                min_bit = i;
            }
        }
    }
    if(min == MAX_WEIGHT) {
        return -1;
    }
    return min_bit;
}


//建立哈弗曼树
int Creat_HuffTree() 
{
    int min_one, min_two;

    while (1) {
        min_one = Min_bit();
        HuffNode[min_one].parent = count;
        min_two = Min_bit();
        HuffNode[min_two].parent = count;

        HuffNode[count].weight = HuffNode[min_one].weight + HuffNode[min_two].weight;
        HuffNode[count].lchild = min_one;
        HuffNode[count].rchild = min_two;
        count++ ;
        if (count == (leaf + 256)) {
            return 0;
        }
    }
    return 0;
}


//编码
void Code_Tree(char *sourcefile)
{
    int i, a, x, fd, fd1, w, s, j, d, flag = 0, c = 0;
    char code[256], buf[256];
    char ch;

    if( (fd = open("decode", O_WRONLY)) < 0) {
        fprintf (stderr,"line:%d\n", __LINE__);
    }
   
    for (i = 0; i < 512; i++) {
        printf("%d-%d-%d-%d-%d\n",i,HuffNode[i].weight, HuffNode[i].lchild, HuffNode[i].rchild, HuffNode[i].parent);
    }

    for (i = 0; i < 256; i++) {
        memset(code, 0, sizeof(code));
        a = i;
        flag = 0;
        j = 0; x = 0;
        c = HuffNode[a].parent;
        while (c != -1) {
            if (HuffNode[c].lchild == a) {
                code[j++] = '0';
                flag = 1;
            } 
            else {
                code[j++] = '1';
                flag = 1;
            }
            code[j] = '\0' ;
            a = c;
            c = HuffNode[a].parent;
        }
        if(flag == 0) {
            continue;
        }
        while(j != 0) {
            code_tree[i][x++] = code[--j];
        }
        printf ("%c : %s\n", i, code);
    }

    if((fd1 = open(sourcefile, O_RDONLY)) < 0 ) {
        fprintf (stderr, "line:%d\n", __LINE__);
    }
    
    while ((read (fd1, &ch, sizeof(char))) > 0 ) {
        d = (int)ch;
        strcpy(buf,code_tree[d]);
        if(write(fd, buf, strlen(buf)) < 0) {
            fprintf(stderr, "line:%d\n", __LINE__);
        }
    }
    close(fd);
    close(fd1);
}


//译码
void Decode_Tree(char *string, char *deprefile)
{
    int fd,fd1, i = 0, c;
    char ch;
    char temp[256];


    c = --count;
    
    if((fd = open(string, O_RDONLY)) < 0 ) {
        fprintf(stderr, "line:%d\n", __LINE__);
    }
    if ((fd1 = open(deprefile, O_WRONLY)) < 0) {
        fprintf (stderr, "linene:%d\n", __LINE__);
    }

        while(read(fd, &ch, sizeof(char)) == 1) {
            temp[i++] = ch;
            if (ch == '0') {
                c = HuffNode[c].lchild;
                if(HuffNode[c].lchild == -1 && HuffNode[c].rchild == -1) {
                    if((write(fd1, &c, sizeof(char))) < 0) {
                        fprintf (stderr, "line:%d\n", __LINE__);
                    }
                    memset(temp, 0, sizeof(temp));
                    c = count;
                }
            }
            else if(ch == '1') {
                c = HuffNode[c].rchild;
                if (HuffNode[c].lchild == -1 && HuffNode[c].rchild == -1) {
                    if((write(fd1, &c, sizeof(char))) < 0 ){
                        fprintf (stderr, "line:%d\n", __LINE__);
                    }
                    memset(temp, 0, sizeof(temp));
                    c = count;
                }
            }
        }
    close(fd);
    close(fd1);

}

//压缩文件
int compress(char *filename, char *filename1)
{
    char temp = 0, ch;
    int t = 0, fd, fd1;

    if ((fd = open(filename, O_RDONLY)) < 0) {
        fprintf (stderr, "line: %d\n", __LINE__);
    }

    if ((fd1 = open(filename1, O_WRONLY | O_CREAT, 0744)) < 0) {
        fprintf (stderr, "line: %d\n", __LINE__);
    }

    while ( read(fd, &ch, sizeof(char)) > 0) {
        ch = ch - '0';
        temp = temp << 1;
        temp = temp | ch;
        t++;
        if (t == 8) {
            if(( write(fd1, &temp, sizeof(char))) < 0) {
               fprintf (stderr, "line:%d\n", __LINE__);
            }
            temp = 0;
            t = 0;
        }
    }
    if (t < 8) {
        temp = temp << (8 - t);
        if (write(fd1, &temp, sizeof(char)) < 0) {
            fprintf (stderr, "line:%d\n", __LINE__);
        }
    }
    close(fd);
    close(fd1);
}

//解压文件
int decompress(char *codefile)
{
    int fd, fd1, i= 0;
    unsigned char ch = 0, temp = 0;
    int flag = 0;

    if ((fd = open(codefile,O_RDONLY )) < 0) {
        fprintf (stderr, "line :%d\n", __LINE__);
    }

    if ((fd1 = open("dedecode",O_WRONLY)) < 0) {
        fprintf (stderr, "line:%d\n", __LINE__);
    }
    while ((read(fd, &ch, sizeof(char))) > 0) {
        i = 0;
        while (i < 8) {
            temp = 0;
            temp = temp | ch;
            temp = temp >> 7;
            ch = ch << 1;
            temp += '0';
            if ((write(fd1, &temp, sizeof(char))) < 0) {
                fprintf (stderr, "line:%d\n", __LINE__);
            }
            i++;
        }
    }
    close(fd);
    close(fd1);
}

int main()
{
    int i, choice;
    char sourcefile[30], codefile[30];
    char *string = "dedecode";
    char *string1 = "decode";
    char deprefile[30];

     for (i = 0; i < 512; i++) {
        HuffNode[i].weight = 0;
        HuffNode[i].lchild = -1;
        HuffNode[i].rchild = -1;
        HuffNode[i].parent = -1;
    }

    printf ("please input the filename: ");
    scanf ("%s", sourcefile);
    printf ("please input the filename: ");
    scanf ("%s", codefile);

    memset(code_tree, 0, sizeof(code_tree));

    printf ("1.压缩文件  2.解压文件\n");
    scanf ("%d", &choice);


        read_file(sourcefile); //从sourcefile文件中读出文件
        Creat_HuffTree(); //建立哈弗曼树
        Code_Tree(sourcefile); //把sourcefile中的内容编码写入*.decode
       compress(string1, codefile); //将*.decode的编码压缩入codefile
        
        printf ("请输入解压后存储文件：");
        scanf ("%s", deprefile);
        decompress(codefile);       //从codefile读出压缩码解压至dedecode
        Decode_Tree(string, deprefile);     //将dedecode中的编码译码至deprefile
        
    //read_file(sourcefile); 
   // Creat_HuffTree();
    //Code_Tree();
    //Decode_Tree();
    // compress(sourcefile, codefile);
    return 0;
}








