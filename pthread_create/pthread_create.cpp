#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<ctime>
// #include <windows.h>
#include<pthread.h>

using std::vector;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cin;
using std::endl;

int B = 9;
long threadID[9];

struct threadDATA {
    int ThreadCount; //номер потока
    int ThreadNumber; //число потоков
    vector<char>&Arr;
};
vector<char> ReadFile(std::string FileName){
    vector<char> F;

    std::ifstream f;
    f.open(FileName, std::ifstream::binary);
    if (f){
        char s;
        for (f >> s; !f.eof(); f >> s){
            F.push_back(s);
            }
    }
    f.close();
    return F;
};

void* ThreadWork(void *params){
    // cout<<d->ThreadCount<<endl; // для проверки, перемешиваются ли потоки
    threadDATA * d = (threadDATA*)params;
    int FileSize = d->Arr.size();
    int ThreadAmount = FileSize/d->ThreadNumber; //целое от деления
    int AdditionalAmount = FileSize%d->ThreadNumber;
    int FirstByte, LastByte;
    if (d->ThreadCount<AdditionalAmount) {
        FirstByte = (ThreadAmount+1)*d->ThreadCount;
        LastByte = FirstByte + ThreadAmount + 1;
    }
    else {
        FirstByte = ThreadAmount*d->ThreadCount+AdditionalAmount;
        LastByte = FirstByte + ThreadAmount;
    }
    
    int x = B;
    for (auto i = FirstByte; i < LastByte; ++i){
        d->Arr[i] = (i*x)&255;
        // std::cout<<d->ThreadCount<<std::endl;
    }
    return 0;
};


void WriteFile(std::string FileName, vector<char> F){
    ofstream f;
    f.open(FileName, std::ifstream::binary);
    for (auto i = 0; i < F.size(); ++i){
        f << F[i];
    }
    f.close();
}

void TestProgram(std::string filename){

    ifstream f;
    f.open(filename,std::ifstream::binary);
    // f.seekg(0, std::ios_base::beg);
    vector<char> F;
    if (f){
        char s;
        for (f >> s ; !f.eof(); f >> s){
            F.push_back(s);
            }
    }
    f.close();

    int start = clock();
    int x = 9;
    for(int i = 0; i < F.size();++i){
        F[i] = (i*x)&255;
        // std::cout<<3<<std::endl;
    }
    int stop = clock();

    WriteFile("output2", F);
    
    std::cout<<"Computing time without threads = "<<stop - start<<std::endl;
};

int main(){
    std::string filename = "dog.jpg";
    vector<char> F = ReadFile(filename);
    int start = clock();
    
    pthread_t Threads[B];
    void * status;
    // cout<<"work";
    for (auto i = 0; i < B; ++i){
        threadDATA *p = new threadDATA {i,B, F};
        pthread_create(&Threads[i], NULL, &ThreadWork,(void*)p);
        // delete p;
    }
    // cout<<"work";
    for (auto i = 0; i < B; ++i){
        pthread_join(Threads[i], &status);
    }
    int stop = clock();
    WriteFile("output1.binary",F);
    
    TestProgram(filename);

    return 0;
}
