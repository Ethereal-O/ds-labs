#include <iostream>

#include "HashTable.h"

int HashTable::hash(int key)
{
    return key % BUCKET_SIZE;
}

void HashTable::Insert(int key, int value)
{
    // TODO
    int numtable = hash(key);
    HashNode* head = this->bucket[numtable];
    if (head == NULL) {
        this->bucket[numtable] = new HashNode(key,value);
        return;
    }
    while (head->next != NULL) {
        if (head->key == key) {
            head->val = value;
            return;
        }
        head =head->next;
    }
    if (head->key == key) {
        head->val = value;
        return;
    }
    else {
        head->next = new HashNode(key, value);
    }
}

void HashTable::Search(int key)
{
    // TODO
    int numtable = hash(key);
    HashNode* head = this->bucket[numtable];
    if (head == NULL) {
        std::cout << "Not Found"<<std::endl;
        return;
    }
    int i = 0;
    while (head->next != NULL) {
        if (head->key == key) {
            std::cout <<"bucket "<< numtable<<" index " << i<<" key " << head->key <<" value " << head->val<<std::endl;
            return;
        }
        head = head->next;
        i++;
    }
    if (head->key == key) {
        std::cout << "bucket " << numtable << " index " << i << " key " << head->key << " value " << head->val << std::endl;
        return;
    }
    else {
        std::cout << "Not Found" << std::endl;
    }
}

void HashTable::Delete(int key)
{
    // TODO
    int numtable = hash(key);
    HashNode* head = this->bucket[numtable];
    if (head == NULL) {
        return;
    }
    if (head->key == key) {
        this->bucket[numtable] = head->next;
        return;
    }
    while (head->next != NULL) {
        if (head->next->key == key) {
            head->next = head->next->next;
            return;
        }
        head = head->next;
    }
}

void HashTable::Display()
{
    for (int i = 0; i < BUCKET_SIZE; ++i)
    {
        std::cout << "|Bucket " << i << "|";
        HashNode *node = bucket[i];
        while (node)
        {
            std::cout << "-->(" << node->key << "," << node->val << ")";
            node = node->next;
        }
        std::cout << "-->" << std::endl;
    }
}