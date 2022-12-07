#include <thread>
#include <random>
#include <vector>
#include <ctime>
#include <windows.h>

#define LENGTH 1000000
// #define LENGTH 100

void mergearray(int *data, int first, int mid, int last)
{
    int *tmp = new int[last - first + 1];
    int i = first, j = mid + 1, k = 0;
    while (i <= mid && j <= last)
    {
        if (data[i] <= data[j])
            tmp[k++] = data[i++];
        else
            tmp[k++] = data[j++];
    }
    while (i <= mid)
        tmp[k++] = data[i++];
    while (j <= last)
        tmp[k++] = data[j++];
    for (i = 0; i < k; i++)
        data[first + i] = tmp[i];
    delete[] tmp;
}

void mergesort(int *data, int first, int last)
{
    if (first < last)
    {
        int mid = (first + last) / 2;
        mergesort(data, first, mid);
        mergesort(data, mid + 1, last);
        mergearray(data, first, mid, last);
    }
}

void multimerge(int *data,int threadNum)
{
    int blocklength=LENGTH/threadNum;
    int *tmp=new int[LENGTH];
    int *nowindex=new int[threadNum+1]{0};
    for (int i=0;i<LENGTH;i++)
    {
        int nowmin=INT16_MAX;
        int nowminblock=0;
        for (int j=0;j<threadNum;j++)
        {
            if (nowindex[j]<blocklength&&data[blocklength*j+nowindex[j]]<nowmin)
            {
                nowmin=data[blocklength*j+nowindex[j]];
                nowminblock=j;
            }
        }
        if (nowindex[threadNum] < LENGTH-blocklength*threadNum && data[blocklength * threadNum + nowindex[threadNum]] < nowmin)
        {
            nowmin = data[blocklength * threadNum+ nowindex[threadNum]];
            nowminblock = threadNum;
        }
        tmp[i]=nowmin;
        nowindex[nowminblock]++;
    }

    for (int i=0;i<LENGTH;i++)
    {
        data[i]=tmp[i];
    }
    delete []tmp;
}

int main(int argc, char const *argv[])
{
    // if (argc != 2)
    // {
    //     fprintf(stderr, "Usage: %s <threadNum>\n", argv[0]);
    //     exit(0);
    // }

    int *data =new int[LENGTH];
    srand(1);
    for (int i = 0; i < LENGTH; i++)
    {
        data[i] = rand();
    }

    printf("enter threadNum:\n");
    int threadNum;
    scanf("%d",&threadNum);
    std::vector<std::thread> threads;
    int blocklength=LENGTH/threadNum;

    clock_t start,finish;
    start=clock();
    // SYSTEMTIME start,finish; 
    // GetLocalTime( &start ); 

    for (int i=0;i<threadNum;i++)
        threads.emplace_back(mergesort,data,i*blocklength,i*blocklength+blocklength-1);

    for (int i=0;i<threadNum;i++)
        threads[i].join();

    if (blocklength*threadNum!=LENGTH)
        mergesort(data,blocklength*threadNum,LENGTH-1);

    multimerge(data,threadNum);

    finish=clock();
    // GetLocalTime( &finish ); 

    printf("%llf\n",(double)(finish-start));
    // printf("%llf\n",(double)(finish.wMilliseconds-start.wMilliseconds));

    // for (int i=0;i<LENGTH;i++)
    // {
    //     printf("%d ",data[i]);
    // }
    return 0;
}
