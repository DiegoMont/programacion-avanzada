//#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

const int PASSWORD_SIZE = 5;

char const alphabet[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};
const size_t ALPHABET_LENGTH = 26;
const size_t HASH_LENGTH = 32;

int main(){
    char* userPassword = getPassword(PASSWORD_SIZE);
    unsigned char userHash[32];
    //SHA256(userPassword, PASSWORD_SIZE, userHash);
    puts(userPassword);
    puts(userHash);
    bruteForcePassword(userHash);
    // Cleaning up
    free(userPassword);
}

char* getPassword(int passwordSize){
    printf("Ingrese una contraseña de %d caracteres que contenga solo letras minúsculas: ", passwordSize);
    char* password = (char*) malloc(passwordSize * sizeof(char));
    strcpy(password, "aaaaa");
    //scanf("%s", password);
    return password;
}

void bruteForcePassword(unsigned char* userHash){
    for(size_t i = 0; i < ALPHABET_LENGTH; i++){
        char testPassword[PASSWORD_SIZE];
        testPassword[0] = *(alphabet + i);
        for(size_t j = 0; j < ALPHABET_LENGTH; j++){
            testPassword[1] = *(alphabet + j);
            for(size_t k = 0; k < ALPHABET_LENGTH; k++){
                testPassword[2] = *(alphabet + k);
                for(size_t l = 0; l < ALPHABET_LENGTH; l++){
                    testPassword[3] = *(alphabet + l);
                    for(size_t m = 0; m < ALPHABET_LENGTH; m++){
                        testPassword[4] = *(alphabet + m);
                        /* unsigned char testHash[32];
                        SHA256(testPassword, PASSWORD_SIZE, testHash);
                        int areHashesEqual = compareHashes(testHash, userHash);
                        if(areHashesEqual){
                            puts(testPassword);
                            return;
                        } */
                    }
                }
            }
        }
    }
}

void tryPassword(char* testPassword, unsigned char* userHash){}

int compareHashes(unsigned char* hash1, unsigned char* hash2){
    for(size_t i = 0; i < HASH_LENGTH; i++){
        if(*(hash1 + i) != *(hash2 + i))
            return 0;
    }
    return 1;
}
