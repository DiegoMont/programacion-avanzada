#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void manejador1(int senial){
    puts("Estoy aprendiendo a dar señales...");
}

void manejador2(int senial){
    puts("Ahora cambié de manejador");
}

void contar(const char* objetoAContar){
    for (int i = 1; i < 11; i++){
        sleep(1);
        printf("%s %d\n", objetoAContar, i);
    }
}

int main(){
    if(signal(SIGINT, manejador1) == SIG_ERR)
        puts("Error");
    contar("Oveja");
    if(signal(SIGINT, manejador2) == SIG_ERR)
        puts("Error");
    contar("Trineo");
    if(signal(SIGINT, manejador1) == SIG_ERR)
        puts("Error");
    contar("Cabrito");
}