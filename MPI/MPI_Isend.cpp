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

    MPI_Request req[B];

    if (ProcRank == 0){
        string filename = "../sourse/dog.jpg";
        vector<char> F = ReadFile(filename);
        int FileSize = F.size();
        int ThreadAmount = FileSize/B;
        int AdditionalAmount = FileSize%B;

        int FirstByte[B];
        int BytesAmount[B];

        for (auto i = 1; i <= B; ++i){ //передача информации о размере передаваемой части массива
            if (i-1<AdditionalAmount) {
                FirstByte[i-1] = (ThreadAmount+1)*(i-1);
                BytesAmount[i-1] = ThreadAmount + 1;
            }
            else {
                FirstByte[i-1] = ThreadAmount*(i-1)+AdditionalAmount;
                BytesAmount[i-1] = ThreadAmount;
            }
            MPI_Isend(&BytesAmount[i-1], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &req[i-1]);
        }

        for (auto i = 1; i <= B; ++i){
            MPI_Isend(&F[FirstByte[i-1]], BytesAmount[i-1], MPI_CHAR, i, 1, MPI_COMM_WORLD, &req[i-1]);
        }
        
        char res[FileSize];
        for (auto i = 1; i <= B; ++i){
            MPI_Irecv(&res[FirstByte[i-1]], BytesAmount[i-1], MPI_CHAR, i, 2, MPI_COMM_WORLD, &req[i-1]);
        }

        MPI_Status stat[B];
        MPI_Waitall(B, req,  stat);
        ofstream result;
        result.open("output2");
        for(auto i = 0; i < FileSize; ++i){
            result<<res[i];
        }
        result.close();
        // cout<<"picsize = "<<FileSize<<endl;
        
        }
        else{
            int size;
            // MPI_Recv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Irecv(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req[ProcRank-1]);
            MPI_Wait(&req[ProcRank-1], &status);
            char Q[size];

            MPI_Irecv(&Q, size, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &req[ProcRank-1]);
            MPI_Wait(&req[ProcRank-1], &status);
            int x = B;
            for (auto i = 0; i < size; ++i){
                Q[i] = (i*x)&255;
            }
            MPI_Isend(&Q, size, MPI_CHAR, 0, 2, MPI_COMM_WORLD, &req[ProcRank-1]);
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
