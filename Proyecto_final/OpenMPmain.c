// gcc OpenMPmain.c -fopenmp -lssl -lcrypto

#include <omp.h>
#include "main.h"

int main(){
    char* userPassword = getPassword(PASSWORD_SIZE);
    unsigned char userHash[HASH_LENGTH];
    SHA256(userPassword, strlen(userPassword), userHash);
    puts(userPassword);
    bruteForcePassword(userHash);

    // Cleaning up
    free(userPassword);
}

void bruteForcePassword(unsigned char* userHash){
    size_t possibleCombinations = elevateToPow(ALPHABET_LENGTH, PASSWORD_SIZE);
    #pragma omp parallel for shared(possibleCombinations)
    for(int combination = 0; combination < possibleCombinations; combination++){
        testPassword(userHash, combination);
    }
}
