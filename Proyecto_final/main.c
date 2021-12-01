//#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "Password.c"

const int PASSWORD_SIZE = 2;

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
    strcpy(password, "aa");
    //scanf("%s", password);
    return password;
}

void bruteForcePassword(unsigned char* userHash){
    for(size_t currentPasswordLength = 1; currentPasswordLength <= PASSWORD_SIZE; currentPasswordLength++){
        size_t possibleCombinations = pow(ALPHABET_LENGTH, currentPasswordLength);
        struct Password* possiblePassword = initTestPassword(currentPasswordLength);
        for(int combination = 0; combination < possibleCombinations; combination++){
            /* unsigned char testHash[32];
            SHA256(testPassword, PASSWORD_SIZE, testHash);
            int areHashesEqual = compareHashes(testHash, userHash);
            if(areHashesEqual){
                puts(testPassword);
                return;
            } */
            puts(possiblePassword->strVal);
            incrementPassword(possiblePassword);
        }
        destroyPassword(possiblePassword);
    }
}

int pow(int base, int exp){
    if(exp == 0)
        return 1;
    int result = base;
    for (size_t i = 1; i < exp; i++)
        result *= base;
    return result;
}

void tryPassword(char* testPassword, unsigned char* userHash){}

int compareHashes(unsigned char* hash1, unsigned char* hash2){
    const size_t HASH_LENGTH = 32;
    for(size_t i = 0; i < HASH_LENGTH; i++){
        if(*(hash1 + i) != *(hash2 + i))
            return 0;
    }
    return 1;
}
