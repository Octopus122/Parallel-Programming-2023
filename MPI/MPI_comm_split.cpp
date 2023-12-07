//Просто разделила все процессы на 2 части? пересылала данные посредством отдельных коммуникаторов, только теперь с помощью comm_split
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
int x = 9;
vector<char> ReadFile(std::string FileName);

int main(int argc, char* argv[]){
    int ProcNum, ProcRank;
    MPI_Status status;
    MPI_Init( &argc, &argv );
    MPI_Comm_size ( MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank ( MPI_COMM_WORLD, &ProcRank);

    MPI_Comm first;
    MPI_Comm second;
    

    if (ProcRank == 0){ 
        string filename = "../sourse/dog.jpg";
        vector<char> F = ReadFile(filename);
        int FileSize = F.size();
        int ThreadAmount = FileSize/B;
        int AdditionalAmount = FileSize%B;

        //Отслылаем количество процессов с числом отсылаемых символов на 1 больше
        MPI_Bcast(&AdditionalAmount, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&ThreadAmount, 1, MPI_INT, 0, MPI_COMM_WORLD);

        MPI_Comm_split(MPI_COMM_WORLD, 1, 0, &first);
        MPI_Comm_split(MPI_COMM_WORLD, 1, 0, &second);

        for (auto i = 1; i <= AdditionalAmount; ++i ){
            MPI_Send(&F[(i-1)*(ThreadAmount+1)], ThreadAmount+1, MPI_CHAR, i, 0, first);
        } 

        for (auto i = 1; i <= B - AdditionalAmount; ++i ){
            MPI_Send(&F[(i-1)*ThreadAmount+AdditionalAmount*(ThreadAmount+1)], ThreadAmount, MPI_CHAR, i, 0, second);
        } 

        char res[FileSize];

        for (auto i = 1; i <= AdditionalAmount; ++i){
            MPI_Recv(&res[(i-1)*(ThreadAmount+1)], ThreadAmount+1, MPI_CHAR, i, 2, first, &status);
        }
        for (auto i = 1; i <= B - AdditionalAmount; ++i ){
            MPI_Recv(&res[(i-1)*ThreadAmount+AdditionalAmount*(ThreadAmount+1)], ThreadAmount, MPI_CHAR, i, 2, second, &status);
        } 
        
        ofstream result;
        result.open("output5");
        for(auto i = 0; i < FileSize; ++i){
            result<<res[i];
        }
        result.close();

        }
        else{
            int size;
            int additional;
            MPI_Bcast(&additional, 1, MPI_INT, 0, MPI_COMM_WORLD);
            MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);
            // cout<<additional;
            if (additional>=ProcRank){
                MPI_Comm_split(MPI_COMM_WORLD, 1, 0, &first);
                MPI_Comm_split(MPI_COMM_WORLD, 2, 0, &second);
                char Q[size+1];
                MPI_Recv(&Q, size+1, MPI_CHAR, 0, 0, first, &status);
                for (auto i = 0; i < size; ++i){
                    Q[i] = (i*x)&255;
                }
                MPI_Send(&Q, size, MPI_CHAR, 0, 2, first);

            }
            else{
                MPI_Comm_split(MPI_COMM_WORLD, 2, 0, &first);
                MPI_Comm_split(MPI_COMM_WORLD, 1, 0, &second);
                char Q[size+1];
                MPI_Recv(&Q, size+1, MPI_CHAR, 0, 0, second, &status);
                for (auto i = 0; i < size; ++i){
                    Q[i] = (i*x)&255;
                }
                MPI_Send(&Q, size, MPI_CHAR, 0, 2, second);

            }

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
