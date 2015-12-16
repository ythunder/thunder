/*************************************************************************
	> File Name: Hash.cpp
	> Author: yunting
	> Mail:1368282581@qq.com 
	> Created Time: 2015年12月16日 星期三 13时44分04秒
 ************************************************************************/

#include <stdio.h>

#define HASHSIZE 11

typedef struct
{
    int key;
    int take_over;
}Datatype;

typedef struct
{
    int data;
    int time;
} Hashtable;


void Createht(Hashtable *ht, Datatype *L, int key_num)  //ht为表，L中存关键字，
{
    int i, times, a;
    for (i = 0; i < HASHSIZE; i++) {
        ht[i].data = -2;
        ht[i].time = 0;
    }
    for (i = 0; i < key_num; i++) {
        if ( ht[L[i].take_over].data == -2 ) {
            ht[L[i].take_over].data = L[i].key;
            ht[L[i].take_over].time = 1;
        }
        else {
            times = 1;
            a =  L[i].take_over;
            while (ht[a].data != -2) {
                a++;
                times++;
            }
            ht[a].data = L[i].key;
            ht[a].time = times;

        }   
    }

    for (i = 0; i < key_num; i++) {
        printf("%d-%d  ",L[i].key, L[i].take_over );
    }
    printf ("\n\n");
    for (i = 0 ; i < HASHSIZE; i++) {
        printf ("%d-%d  ", ht[i].data, ht[i].time);
    }
}

int HashFunc (int key)
{
    return key % HASHSIZE;
}

int Collision (int di)
{
    return (di + 1) % HASHSIZE;
}

int HashSearch(Hashtable *ht, int key)
{
    int address;
    address = HashFunc(key);
    while (ht[address].data != -2 && ht[address].data != key) {
        address = Collision(address);
    }
    if (ht[address].data == key) {
        return address;
    } else {
        return -1;
    }
}


int main()
{
    int key_num, i, find_key, key_add;
    Datatype L[HASHSIZE];   
    Hashtable ht[HASHSIZE];

    printf ("please input the number of all key : ");
    scanf ("%d", &key_num);
    getchar();
    printf ("please input the all key : ");
    for (i = 0; i < key_num; i++) {
        scanf ("%d", &L[i].key);
        L[i].take_over = L[i].key % HASHSIZE;
    }
    getchar();

    Createht(ht, L, key_num);

    printf ("please input the key you want to find : ");
    scanf ("%d", &find_key);

    key_add = HashSearch(ht, find_key); 

    printf("%d",key_add);
    return 0;
}
