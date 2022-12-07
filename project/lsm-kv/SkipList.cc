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

InsertState SkipList::Insert(uint64_t key, std::string value)
{
    // TODO
    SKNode *tmpnode = head;
    SKNode *update[MAX_LEVEL];
    for (int i = MAX_LEVEL - 1; i >= 0; i--)
    {
        if (tmpnode != head && tmpnode->key == key)
        {
            unsigned long long newsize = size + value.length() - tmpnode->val.length();
            if (newsize > MAX_SIZE)
            {
                return OVERSIZE;
            }
            tmpnode->val = value;
            size = newsize;
            return DUPLICATE;
        }
        while (tmpnode->forwards[i]->key <= key)
        {
            tmpnode = tmpnode->forwards[i];
        }
        update[i] = tmpnode;
    }
    if (tmpnode != head && tmpnode->key == key)
    {
        unsigned long long newsize = size + value.length() - tmpnode->val.length();
        if (newsize > MAX_SIZE)
        {
            return OVERSIZE;
        }
        tmpnode->val = value;
        size = newsize;
        return DUPLICATE;
    }
    unsigned long long newsize = size + value.length() + sizeof(uint32_t) + sizeof(uint64_t);
    if (newsize > MAX_SIZE)
    {
        return OVERSIZE;
    }
    SKNode *newnode = new SKNode(key, value, NORMAL);
    int level = randomLevel();
    for (int i = 0; i < level; i++)
    {
        SKNode *knode = update[i]->forwards[i];
        newnode->forwards[i] = knode;
        update[i]->forwards[i] = newnode;
    }
    size = newsize;
    return CORRECT;
}

SKNode *SkipList::Search(uint64_t key)
{
    // TODO
    SKNode *tmpnode = head;
    for (int i = MAX_LEVEL - 1; i >= 0; i--)
    {
        // if (tmpnode == head) {
        //     std::cout << i + 1 << ",h " ;
        // }
        // else {
        //     std::cout << i + 1 << "," << tmpnode->key<<" ";
        // }
        while (tmpnode->forwards[i]->key < key)
        {
            tmpnode = tmpnode->forwards[i];
            // std::cout << i+1<<"," << tmpnode->key<<" ";
        }
    }
    SKNode *resultnode = tmpnode->forwards[0];
    // if (tmpnode==NIL){
    //     std::cout << "1,N"<< " ";
    // }else{
    //     std::cout << "1," << tmpnode->key << " ";
    // }
    if (resultnode->key == key)
    {
        // std::cout << tmpnode->val << std::endl;
        return resultnode;
    }
    else
    {
        // std::cout << "Not Found" << std::endl;
        return NULL;
    }
}

bool SkipList::Delete(uint64_t key)
{
    // TODO
    SKNode *tmpnode = head;
    SKNode *pre[MAX_LEVEL];
    for (int i = MAX_LEVEL - 1; i >= 0; i--)
    {
        while (tmpnode->forwards[i]->key < key)
        {
            tmpnode = tmpnode->forwards[i];
        }
        pre[i] = tmpnode;
    }
    if (pre[0]->forwards[0]->key != key)
    {
        return false;
    }
    for (int i = MAX_LEVEL - 1; i >= 0; i--)
    {
        if (pre[i]->forwards[i]->key == key)
        {
            pre[i]->forwards[i] = pre[i]->forwards[i]->forwards[i];
        }
    }
    unsigned long long newsize = size - pre[0]->forwards[0]->val.length() - sizeof(uint32_t) - sizeof(uint64_t);
    size = newsize;
    return true;
}

void SkipList::SecSearch(uint64_t key_start, uint64_t key_end, std::list<std::pair<uint64_t, std::string>> &slist)
{
    SKNode *tmpnode = Search(key_start);
    if (tmpnode == NULL)
    {
        while ((++key_start) < key_end)
        {
            if ((tmpnode = Search(key_start)) != NULL)
                break;
        }
        if (tmpnode == NULL)
            return;
    }
    do
    {
        slist.push_back(std::make_pair(tmpnode->key, tmpnode->val));
        tmpnode = tmpnode->forwards[0];
    } while (tmpnode->key <= key_end);
}

unsigned long long SkipList::Getsize()
{
    return size;
}

void SkipList::Getdata(uint64_t &num, uint64_t &minkey, uint64_t &maxkey)
{
    int tot = 0;
    bool flag = true;
    SKNode *tmpnode = head->forwards[0];
    if (tmpnode == NIL)
        return;
    minkey = tmpnode->key;
    while (tmpnode != NIL)
    {
        tot++;
        maxkey = tmpnode->key;
        tmpnode = tmpnode->forwards[0];
    }
    num = tot;
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