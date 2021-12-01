#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char const alphabet[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
};


void bruteForcePrueba(unsigned char* userHash){
    int lengthPassword = 3;
    char testPassword[lengthPassword];
    int numLetters = 3;
    int total = numLetters * numLetters * numLetters ;
    
    for(int i = 0; i < total; i++){
        int tempLengthPassword = lengthPassword;
        int tempNumLetters = numLetters;     
        while (tempLengthPassword > 0){
            // no que no está bien es esto alphabet[i % tempNumLetters], no he encontrado la fórmula del patrón
            testPassword[tempLengthPassword - 1] = alphabet[i % tempNumLetters];
            tempLengthPassword--; 
            tempNumLetters--;     
            
        } 

         printf("%c%c%c\n",testPassword[0], testPassword[1], testPassword[2]);
        

    }
            
}



int main(){

    char* contrasenia = "t";
    bruteForcePrueba(contrasenia);

}


   