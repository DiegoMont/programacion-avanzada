#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int PASSWORD_SIZE = 6;
char const alphabet[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};

const size_t ALPHABET_LENGTH = 26;

void bruteForcePassword(unsigned char* userHash);

const size_t HASH_LENGTH = 32;

char* getPassword(int passwordSize){
    //printf("Ingrese una contraseña de %d caracteres que contenga solo letras minúsculas: ", passwordSize);
    char* password = (char*) malloc(passwordSize * (sizeof(char) + 1));
    strcpy(password, "zzzzba");
    //scanf("%s", password);
    return password;
}

int elevateToPow(int base, int exp){
    if(exp == 0)
        return 1;
    int result = base;
    for (size_t i = 1; i < exp; i++)
        result *= base;
    return result;
}

void combinationToPassword(char* pass, size_t combination){
    for (size_t i = 0; i < PASSWORD_SIZE; i++){
        int exp = PASSWORD_SIZE - i - 1;
        int divisor = elevateToPow(ALPHABET_LENGTH, exp);
        char correspondingLetter = *(alphabet + combination / divisor);
        combination %= divisor;
        *(pass + i) = correspondingLetter;
    }
    *(pass + PASSWORD_SIZE) = 0;
}

int compareHashes(unsigned char* hash1, unsigned char* hash2){
    for(size_t i = 0; i < HASH_LENGTH; i++){
        if(*(hash1 + i) != *(hash2 + i))
            return 0;
    }
    return 1;
}

void testPassword(unsigned char* userHash, int combination){
    char testPass[PASSWORD_SIZE + 1];
    combinationToPassword(testPass, combination);
    unsigned char testHash[HASH_LENGTH];
    SHA256(testPass, PASSWORD_SIZE, testHash);
    if(compareHashes(userHash, testHash) == 1)
        printf("Contraseña encontrada: %s\n", testPass);
}
