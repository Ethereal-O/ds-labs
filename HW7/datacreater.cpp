#include <fstream>
#include <random>
#include <vector>
#include <string.h>

const int datasize[5]={50,100,200,500,1000};
int main(){
    for (int i=0;i<5;i++)
    {
        // sequence
        std::ofstream filesequence("./data/sequence"+std::to_string(i)+".txt");
        for (int j=0;j<datasize[i];j++)
        {
            filesequence<<j<<"\n";
        }
        filesequence.close();

        // random
        std::ofstream filerandom("./data/random"+std::to_string(i)+".txt");
        bool hash[datasize[i]];
        memset(hash,0,sizeof(hash));
        for (int j=0;j<datasize[i];j++)
        {
            int newdata=rand()%datasize[i];
            while (hash[newdata])
            {
                newdata=rand()%datasize[i];
            }
            hash[newdata]=true;
            filerandom<<newdata<<"\n";
        }
        filerandom.close();
    }
}