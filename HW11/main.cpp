#include <cstdio>
#include <cmath>
#include <ctime>
#include <string>
#include <random>

#define TINITLENGTH 10000
#define TTIMES 4
#define PINITLENGTH 10
#define PTIMES 3
#define NTIMES 3

#define DATALENGTH 3

const char data[DATALENGTH]={'a','b','c'};
long long int tlength;
long long int plength;
std::string tstring[NTIMES];
std::string pstring;
int *next;

clock_t start,finish;
double resulttime;

void kmpinit()
{
    next=new int[plength];
    int t=next[0]=-1,j=0;
    while (j<plength-1)
    {
        if(0>t||pstring[j]==pstring[t])
        {
            j++;
            t++;
            next[j]=(pstring[j]!=pstring[t]?t:next[t]);
        }else{
            t=next[t];
        }
    }
}

void stringinit()
{
    srand(1);
    for (int i=0;i<NTIMES;i++)
    {
        tstring[i]="";
        for (int j=0;j<tlength;j++)
        {
            int index=rand()%DATALENGTH;
            tstring[i]+=data[index];
        }
    }

    pstring="";
    for (int j = 0; j < plength; j++)
    {
        int index = rand() % DATALENGTH;
        pstring += data[index];
    }
}

int kmp(int index)
{
    int i=0;
    int j=0;
    while(i<tlength&&j<plength)
    {
        if(0>j||tstring[index][i]==pstring[j])
        {
            i++;
            j++;
        }else{
            j=next[j];
        }
    }

    
    return i-j;
}

int simple(int index)
{
    bool flag=false;
    for (int i=0;i<tlength;i++)
    {
        for (int j=0;j<plength;j++)
        {
            if((i+j)>tlength) break;
            if (tstring[index][i+j]!=pstring[j]) break;
            if (j==plength-1) flag=true;
        }
        if (flag) return i;
    }
    return tlength;
}

int main()
{
    for (int i=0;i<TTIMES;i++)
    {
        for (int j=0;j<PTIMES;j++)
        {
            tlength=TINITLENGTH*pow(10,i);
            plength=PINITLENGTH*pow(10,j);
            stringinit();
            printf("-------当前字符串长度为%d，模式串长度为%d-------\n",tlength,plength);
            start=clock();
            kmpinit();
            for (int k=0;k<NTIMES;k++)
            {
                kmp(k);
                // printf("%d\n",kmp(k));
            }
            delete []next;
            finish=clock();
            resulttime=(double)(finish-start);
            printf("kmp算法所用时间为%llf\n",resulttime/NTIMES);
            start=clock();
            for (int k=0;k<NTIMES;k++)
            {
                simple(k);
                // printf("%d\n",simple(k));
            }
            finish=clock();
            resulttime=(double)(finish-start);
            printf("朴素算法所用时间为%llf\n",resulttime/NTIMES);
        }
    }
}