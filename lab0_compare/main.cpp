#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <string.h>

#include "Container.h"
#include "HashTable.h"
#include "SkipList.h"
#include<ctime>
#include<unistd.h>

using namespace std;

HashTable hashtable;
SkipList skiplist;
string input_file_path="./input/skiplist_create_input1000";
void test(){
    int left=100,right=600;
    bool isconsist=true;
    clock_t start,end;
    double cost;
    start=clock();
    for(int i=0;i<=10000;i++){
        hashtable.Search(left,right);
    }
    end=clock();
    cost=(double)(end-start)/CLOCKS_PER_SEC;
    cout<<cost<<endl;
    // start=clock();
    // long i=100000000L;
    // while(i--);
    // //sleep(10);
    // end=clock();
    // cost=(double)(end-start)/CLOCKS_PER_SEC;
    // cout<<cost<<endl;
    start=clock();
    for(int i=0;i<=10000;i++){
        if(isconsist){
            skiplist.Search(left,right);
        }else{
            for(int j=left;j<=right;j++){
            skiplist.Search(j);
        }
        }
    }
    end=clock();
    cost=(double)(end-start)/CLOCKS_PER_SEC;
    cout<<cost<<endl;
}
int main(){
    ifstream inputData;
    inputData.open(input_file_path, ios::in);
    string line;
    while (inputData >> line)
    {
		int bracketPos = line.find('(');
		string op = line.substr(0, bracketPos);
		string param = line.substr(bracketPos + 1, line.size() - bracketPos - 2);
        if (op == "Insert")
        {
            int commaPos = param.find(',');
            int key = atoi(param.substr(0, commaPos).c_str());
            int val = atoi(param.substr(commaPos + 1).c_str());
            hashtable.Insert(key, val);
            skiplist.Insert(key,val);
        }
    }
    inputData.close();
    test();
}