#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int randomNum(int min, int max){
    return (rand() % (max - min + 1)) + min;
}

int movimientoTortuga(int num){
    switch (num)
    {
        case 1 ... 5:
            return 3;
        case 6 ... 7:
            return -6;
        default:
            return 1;
    }   
}

int movimientoLiebre(int num){
    switch (num)
    {
        case 1 ... 2:
            return 0;
        case 3 ... 4:
            return 9;
        case 5:
            return -12;
        case 6 ... 8:
            return 1;
        default:
            return -2;
    }   
}


void posicionTortuga(int **fd){
    int pos_tortuga = 0;
    while(pos_tortuga < 70){
        close(fd[0][0]);
        close(fd[1][0]);
        close(fd[1][1]);
        int temp = randomNum(1,10);
        pos_tortuga += movimientoTortuga(temp);
        //printf("random:%d  pos:%d\n", temp, pos_tortuga);
        write(fd[0][1], &pos_tortuga, sizeof(int));
        sleep(1);
        
    }
}

void posicionLiebre(int **fd){
    int pos_liebre = 0;
    while(pos_liebre < 70){
        close(fd[0][0]);
        close(fd[0][1]);
        close(fd[1][0]);
        int temp = randomNum(1,10);
        pos_liebre += movimientoLiebre(temp);
        //printf("random:%d  pos:%d\n", temp, pos_tortuga);
        write(fd[1][1], &pos_liebre, sizeof(int));
        sleep(1);
        
    }
}

void printerTablero(char *tablero, int casillas, int pos_tortuga, char t){
    char *final = tablero + casillas;
    char *tortuga = tablero + pos_tortuga;
    char*aux = tablero;
    if(pos_tortuga<0){
         printf(" %c ", t);
         aux++;
         for(; aux < final; aux++)
              printf("_ ");      
    }else{
        for(; aux < final; aux++){
        if(aux == tortuga)
            printf(" %c ", t);
        else
            printf("_ ");   
        }
    }

    printf("\n");
    printf("\n");
    
}

void imprimirJuego(int **fd, char *tablero, int casillas, char t){
    int pos_tortuga = 0;
    while(pos_tortuga < 70){
        close(fd[0][1]);
        close(fd[1][0]);
        close(fd[1][1]);
        read(fd[0][0], &pos_tortuga, sizeof(int));
        //printf("---  Recibí %d  \n", pos_tortuga);  
        printerTablero(tablero, casillas, pos_tortuga - 1, t);
    }
}

int main(){
    pid_t pid1, pid2;
    int filas = 2, columnas = 2;
    int** tubo = (int**) malloc(filas * sizeof(int*));
    int** final = tubo + filas;
    
    for(int** aux = tubo; aux < final; aux++){
        *aux = (int*)malloc(columnas * sizeof(int));
        if(pipe(*aux) < 0)
            return 1;
    }

    pid1 = fork();
    srand(time(0));

    if( pid1 == 0){
        printf("Estoy en el proceso hijo\n");
        posicionTortuga(tubo);

    }
    else if (pid1 == -1)
    {
        /* Error al crear el proceso hijo */
        exit(-1);
    }
    else {
        printf("Estoy en el proceso padre\n");
        int casillas = 70;
        char *tablero = malloc(casillas * sizeof(char));
        //printerTablero(tablero, casillas, -1, 'T');
        imprimirJuego(tubo, tablero, casillas, 'T');
    }


    return 0;
}

