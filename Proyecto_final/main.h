#include <openssl/sha.h>
#include <stdio.h>
#include <string.h>

#include "Password.c"
#include "settings.c"

void bruteForcePassword(unsigned char* userHash);

const size_t HASH_LENGTH = 32;

char* getPassword(int passwordSize){
    printf("Ingrese una contraseña de %d caracteres que contenga solo letras minúsculas: ", passwordSize);
    char* password = (char*) malloc(passwordSize * sizeof(char));
    strcpy(password, "xxjzd");
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

int compareHashes(unsigned char* hash1, unsigned char* hash2){
    for(size_t i = 0; i < HASH_LENGTH; i++){
        if(*(hash1 + i) != *(hash2 + i))
            return 0;
    }
    return 1;
}
