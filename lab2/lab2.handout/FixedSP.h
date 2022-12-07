#include <vector>
#include <string>
#include <string.h>

using namespace std;

#define INF 1e8

class FixedSP
{
private:
    vector<vector<int>> graph;
    int getMinPrePath(vector<int>&path,int source,int nownode,int restnum,vector<bool>isvisied,vector<int> &intermediates,vector<vector<int>>&length);
    void Dijkstra(int source,  vector<int>&length,vector<vector<int>>&path);
public:
    FixedSP(vector<vector<int>> matrix):graph(matrix) {}
    ~FixedSP() {}

    vector<int> getFixedPointShortestPath(int source, vector<int> intermediates);
};
