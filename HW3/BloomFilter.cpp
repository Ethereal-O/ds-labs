#include<functional>
#include<iostream>
#include<cstring>
using namespace std;
const int n=100;
const int m=4*n;
const int k=1;
int myhash1(int num){
    return ((num%101)*(num%97)*(num%103)*(num%91)*(num%107)*(num%89))%m;
}
int myhash2(int num){
    srand(num);
    return rand()%m;
}
int main(){
    hash<int> shash;
    bool*dest=new bool[m];
    double errnum=0;
    memset(dest,0,sizeof(bool)*m);
    for (int i=0;i<n;i++){
        for (int j=1;j<=k;j++){
            dest[myhash2(i*j)]=1;
        }
    }
    for (int i=n;i<200;i++){
        for (int j=1;j<=k;j++){
            if(dest[myhash2(i*j)]==0){
                break;
            }
            if(j==k){
                errnum++;
            }
        }
    }
    //delete []dest;
    cout<<errnum/100.0;
}