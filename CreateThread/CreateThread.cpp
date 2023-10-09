#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<ctime>
#include <windows.h>

using std::vector;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::cin;
using std::endl;

int B = 9;
LPDWORD threadID [9];

struct threadDATA {
    std::string filename;
    int ThreadNumber;
    int ThreadCount;
    vector<char> &Arr;
};

DWORD WINAPI ThreadWork(void *params){
    threadDATA * d = (threadDATA*)params;
    ifstream f;
    f.open(d->filename, ifstream::binary);
    f.seekg(0, std::ios_base::end);
    int FileSize = f.tellg();
    int FirstByte = FileSize/d->ThreadNumber*d->ThreadCount;
    f.seekg(FirstByte, std::ios_base::beg);
    if (f){
        char s;
        for (auto i = FirstByte; i < FirstByte+FileSize/d->ThreadNumber; ++i){
            // cout<<d->ThreadCount<<endl; // для проверки, перемешиваются ли потоки
            // if (f.eof()) break;
            f >> s;
            d->Arr.push_back(s);
            }
    }
    int x = B;
    for (auto i = 0; i < d->Arr.size(); ++i){
        d->Arr[i] = (i*x)&255;
    }
    f.close();
    return 0;
};
void Write_in_file(vector<char> Vec)
{
    ofstream f;
    f.open("output2", std::ifstream::binary);
    for (auto i = 0; i < Vec.size(); ++i){
        f << Vec[i];
    }
}

vector<char> VectorConc(vector<char> *Arr, int ThreadNumber)
{
    vector<char> Res = Arr[0];
    for (auto i = 1; i < ThreadNumber; ++i){
        Res.insert(Res.end(), Arr[i].begin(), Arr[i].end());
    }
    return Res;
}

void TestProgram(std::string filename){
    int start = clock();
    ifstream f;
    f.open(filename,std::ifstream::binary);
    // f.seekg(0, std::ios_base::beg);
    vector<char> F;
    f.seekg(0, std::ios_base::end);

    int LastByte = f.tellg();
    f.seekg(0, std::ios_base::beg); 
    if (f){
        char s;
        for (auto i = 0; i < LastByte; ++i){
            F.push_back(s);
            }
    }
    f.close();
    int stop = clock();
    ofstream f1;
    f1.open("output1", std::ifstream::binary);
    int x = B;
    for(int i = 0; i < F.size();++i){
        F[i] = (i*x)&255;
        f1<<F[i];
    }
    f1.close();
    
    // std::cout<<"Time without threads = "<<stop - start<<std::endl;
}


int main(){
    std::string filename = "../sourse/cosmos.jpg";
    vector<char> F[B];

    int start = clock();
    HANDLE Threads[B]; 
    for (auto i = 0; i < B; ++i){
        threadDATA *p = new threadDATA {filename, B, i, F[i]};
        Threads[i] = CreateThread(NULL, 0, ThreadWork, (void*)p, 0, threadID[i]);
    }

    WaitForMultipleObjects(B,Threads, true, INFINITE);
    int stop = clock();

    vector<char> Res = VectorConc(F, B);
    Write_in_file(Res);
    
    // std::cout<<"Time with thread = "<<stop - start<<std::endl;
    TestProgram(filename);

    return 0;
}



