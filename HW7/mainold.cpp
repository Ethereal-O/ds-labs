#include "RBTree.h"
#include "AVLTree.h"
#include <stdio.h>
#include <time.h>
#include <random>
#include <vector>
#include <string.h>

#define insertnum 200
#define searchstart 1
#define searchtime 20000
int main()
{
    AVLTree avltree;
    bool hash[insertnum];
    memset(hash,0,insertnum);
    int hasinsertnum=0;
    printf("查找比例:");
    double searchratio=0;
    scanf("%lf",&searchratio);
    double searchend=searchratio*insertnum+searchstart;

    //插入
    while(hasinsertnum<insertnum)
    {
        int num=rand()%insertnum;
        if (hash[num]) continue;
        avltree.Insert(num,num);
        hash[num]=true;
        hasinsertnum++;
    }

    //查找AVL
    clock_t start,end;
    double time;
    start = clock();
    for (int i = 0; i < searchtime; i++)
    {
        for (int i = searchstart; i <searchend; i++)
        {
            //splaytree.search(i);
            avltree.Search(i);
        }
    }
    end=clock();
    time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("AVL:%lf\n",time);

    //查找AplayTree
    start = clock();
    // for (int i = 0; i < searchtime; i++)
    // {
    //     for (int i = searchstart; i <searchend; i++)
    //     {
    //         splaytree.search(i);
    //     }
    // }
    end=clock();
    time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("SplayTree:%lf\n",time);
}