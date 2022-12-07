#include <vector>
#include <string>
#include <string.h>

using namespace std;

#define INF 1e8

class FixedSP
{
private:
    int getMinPrePath(vector<int>&path,int source,int nownode,int restnum,vector<bool>isvisied,vector<int> &intermediates,vector<vector<int>>&length);
public:
    vector<vector<int>> graph;
    FixedSP(vector<vector<int>> matrix):graph(matrix) {}
    ~FixedSP() {}
    vector<int> getFixedPointShortestPath(int source, vector<int> intermediates);
};
void newthread(int begin,int end,vector<int> &intermediates, vector<vector<int>> &length, vector<vector<vector<int>>>& allpath,FixedSP* fixedsp);
static void Dijkstra(int source,  vector<int>&length,vector<vector<int>>&path,FixedSP* fixedsp);
