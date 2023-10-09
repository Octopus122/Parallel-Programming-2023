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
// void Write_in_file(vector<char> *Arr, int ThreadNumber);
void Write_in_file(vector<char> Vec);
vector<char> VectorConc(vector<char> *Arr, int ThreadNumber);
void TestProgram(std::string filaname); 

int B = 9;

int main(){
    std::string filename = "../sourse/cosmos.jpg";
    std::thread Threads[B];
    vector<char> F[B];
    int start = clock();
    for (auto i = 0; i < B; ++i){
        Threads[i] = std::thread(ThreadWork, filename, B, i, std::ref(F[i]));
    }
    for (auto i = 0; i < B; ++i){
        Threads[i].join();
    }
    int stop = clock();
    vector<char> Res = VectorConc(F, B);
    Write_in_file(Res);
    
    // std::cout<<"Time with thread = "<<stop - start<<std::endl;
    TestProgram(filename);

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
            // if (f.eof()) break;
            // std::cout<<ThreadCount<<std::endl; // для проверки, перемешиваются ли потоки
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