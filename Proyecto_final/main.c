// mpicc main.c -fopenmp
// mpirun -f machines -n 2 ./a.out

#include "mpi.h"
#include <omp.h>
#include "main.h"

static int startFrom;

int main(int argc, char** argv){
    char* userPassword = getPassword(PASSWORD_SIZE);
    unsigned char userHash[HASH_LENGTH];
    //SHA256(userPassword, strlen(userPassword), userHash);
    puts(userPassword);
    
    int numtasks, id, rc;
    rc = MPI_Init(&argc, &argv);
    if(rc != MPI_SUCCESS){
        puts("Error con MPI");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    startFrom = id == 0 ? 0: 1;
    bruteForcePassword(userHash);

    // Cleaning up
    free(userPassword);
    MPI_Finalize();
}

void bruteForcePassword(unsigned char* userHash){
    size_t possibleCombinations = elevateToPow(ALPHABET_LENGTH, PASSWORD_SIZE);
    #pragma omp parallel for shared(possibleCombinations)
    for(int combination = startFrom; combination < possibleCombinations; combination+=2){
        testPassword(userHash, combination);
    }
}
