#include <cstdio>
#include "cuckoo.h"

cuckoo_hash::cuckoo_hash()
{
    size=INIT_LENGTH;
    hash1=new hashnode[size]();
    hash2=new hashnode[size]();
}

cuckoo_hash::~cuckoo_hash()
{
    delete []hash1;
    delete []hash2;
}

int cuckoo_hash::hash1method(int key)
{
    return key%size;
}

int cuckoo_hash::hash2method(int key)
{
    return (key / size) % size;
}

void cuckoo_hash::kickandjudge(int key,int val)
{
    int kicknum=0;
    int tmpkey=key;
    int tmpval=val;
    while(true)
    {
        int hash1index=hash1method(tmpkey);
        if (!hash1[hash1index].valid)
        {
            hash1[hash1index].valid=true;
            hash1[hash1index].key=tmpkey;
            hash1[hash1index].val=tmpval;
            return;
        }
        int ttmpkey=tmpkey;
        int ttmpval=tmpval;
        tmpkey=hash1[hash1index].key;
        tmpval=hash1[hash1index].val;
        hash1[hash1index].key=ttmpkey;
        hash1[hash1index].val=ttmpval;
        // 输出kick
        // printf("Kick %d with %d in table 0 %d\n",tmpkey,ttmpkey,hash1index);
        kicknum++;
        if (kicknum>=2*size) break;


        int hash2index=hash2method(tmpkey);
        if (!hash2[hash2index].valid)
        {
            hash2[hash2index].valid=true;
            hash2[hash2index].key=tmpkey;
            hash2[hash2index].val=tmpval;
            return;
        }
        ttmpkey=tmpkey;
        ttmpval=tmpval;
        tmpkey=hash2[hash2index].key;
        tmpval=hash2[hash2index].val;
        hash2[hash2index].key=ttmpkey;
        hash2[hash2index].val=ttmpval;
        //输出kick
        // printf("Kick %d with %d in table 1 %d\n",tmpkey,ttmpkey,hash2index);
        kicknum++;
        if (kicknum>=2*size) break;
    }
    // 存在环
    printf("Loop Detect\n");
    resize();
    Insert(tmpkey,tmpval);
}

void cuckoo_hash::resize()
{
    int oldsize=size;
    size=size<<1;
    hashnode *oldhash1=hash1,*oldhash2=hash2;
    hash1=new hashnode[size]();
    hash2=new hashnode[size]();
    for(int i=0;i<oldsize;i++)
    {
        if (oldhash1[i].valid)
        {
            Insert(oldhash1[i].key,oldhash1[i].val);
        }
    }
    for(int i=0;i<oldsize;i++)
    {
        if (oldhash2[i].valid)
        {
            Insert(oldhash2[i].key,oldhash2[i].val);
        }
    }
    delete []oldhash1;
    delete []oldhash2;
}

void cuckoo_hash::Insert(int key, int val)
{
    // 已经存在
    hashnode *existnode=this->Lookup(key);
    if (existnode)
    {
        existnode->val=val;
        return;
    }

    // 有空位
    hashnode* hash1result=&hash1[hash1method(key)];
    if(!hash1result->valid)
    {
        hash1result->valid=true;
        hash1result->key=key;
        hash1result->val=val;
        return;
    }
    hashnode* hash2result=&hash2[hash2method(key)];
    if(!hash2result->valid)
    {
        hash2result->valid=true;
        hash2result->key=key;
        hash2result->val=val;
        return;
    }

    // 无空位
    kickandjudge(key,val);
}
hashnode* cuckoo_hash::Lookup(int key)
{
    hashnode* hash1result=&hash1[hash1method(key)];
    if (hash1result->valid&&hash1result->key==key) return hash1result;
    hashnode* hash2result=&hash2[hash2method(key)];
    if (hash2result->valid&&hash2result->key==key) return hash2result;
    return nullptr;
}
bool cuckoo_hash::Delete(int key)
{
    hashnode *existnode=this->Lookup(key);
    if (!existnode) return false;
    existnode->valid=false;
    return true;
}