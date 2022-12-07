#pragma once

#include <stdlib.h>
#include <climits>
#include <iostream>
#include <vector>
#include <list>
#include <string>

#define MAX_LEVEL 8
#define PRE_SIZE 32 + 10240
#define MAX_SIZE 2 * 1024 * 1024

enum SKNodeType
{
    HEAD = 1,
    NORMAL,
    NIL
};

enum InsertState
{
    CORRECT = 1,
    DUPLICATE,
    OVERSIZE
};

struct SKNode
{
    uint64_t key;
    std::string val;
    SKNodeType type;
    std::vector<SKNode *> forwards;
    SKNode(uint64_t _key, std::string _val, SKNodeType _type)
        : key(_key), val(_val), type(_type)
    {
        for (int i = 0; i < MAX_LEVEL; ++i)
        {
            forwards.push_back(nullptr);
        }
    }
};

class SkipList
{
private:
    SKNode *head;
    SKNode *NIL;
    unsigned long long size = PRE_SIZE;
    unsigned long long s = 1;
    double my_rand();
    int randomLevel();

public:
    SkipList()
    {
        head = new SKNode(0, "", SKNodeType::HEAD);
        NIL = new SKNode(UINT_MAX, "", SKNodeType::NIL);
        for (int i = 0; i < MAX_LEVEL; ++i)
        {
            head->forwards[i] = NIL;
        }
    }
    InsertState Insert(uint64_t key, std::string value);
    SKNode *Search(uint64_t key);
    bool Delete(uint64_t key);
    void SecSearch(uint64_t key_start, uint64_t key_end, std::list<std::pair<uint64_t, std::string>> &slist);
    unsigned long long Getsize();
    void Getdata(uint64_t &num, uint64_t &minkey, uint64_t &maxkey);
    void Display();
    ~SkipList()
    {
        SKNode *n1 = head;
        SKNode *n2;
        while (n1)
        {
            n2 = n1->forwards[0];
            delete n1;
            n1 = n2;
        }
    }
};
