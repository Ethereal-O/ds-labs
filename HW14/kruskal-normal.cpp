#include<bits/stdc++.h>	//POJ不支持
#include <fstream>
#include <sys/time.h> 
#define rep(i,a,n) for (int i=a;i<=n;i++)//i为循环变量，a为初始值，n为界限值，递增
#define per(i,a,n) for (int i=a;i>=n;i--)//i为循环变量， a为初始值，n为界限值，递减。
#define pb push_back
#define IOS ios::sync_with_stdio(false);cin.tie(0); cout.tie(0)
#define fi first
#define se second
#define mp make_pair
#define NODENUM 2000
#define EDGENUM 1000000

using namespace std;

const int inf = 0x3f3f3f3f;//无穷大
const int maxn = 1e5;//最大值。
typedef long long ll;
typedef long double ld;
typedef pair<ll, ll>  pll;
typedef pair<int, int> pii;
//*******************************分割线，以上为自定义代码模板***************************************//

struct edge{
	int s;//边的起始顶点。
	int e;//边的终端顶点。
	int w;//边权值。
	bool operator < (const edge &a){
		return w<a.w;
	}
};
edge temp[maxn];//临时数组存储边。
int verx[maxn];//辅助数组，判断是否连通。
edge tree[maxn];//最小生成树。
int n,m;//n*n的图，m条边。
int cnt;//统计生成结点个数，若不满足n个，则生成失败。
int sum;//最小生成树权值总和。
void print(){
	//打印最小生成树函数。
	// cout<<"最小生成树的权值总和为："<<sum<<endl;
	// rep(i,0,cnt-1){
	// 	cout<<tree[i].s<<" "<<tree[i].e<<"边权值为"<<tree[i].w<<endl;
	// }
}
void Kruskal(){
	rep(i,1,n)
		verx[i]=i;//这里表示各顶点自成一个连通分量。
	cnt=0;sum=0;
	sort(temp,temp+m);//将边按权值排列。
	int v1,v2;
	rep(i,0,m-1){
		v1=verx[temp[i].s];
		v2=verx[temp[i].e];
		if(v1!=v2){
			tree[cnt].s=temp[i].s;tree[cnt].e=temp[i].e;tree[cnt].w=temp[i].w;//并入最小生成树。
			rep(j,1,n){
				//合并v1和v2的两个分量，即两个集合统一编号。
				if(verx[j]==v2)verx[j]=v1; //默认集合编号为v2的改为v1.
			}
			sum+=tree[cnt].w;
			cnt++;
		}
	}
	//结束双层for循环之后得到tree即是最小生成树。
	print();
}
int main(){
	//freopen("in.txt", "r", stdin);//提交的时候要注释掉
    ifstream file("./data/data+"+to_string(NODENUM)+"+"+to_string(EDGENUM));
	IOS;

    struct timeval start, finish;
    gettimeofday(&start,NULL);
	while(file>>n>>m){
		int u,v,w;
		rep(i,0,m-1){
			file>>u>>v>>w;
			temp[i].s=u;temp[i].e=v;temp[i].w=w;
		}
		Kruskal();
	}
    gettimeofday(&finish,NULL);
    printf("时间：%lf\n",((double)1000*(finish.tv_sec-start.tv_sec)+ (double)(finish.tv_usec -start.tv_usec)/1000));
    
	return 0;
}

