#include <iostream>
#include <string>
#include <ctime>
#include "cuckoo.h"

#define MAX_LENGTH 100000
#define PR 1.0
#define TIMES 1000

using namespace std;
int main()
{
    clock_t start,finish;
    cuckoo_hash cuckoo;

    for(int i=0;i<PR*MAX_LENGTH;i++)
    {
        cuckoo.Insert(i,i);
    }

    start=clock();
    for (int i=0;i<TIMES;i++)
    {
        for (int j=0;j<PR*MAX_LENGTH;j++)
        {
            cuckoo.Lookup(j);
        }
    }
    finish=clock();
    
    printf("%lf\n",(double)(finish-start)/(TIMES*PR));
}