#include <iostream>
#include <stdlib.h>

#include "SkipList.h"

double SkipList::my_rand()
{
    s = (16807 * s) % 2147483647ULL;
    return (s + 0.0) / 2147483647ULL;
}

int SkipList::randomLevel()
{
    int result = 1;
    while (result < MAX_LEVEL && my_rand() < 0.5)
    {
        ++result;
    }
    return result;
}

void SkipList::Insert(int key, int value)
{
    // TODO
    SKNode* tmpnode = head;
    SKNode* update[MAX_LEVEL];
    for (int i = MAX_LEVEL-1; i >= 0; i--) {
        if (tmpnode!=head&&tmpnode->key == key) {
            tmpnode->val = value;
            return;
        }
        while (tmpnode->forwards[i]->key <= key) {
            tmpnode = tmpnode->forwards[i];
        }
        update[i] = tmpnode;
    }
    if (tmpnode!=head&&tmpnode->key == key) {
        tmpnode->val = value;
        return;
    }
    SKNode* newnode = new SKNode(key, value, NORMAL);
    int level = randomLevel();
    for (int i = 0; i <level; i++) {
        SKNode* knode = update[i]->forwards[i];
        newnode->forwards[i] = knode;
        update[i]->forwards[i] = newnode;
    }
}

void SkipList::Search(int key)
{
    // TODO
    SKNode* tmpnode = head;
    for (int i = MAX_LEVEL-1; i >= 0; i--) {
        if (tmpnode == head) {
            std::cout << i + 1 << ",h " ;
        }
        else {
            std::cout << i + 1 << "," << tmpnode->key<<" ";
        }
        while (tmpnode->forwards[i]->key < key) {
            tmpnode = tmpnode->forwards[i];
            std::cout << i+1<<"," << tmpnode->key<<" ";
        }
    }
    tmpnode = tmpnode->forwards[0];
    if (tmpnode==NIL){
        std::cout << "1,N"<< " ";
    }else{
        std::cout << "1," << tmpnode->key << " ";
    }
    
    if (tmpnode->key == key) {
            std::cout << tmpnode->val << std::endl;
            return;
        }else{
    std::cout << "Not Found" << std::endl;
        }
    
}

void SkipList::Delete(int key)
{
    // TODO
    SKNode* tmpnode = head;
    SKNode* pre[MAX_LEVEL];
    for (int i = MAX_LEVEL-1; i >= 0; i--) {
        while (tmpnode->forwards[i]->key < key) {
            tmpnode = tmpnode->forwards[i];
        }
        pre[i] = tmpnode;
    }
    for (int i = MAX_LEVEL - 1; i >= 0; i--) {
        if (pre[i]->forwards[i]->key == key) {
            pre[i]->forwards[i] = pre[i]->forwards[i]->forwards[i];
        }
    }
}

void SkipList::Display()
{
    for (int i = MAX_LEVEL - 1; i >= 0; --i)
    {
        std::cout << "Level " << i + 1 << ":h";
        SKNode *node = head->forwards[i];
        while (node->type != SKNodeType::NIL)
        {
            std::cout << "-->(" << node->key << "," << node->val << ")";
            node = node->forwards[i];
        }

        std::cout << "-->N" << std::endl;
    }
}