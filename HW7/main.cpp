#include "RBTree.h"
#include "AVLTree.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

int main(){
    for (int i=0;i<5;i++)
    {
        printf("-------第%d组-------\n",i);
        RBTree rbtree_sequence,rbtree_random;
        AVLTree avltree_sequence,avltree_random;
        clock_t rb_seq_start,rb_seq_end,avl_seq_start,avl_seq_end,rb_ran_start,rb_ran_end,avl_ran_start,avl_ran_end;
        std::ifstream filesequence("./data/sequence"+std::to_string(i)+".txt");
        std::vector<int> vectorsequence;
        std::ifstream filerandom("./data/random"+std::to_string(i)+".txt");
        std::vector<int> vectorrandom;
        int nowdata;


        // sequence
        while(filesequence>>nowdata)
        {
            vectorsequence.push_back(nowdata);
        }
        filesequence.close();
        
        // RBTree
        rb_seq_start=clock();
        for (int i=0;i<vectorsequence.size();i++)
        {
            rbtree_sequence.Insert(i,vectorsequence[i]);
        }
        rb_seq_end=clock();
        printf("顺序插入RBTree第%d组时间：%lf",i,(double)(rb_seq_end-rb_seq_start)/CLOCKS_PER_SEC);
        printf("顺序插入RBTree第%d组旋转次数：%d\n",i,rbtree_sequence.rotatenum);

        rb_seq_start=clock();
        for (int i=0;i<vectorsequence.size();i++)
        {
            rbtree_sequence.Search(i);
        }
        rb_seq_end=clock();
        printf("顺序查找RBTree第%d组时间：%lf\n",i,(double)(rb_seq_end-rb_seq_start)/CLOCKS_PER_SEC);

        // AVLTree
        avl_seq_start=clock();
        for (int i=0;i<vectorsequence.size();i++)
        {
            avltree_sequence.Insert(i,vectorsequence[i]);
        }
        avl_seq_end=clock();
        printf("顺序插入AVLTree第%d组时间：%lf",i,(double)(avl_seq_end-avl_seq_start)/CLOCKS_PER_SEC);
        printf("顺序插入AVLTree第%d组旋转次数：%d\n",i,avltree_sequence.rotatenum);

        avl_seq_start=clock();
        for (int i=0;i<vectorsequence.size();i++)
        {
            avltree_sequence.Search(i);
        }
        avl_seq_end=clock();
        printf("顺序查找AVLTree第%d组时间：%lf\n\n",i,(double)(avl_seq_end-avl_seq_start)/CLOCKS_PER_SEC);
        

        // random
        while(filerandom>>nowdata)
        {
            vectorrandom.push_back(nowdata);
        }
        filerandom.close();

        // RBTree
        rb_ran_start=clock();
        for (int i=0;i<vectorrandom.size();i++)
        {
            rbtree_random.Insert(i,vectorrandom[i]);
        }
        rb_ran_end=clock();
        printf("乱序插入RBTree第%d组时间：%lf",i,(double)(rb_ran_end-rb_ran_start)/CLOCKS_PER_SEC);
        printf("乱序插入RBTree第%d组旋转次数：%d\n",i,rbtree_random.rotatenum);

        rb_ran_start=clock();
        for (int i=0;i<vectorrandom.size();i++)
        {
            rbtree_random.Search(i);
        }
        rb_ran_end=clock();
        printf("乱序查找RBTree第%d组时间：%lf\n",i,(double)(rb_ran_end-rb_ran_start)/CLOCKS_PER_SEC);

        // AVLTree
        avl_ran_start=clock();
        for (int i=0;i<vectorrandom.size();i++)
        {
            avltree_random.Insert(i,vectorrandom[i]);
        }
        avl_ran_end=clock();
        printf("乱序插入AVLTree第%d组时间：%lf",i,(double)(avl_ran_end-avl_ran_start)/CLOCKS_PER_SEC);
        printf("乱序插入AVLTree第%d组旋转次数：%d\n",i,avltree_random.rotatenum);

        avl_ran_start=clock();
        for (int i=0;i<vectorrandom.size();i++)
        {
            avltree_random.Search(i);
        }
        avl_ran_end=clock();
        printf("乱序查找AVLTree第%d组时间：%lf\n\n",i,(double)(avl_ran_end-avl_ran_start)/CLOCKS_PER_SEC);

    }
}