#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <string.h>

#include "Container.h"
#include "SkipList.h"

using namespace std;
SkipList skiplist;
string input_file_path="./input/skiplist_create_input1000";
int totalnum=1000;
double searchnum=10000;
void search(){
    double length=0;
    for(int i=0;i<searchnum;i++){
        int key = 1+rand()%totalnum;
        length+=skiplist.Search(key);
    }
    length/=searchnum;
    cout<<length<<endl;
}
int main(){
    ifstream inputData;
    inputData.open(input_file_path, ios::in);
    string line;
    while(inputData>>line){
        int bracketPos = line.find('(');
		string op = line.substr(0, bracketPos);
		string param = line.substr(bracketPos + 1, line.size() - bracketPos - 2);
        if (op == "Insert")
        {
            int commaPos = param.find(',');
            int key = atoi(param.substr(0, commaPos).c_str());
            int val = atoi(param.substr(commaPos + 1).c_str());
            skiplist.Insert(key, val);
        }
    }
    inputData.close();
    search();
}
