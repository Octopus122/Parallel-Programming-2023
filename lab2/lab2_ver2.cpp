#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<thread>
#include<ctime>

using std::vector;
using std::ifstream;
using std::ofstream;

void ThreadWork(std::string filaname, int ThreadNumber, int ThreadCount, vector<char> &Arr);
void Write_in_file(vector<char> *Arr, int ThreadNumber);

int B = 9;

int main(){
    std::string filename = "../sourse/cat2.jpg";
    std::thread Threads[B];
    vector<char> F[B];
    for (auto i = 0; i < B; ++i){
        Threads[i] = std::thread(ThreadWork, filename, B, i, std::ref(F[i]));
    }
    for (auto i = 0; i < B; ++i){
        Threads[i].join();
    }
    Write_in_file(F,B);

    return 0;
}

void ThreadWork(std::string filename, int ThreadNumber, int ThreadCount, vector<char> &Arr){
    ifstream f;
    f.open(filename, ifstream::binary);
    f.seekg(0, std::ios_base::end);
    int FileSize = f.tellg();
    int FirstByte = FileSize/ThreadNumber*ThreadCount;
    f.seekg(FirstByte, std::ios_base::beg);
    if (f){
        char s;
        for (auto i = FirstByte; i < FirstByte+FileSize/ThreadNumber; ++i){
            if (f.eof()) break;
            f >> s;
            Arr.push_back(s);
            }
    }
    int x = 9;
    for (auto i = 0; i < Arr.size(); ++i){
        Arr[i] = (i*x)&255;
    }
    f.close();
}

void Write_in_file(vector<char> *Arrs, int ThreadNumber){
    ofstream f;
    f.open("output2", std::ifstream::binary);
    for (auto i = 0; i < ThreadNumber; ++i){
        for (auto j = 0; i < Arrs[i].size(); ++j){
            f << Arrs[i][j];
        }
    }
    f.close();
}
