#include<iostream>
#include<mpi.h>
#include<vector>
#include<fstream>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

int B = 9;
vector<char> ReadFile(std::string FileName);

int main(int argc, char* argv[]){
    int ProcNum, ProcRank;
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_size ( MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank ( MPI_COMM_WORLD, &ProcRank);
    MPI_Group mainGroup; 
    MPI_Comm_group(MPI_COMM_WORLD, &mainGroup);//Тут идентифицировали группу со всеми процессами

    //Создаём нужные группы и коммуникаторы///////////////////////////
    
    int BaseRanks[] = {0,1,2,3,4,5,6,7,8}; //первая группа процессов -> приём информации и обработка
    MPI_Group BaseG;
    MPI_Group_incl(mainGroup, 9, BaseRanks, &BaseG);
    MPI_Comm Base;
    MPI_Comm_create(MPI_COMM_WORLD, BaseG, &Base);

    int RestRanks[] = {0,9};//вторая группа процессов запись
    MPI_Group RestG;
    MPI_Group_incl (mainGroup, 2, RestRanks, &RestG);
    MPI_Comm Rest;
    MPI_Comm_create(MPI_COMM_WORLD, RestG, &Rest);

    vector<char> F;
    int FileSize;
    int ThreadAmount;
    int AdditionalAmount;

    if (ProcRank == 0){
        string filename = "../sourse/dog.jpg";
        F = ReadFile(filename);
        FileSize = F.size();
        ThreadAmount = FileSize/B;
        AdditionalAmount = FileSize%B;
        MPI_Bcast(&ThreadAmount, 1, MPI_INT, 0, Base);  
        cout<<FileSize<<" "<<ThreadAmount;
    }
    char Result[FileSize]; 
    if (ProcRank < 9) {
        MPI_Bcast(&ThreadAmount, 1, MPI_INT, 0, Base);
        char work [ThreadAmount];
        MPI_Scatter(&F[0], ThreadAmount, MPI_CHAR, &work[0], ThreadAmount, MPI_CHAR, 0, Base);
        for (auto i = 0; i < ThreadAmount; ++i){
            work[i] = (i*B)&255;
        }
        MPI_Gather (&work[0], ThreadAmount, MPI_CHAR, &Result[0], ThreadAmount, MPI_CHAR, 0, Base); 
    }
    
    if (ProcRank == 0){
    ofstream result;
        result.open("output10");
        for(auto i = 0; i < FileSize-3; ++i){
            result<<Result[i];
        }
        result.close();
    }
    
    MPI_Finalize();  
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
