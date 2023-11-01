#include<iostream>
#include<fstream>
#include<vector>
#include<typeinfo>
#include<string>

int B = 9;
using std::cout;
using std::vector;
using std::string;

int main(int argc, char *argv[]){
    int process_num = atoi(argv[1]);
    cout<<"child process # "<<process_num<<" started \n";
    
    //выбираем область для работы процесса
    int FileSize = atoi(argv[3]);
    cout<<FileSize<<std::endl;
    int ThreadAmount = FileSize/B; //целое от деления
    int AdditionalAmount = FileSize%B;
    int FirstByte, LastByte;
    if (process_num<AdditionalAmount) {
        FirstByte = (ThreadAmount+1)*process_num;
        LastByte = FirstByte + ThreadAmount + 1;
    }
    else {
        FirstByte = ThreadAmount*process_num+AdditionalAmount;
        LastByte = FirstByte + ThreadAmount;
    }
    // cout<<FirstByte<<" "<<LastByte<<std::endl;
    //процесс работает над частью файла
    vector<char> F;
    int x = B;
    for (auto i = FirstByte; i < LastByte; ++i){
        F.push_back((i*x)&255);
        // std::cout<<F[i]<<i<<std::endl;
    }

    //запись в отдельный файл
    std::ofstream f;
    f.open("child_files/process"+string(argv[1]), std::ifstream::binary);
    for (auto i = FirstByte; i < LastByte; ++i){
        f << F[i];
    }
    f.close();

    exit(0);
}