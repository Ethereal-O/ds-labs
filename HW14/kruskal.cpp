#include <iostream>
#include <vector>
#include <thread>
#include <map>
#include <mutex>
#include <condition_variable>
#include <sys/time.h> 
#include <fstream>
using namespace std;
#define THREAD_NUM 12
#define NODENUM 2000
#define EDGENUM 100000


struct edge
{
    edge(){}
    edge(int a,int b,int c):v1(a),v2(b),w(c),next(NULL){}
    int v1,v2;
    int w,tid;
    edge* next;
};

struct vertex
{
    vertex(int v):vid(v),next(NULL){}
    int vid;
    edge* next;
};

vector<vertex> graph;
vector<edge> mst;
vector<thread> subthreads;
multimap<int,edge> edge_queue;
mutex mut;
condition_variable cond_v[THREAD_NUM];
bool isfinished=false;

void construct();
void partition();
void subthread_func(vector<vertex> v,int tid);
void send_edge(multimap<int,edge>&m);
void add_edge(edge e, map<int,int>& i, map<int,vector<int>>&rev_i);
void kruskal();
void output();

void construct()
{
    ifstream file("./data/data+"+to_string(NODENUM)+"+"+to_string(EDGENUM));

    int v_num,a_num;
    file>>v_num>>a_num;
    for (int i=1;i<=v_num;i++)
        graph.push_back(vertex(i));
    for (int i=0;i<a_num;i++)
    {
        edge *tmp1=new edge(),*tmp2;
        file>>tmp1->v1>>tmp1->v2>>tmp1->w;
        tmp2=new edge(*tmp1);
        tmp1->next=graph[tmp1->v1-1].next;
        graph[tmp1->v1-1].next=tmp1;
        tmp2->next=graph[tmp2->v2-1].next;
        graph[tmp2->v2-1].next=tmp2;
    }

    printf("reading done\n");
    file.close();
}

void partition()
{
    vector<vertex> tmp[THREAD_NUM];
    for (int i=0;i<graph.size();i++)
        tmp[i%THREAD_NUM].push_back(graph[i]);
    for (int i=0;i<THREAD_NUM;i++)
    {
        subthreads.push_back(thread(subthread_func,tmp[i],i));
    }
}

void subthread_func(vector<vertex> v,int tid)
{
    multimap<int,edge> e;

    for(int i=0;i<v.size();i++)
    {
        edge* tmp=v[i].next;
        while (tmp!=NULL)
        {
            tmp->tid=tid;
            e.insert(pair<int,edge>(tmp->w,*tmp));
            tmp=tmp->next;
        }
    }

    unique_lock<mutex> lk(mut);
    send_edge(e);
    while(true)
    {
        cond_v[tid].wait(lk);
        if(isfinished) return;
        send_edge(e);
    }
}

void send_edge(multimap<int,edge>&m)
{
    if(!m.empty())
    {
        edge_queue.insert(*(m.begin()));
        m.erase(m.begin());
    }
}

void add_edge(edge e, map<int,int>& i, map<int,vector<int>>&rev_i)
{
    mst.push_back(e);
    int cid1=i[e.v1],cid2=i[e.v2];
    if(cid1==-1&&cid2==-1)
    {
        int cid=rev_i.size()+1;
        vector<int> tmp;
        tmp.push_back(e.v1);
        tmp.push_back(e.v2);
        rev_i[cid]=tmp;
        i[e.v1]=cid;
        i[e.v2]=cid;
    }
    else if(cid1==-1)
    {
        i[e.v1]=cid2;
        rev_i[cid2].push_back(e.v1);
    }
    else if(cid2==-1)
    {
        i[e.v2]=cid1;
        rev_i[cid1].push_back(e.v2);
    }
    else if(rev_i[cid1].size()<=rev_i[cid2].size())
    {
        for(int k=0;k<rev_i[cid1].size();k++)
        {
            i[rev_i[cid1][k]]=cid2;
            rev_i[cid2].push_back(rev_i[cid1][k]);
        }
        rev_i.erase(cid1);
    }
    else
    {
        for(int k=0;k<rev_i[cid2].size();k++)
        {
            i[rev_i[cid2][k]]=cid2;
            rev_i[cid1].push_back(rev_i[cid2][k]);
        }
        rev_i.erase(cid2);
    }
}

void kruskal()
{
    map<int,int> index;
    map<int,vector<int>> rev_index;
    for(int i=0;i<graph.size();i++)
        index[i+1]=-1;
    
    while(mst.size()<graph.size()-1)
    {
        printf("aaa");
        unique_lock<mutex> lk(mut);
        if(edge_queue.empty()) break;
        pair<int,edge> tmp=*(edge_queue.begin());
        edge_queue.erase(edge_queue.begin());
        lk.unlock();
        cond_v[tmp.second.tid].notify_all();
        if(index[tmp.second.v1]!=index[tmp.second.v2]||index[tmp.second.v1]==-1)
            add_edge(tmp.second,index,rev_index);
    }
    isfinished=true;
    printf("aaa");
    for (int i = 0; i < THREAD_NUM; i++)
    {
        cond_v[i].notify_all();
        subthreads[i].join();
    }
}

void output()
{
    printf("%ld\n",mst.size());
    // for (int i = 0; i < mst.size(); i++)
    //     cout << mst[i].v1 << " -> " << mst[i].v2 << endl;
}

int main()
{
    struct timeval start, finish;
    gettimeofday(&start,NULL);
    construct();
    partition();
    printf("bbb");
    kruskal();
    output();
    gettimeofday(&finish,NULL);
    printf("时间：%lf\n",((double)1000*(finish.tv_sec-start.tv_sec)+ (double)(finish.tv_usec -start.tv_usec)/1000));
    return 0;
}
