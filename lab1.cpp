#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<thread>

int main(){
    std::string filename = "sourse/cat1.jpg";
    std::ifstream f;
    f.open(filename, std::ifstream::binary);
    std::vector<char> F;
    if (f){
        char s;
        for(f >> s; !f.eof(); f >> s){
            F.push_back(s);
        }
    }
    f.close();
    std::ofstream f1;
    f1.open("output", std::ifstream::binary);
    int x = 5;
    for(int i = 0; i<F.size();++i){
        F[i] = (i*x)&255;
        f1<<F[i];
    }
    f1.close();

    return 0;
}