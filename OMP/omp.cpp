#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<thread>
#include<ctime>
#include <omp.h>


using std::vector;
using std::ifstream;
using std::ofstream;

vector<char> ReadFile(std::string FileName);
void ThreadWork(int ThreadNumber, int ThreadCount, vector<char> &Arr);
void WriteFile(std::string FileName, vector<char> F);
void TestProgram(std::string filaname); 

int B = 9;

int main(){
    std::string filename = "dog.jpg";
    vector<char> F = ReadFile(filename);
    int start = clock();
    #pragma omp parallel num_threads(9)
        {
            #pragma omp section
                ThreadWork(B, 0, F);
            #pragma omp section
                ThreadWork(B, 1, F);
            #pragma omp section
                ThreadWork(B, 2, F);
                #pragma omp section
                ThreadWork(B, 3, F);
            #pragma omp section
                ThreadWork(B, 4, F);
            #pragma omp section
                ThreadWork(B, 5, F);
                #pragma omp section
                ThreadWork(B, 6, F);
            #pragma omp section
                ThreadWork(B, 7, F);
            #pragma omp section
                ThreadWork(B, 8, F);

        }

    int stop = clock();
    WriteFile("output1.binary",F);
    
    std::cout<<"Time with thread = "<<stop - start<<std::endl;
    TestProgram(filename);

    return 0;
}

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
}

void ThreadWork(int ThreadNumber, int ThreadCount, vector<char> &Arr){
    int FileSize = Arr.size();
    int ThreadAmount = FileSize/ThreadNumber; //целое от деления
    int AdditionalAmount = FileSize%ThreadNumber;
    int FirstByte, LastByte;
    if (ThreadCount<AdditionalAmount) {
        FirstByte = (ThreadAmount+1)*ThreadCount;
        LastByte = FirstByte + ThreadAmount + 1;
    }
    else {
        FirstByte = ThreadAmount*ThreadCount+AdditionalAmount;
        LastByte = FirstByte + ThreadAmount;
    }
    
    int x = B;
    for (auto i = FirstByte; i < LastByte; ++i){
        Arr[i] = (i*x)&255;
        // std::cout<<ThreadCount<<std::endl;
    }
}

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
    }
    int stop = clock();

    WriteFile("output2.binary", F);
    
    std::cout<<"Computing time without threads = "<<stop - start<<std::endl;
}