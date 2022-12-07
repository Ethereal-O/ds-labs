#include "FixedSP.h"
#include <thread>
#include <sys/time.h> 
#define threadNum 20

void newthread(int begin,int end,vector<int> &intermediates, vector<vector<int>> &length, vector<vector<vector<int>>>& allpath,FixedSP* fixedsp)
{
    for (int i=begin;i<end;i++)
    {
        int sourcenode = intermediates[i];
        Dijkstra(sourcenode, length[sourcenode], allpath[sourcenode],fixedsp);
    }
}

vector<int> FixedSP::getFixedPointShortestPath(int source, vector<int> intermediates)
{
    struct timeval start, finish;
    // 初始化
    vector<int> path;
    int n = graph.size();
    int m = intermediates.size();
    vector<vector<vector<int>>> allpath;
    vector<vector<int>> length;
    for (int i = 0; i < n; i++)
    {
        vector<vector<int>> singlepath(n);
        vector<int> singlelength(n);
        allpath.push_back(singlepath);
        length.push_back(singlelength);
    }

    gettimeofday(&start,NULL);
    for (int j = 0; j < 50; j++)
    {
        // 计算所有中间节点引发的路
        vector<thread> threads;
        Dijkstra(source, length[source], allpath[source], this);
        int blocklength = m / threadNum;
        for (int i = 0; i < threadNum; i++)
        {
            threads.push_back(thread(newthread, i * blocklength, i * blocklength + blocklength - 1, ref(intermediates), ref(length), ref(allpath), this));
        }
        if (blocklength * threadNum != m)
        {
            threads.push_back(thread(newthread, blocklength * threadNum, m, ref(intermediates), ref(length), ref(allpath), this));
        }
        for (int i = 0; i < threads.size(); i++)
        {
            threads[i].join();
        }
    }

    gettimeofday(&finish,NULL);
    printf("搜索时间：%lf\n",((double)1000*(finish.tv_sec-start.tv_sec)+ (double)(finish.tv_usec -start.tv_usec)/1000)/50);

    // // 回溯法计算所有路
    // vector<bool> isvisited(m);
    // vector<int> prepath;
    // int minlen = getMinPrePath(prepath, source, source, m, isvisited, intermediates, length);


    // // 不存在即返回
    // if (minlen >= INF)
    //     return path;

    // // 加入中间节点集
    // int beginnode = source;
    // for (int i = 1; i < prepath.size(); i++)
    // {
    //     path.push_back(beginnode);
    //     for (int j = 0; j < allpath[beginnode][prepath[i]].size(); j++)
    //     {
    //         path.push_back(allpath[beginnode][prepath[i]][j]);
    //     }
    //     beginnode = prepath[i];
    // }
    // path.push_back(source);
    return path;
}

int FixedSP::getMinPrePath(vector<int> &path, int source, int nownode, int restnum, vector<bool> isvisied, vector<int> &intermediates, vector<vector<int>> &length)
{
    // path-结果路径, source-源点, nownode-当前节点, restnum-剩余数目, isvisied-当前路已遍历的数组, intermediates-工具数组：中间节点集, length-工具数组：长度集
    // 结束条件
    if (restnum == 0)
    {
        path.push_back(nownode);
        path.push_back(source);
        return length[nownode][source];
    }

    // 回溯法遍历所有情况
    vector<int> minpath;
    int minlen = INF;
    int m = intermediates.size();
    for (int i = 0; i < m; i++)
    {
        if (!isvisied[i])
        {
            isvisied[i] = true;
            vector<int> tmppath;
            int tmplen;
            tmplen = getMinPrePath(tmppath, source, intermediates[i], restnum - 1, isvisied, intermediates, length);
            if ((tmplen + length[nownode][intermediates[i]]) < minlen)
            {
                minlen = tmplen + length[nownode][intermediates[i]];
                minpath = tmppath;
                minpath.insert(minpath.begin(), nownode);
            }
            isvisied[i] = false;
        }
    }

    // 更新最短路径，返回最短长度
    path = minpath;
    return minlen;
}

void Dijkstra(int source, vector<int> &length, vector<vector<int>> &path,FixedSP* fixedsp)
{
    // 计算长度并储存父亲
    int n = fixedsp->graph.size();
    bool *isconfirmed = new bool[n];
    int *parent = new int[n];
    for (int i = 0; i < n; i++)
    {
        length[i] = fixedsp->graph[source][i];
        isconfirmed[i] = false;
        if (length[i] == INF)
            parent[i] = -1;
        else
            parent[i] = source;
    }
    isconfirmed[source] = true;
    parent[source] = -1;
    for (int i = 1; i < n; i++)
    {
        int minlen = INF, minnode = source;
        for (int j = 0; j < n; j++)
        {
            if (!isconfirmed[j] && length[j] < minlen)
            {
                minnode = j;
                minlen = length[j];
            }
        }
        if (minnode == source)
            break;
        isconfirmed[minnode] = true;
        for (int j = 0; j < n; j++)
        {
            if (!isconfirmed[j] && fixedsp->graph[minnode][j] < INF)
            {
                if (length[j] > (length[minnode] + fixedsp->graph[minnode][j]))
                {
                    length[j] = length[minnode] + fixedsp->graph[minnode][j];
                    parent[j] = minnode;
                }
            }
        }
    }

    // 加载路径
    for (int i = 0; i < n; i++)
    {
        int nownode = parent[i];
        if (nownode == -1)
            continue;
        while (parent[nownode] != -1)
        {
            path[i].insert(path[i].begin(), nownode);
            nownode = parent[nownode];
        };
    }

    delete[] isconfirmed;
    delete[] parent;
}
