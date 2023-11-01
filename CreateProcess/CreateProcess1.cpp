#include<iostream>
#include<Windows.h>
#include<stdio.h>
#include<conio.h>
#include<fstream>
#include<vector>

int B = 9;

using std::cout;
using std::endl;
using std::wstring;
using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;

vector<char> ReadFile(std::string FileName);

int main(){
    string filename = "../sourse/dog.jpg";
    vector<char> Q = ReadFile(filename);


    STARTUPINFO sti[B];
    PROCESS_INFORMATION pi[B];
    for (auto i = 0; i < B; ++i){
        ZeroMemory( &sti[i], sizeof(sti[i]) );
        sti[i].cb = sizeof(sti[i]);
        ZeroMemory( &pi[i], sizeof(pi[i]) );
    }

    char cmd[256];

    // cout<<"PARENT PROCESS\n";
    cout<<Q.size()<<endl;
    for (auto i = 0; i < B; ++i){
        std::sprintf(cmd, "child2.exe %d %d %d", i, Q, Q.size());
        if(!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &sti[i], &pi[i])){
            cout<< "cant create child process #"<< i <<endl;
            return -1;
    }
    }
    
    for (auto i = 0; i < B; ++i){
        WaitForSingleObject(pi[i].hProcess, INFINITE);
    }
    cout<<"end parent process"<<endl;

    //запись результата
    ofstream res;
    res.open("output1");
    for (auto i = 0; i < B; ++i){
        ifstream f0("child_files/process"+std::to_string(i), std::ifstream::binary);
        if (f0){
            char s;
            for (f0 >> s; !f0.eof(); f0 >> s){
                res << s;
            }
        }
        f0.close();

    }
    res.close();
    return 0;
}

vector<char> ReadFile(std::string FileName){
    vector<char> F;

    ifstream f(FileName, std::ifstream::binary);
    // f.open(FileName, std::ifstream::binary);
    if (f){
        char s;
        for (f >> s; !f.eof(); f >> s){
            F.push_back(s);
            }
    }
    f.close();
    return F;
};