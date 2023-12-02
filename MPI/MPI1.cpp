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
            MPI_Send(&BytesAmount[i-1], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&F[FirstByte[i-1]], BytesAmount[i-1], MPI_CHAR, i, 1, MPI_COMM_WORLD);
            // cout<<FirstByte<<" "<<LastByte<<endl;
        }
        
        char res[FileSize];
        for (auto i = 1; i <= B; ++i){
            MPI_Recv(&res[FirstByte[i-1]], BytesAmount[i-1], MPI_CHAR, i, 2, MPI_COMM_WORLD, &status);
        }
        
        ofstream result;
        result.open("output1");
        for(auto i = 0; i < FileSize; ++i){
            result<<res[i];
        }
        result.close();


        
        // cout<<"picsize = "<<FileSize<<endl;
        
        }
        else{
            int size;
            MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            char Q[size];
            MPI_Recv(&Q, size, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &status);
            int x = B;
            for (auto i = 0; i < size; ++i){
                Q[i] = (i*x)&255;
            }
            MPI_Send(&Q, size, MPI_CHAR, 0, 2, MPI_COMM_WORLD);
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
