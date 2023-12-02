//Просто разделила все процессы на 2 части пересылала данные посредством отдельных коммутаторов 
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
    
    int firstRanks[] = {0,1,2,3,4}; //первая группа процессов
    MPI_Group firstProcces;
    MPI_Group_incl(mainGroup, 5, firstRanks, &firstProcces);
    MPI_Comm first;
    MPI_Comm_create(MPI_COMM_WORLD, firstProcces, &first);
    
    int secondRanks[] = {0,5,6,7,8,9}; //вторая группа процессов
    MPI_Group secondProcces;
    MPI_Group_incl(mainGroup, 6, secondRanks, &secondProcces);
    MPI_Comm second;
    MPI_Comm_create(MPI_COMM_WORLD, secondProcces, &second);


    if (ProcRank == 0){ 
        string filename = "../sourse/dog.jpg";
        vector<char> F = ReadFile(filename);
        int FileSize = F.size();
        int ThreadAmount = FileSize/B;
        int AdditionalAmount = FileSize%B;

        int FirstByte[B];
        int BytesAmount[B];
        
        for (auto i = 1; i <= B; ++i){
            if (i-1<AdditionalAmount) {
                FirstByte[i-1] = (ThreadAmount+1)*(i-1);
                BytesAmount[i-1] = ThreadAmount + 1;
            }
            else {
                FirstByte[i-1] = ThreadAmount*(i-1)+AdditionalAmount;
                BytesAmount[i-1] = ThreadAmount;
            }
            // MPI_Send(&BytesAmount[i-1], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            // MPI_Send(&F[FirstByte[i-1]], BytesAmount[i-1], MPI_CHAR, i, 1, MPI_COMM_WORLD);
            // // cout<<FirstByte<<" "<<LastByte<<endl;
        }

            for (auto i = 1; i < 5; ++i){
                MPI_Send(&BytesAmount[i-1], 1, MPI_INT, i, 0, first);
                MPI_Send(&F[FirstByte[i-1]], BytesAmount[i-1], MPI_CHAR, i, 1, first);
            }
            for (auto i = 5; i < 10; ++i){
                MPI_Send(&BytesAmount[i-1], 1, MPI_INT, i-4, 0, second);
                MPI_Send(&F[FirstByte[i-1]], BytesAmount[i-1], MPI_CHAR, i-4, 1, second);
            }
            
            char res[FileSize];
            for (auto i = 1; i < 5; ++i){
                MPI_Recv(&res[FirstByte[i-1]], BytesAmount[i-1], MPI_CHAR, i, 2, first, &status);
            }
            for (auto i = 5; i < 10; ++i){
                MPI_Recv(&res[FirstByte[i-1]], BytesAmount[i-1], MPI_CHAR, i-4, 2, second, &status);
            }
            
        ofstream result;
        result.open("output4");
        for(auto i = 0; i < FileSize; ++i){
            result<<res[i];
        }
        result.close();

        }
        else{
            int size;
            if (ProcRank<5){
                MPI_Recv(&size, 1, MPI_INT, 0, 0, first, &status);
                char Q[size];
                MPI_Recv(&Q, size, MPI_CHAR, 0, 1, first, &status);
                int x = B;
                for (auto i = 0; i < size; ++i){
                    Q[i] = (i*x)&255;
                }
                MPI_Send(&Q, size, MPI_CHAR, 0, 2, first);
            }
            if (ProcRank>=5){
                MPI_Recv(&size, 1, MPI_INT, 0, 0, second, &status);
                char Q[size];
                MPI_Recv(&Q, size, MPI_CHAR, 0, 1, second, &status);
                int x = B;
                for (auto i = 0; i < size; ++i){
                    Q[i] = (i*x)&255;
                }
                MPI_Send(&Q, size, MPI_CHAR, 0, 2, second);
            }
            
            
            
            // cout<<"Main rank = "<<ProcRank<<", subRank = "<<subRank<<endl;
        }
    
    MPI_Finalize();
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
