#include "SkipList.h"
#include "AVLTree.h"
#include <stdio.h>
#include <time.h>
#include <random>

#define insertnum 2000
int main()
{
    SkipList skiplist;
    AVLTree avltree;
    clock_t start,end;
    double time;
    start = clock();
    // for (int i=0;i<insertnum;i++)
    // {
    //     //顺序
    //     avltree.Insert(i,i);
    //     skiplist.Insert(i,i);

    //     // //随机
    //     // int data=rand();
    //     // avltree.Insert(data,data);
    //     // skiplist.Insert(data,data);
    // }

    //逆序
    for (int i=insertnum;i>0;i--)
    {
        // avltree.Insert(i,i);
        skiplist.Insert(i,i);
    }
    end=clock();
    time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("%f\n",time);
}