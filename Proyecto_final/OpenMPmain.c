// gcc OpenMPmain.c -fopenmp -lssl -lcrypto

#include <omp.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "Password.c"
#include "settings.c"

int main(){
    char* userPassword = getPassword(PASSWORD_SIZE);
    unsigned char userHash[32];
    SHA256(userPassword, strlen(userPassword), userHash);
    puts(userPassword);
    #pragma omp parallel
    {
        bruteForcePassword(userHash);
    }

    // Cleaning up
    free(userPassword);
}

void bruteForcePassword(unsigned char* userHash){
    for(size_t currentPasswordLength = 1; currentPasswordLength <= PASSWORD_SIZE; currentPasswordLength++){
        size_t possibleCombinations = elevateToPow(ALPHABET_LENGTH, currentPasswordLength);
        struct Password* possiblePassword = initTestPassword(currentPasswordLength);
        #pragma omp for
        for(int combination = 0; combination < possibleCombinations; combination++){
            unsigned char testHash[HASH_LENGTH];
            SHA256(possiblePassword->strVal, possiblePassword->length, testHash);
            int areHashesEqual = compareHashes(testHash, userHash);
            if(areHashesEqual){
                puts(possiblePassword->strVal);
            }
            incrementPassword(possiblePassword);
        }
        destroyPassword(possiblePassword);
    }
}
