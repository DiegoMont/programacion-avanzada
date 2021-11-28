#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

#include "main.h"

const int PASSWORD_SIZE = 5;

int main(){
    char* userPassword = getPassword(PASSWORD_SIZE);
    puts(userPassword);
    unsigned char* userHash = SHA256(userPassword, PASSWORD_SIZE, 0);
    // Cleaning up
    free(userPassword);
}

char* getPassword(int passwordSize){
    printf("Ingrese una contraseña de %d caracteres que contenga solo letras minúsculas: ", passwordSize);
    char* password = (char*) malloc(passwordSize * sizeof(char));
    scanf("%s", password);
    return password;
}


