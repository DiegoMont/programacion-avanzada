#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

const int LARGO_PISTA = 70;
const int DURACION_CICLO_SEGUNDOS = 1;

int posicionTortuga;
int posicionLiebre;

int randomNum(int max){
    srand(time(0));
    return rand() % max;
}

/* int movimientoTortuga(int num){
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
    
} */

void imprimirCarrera(){
    for(int i = 0; i < LARGO_PISTA; i++){
        int liebreSeEncuentraAqui = posicionLiebre == i;
        int tortugaSeEncuentraAqui = posicionTortuga == i;
        if(liebreSeEncuentraAqui && tortugaSeEncuentraAqui){
            printf("OUCH!!!!");
            break;
        } else if(liebreSeEncuentraAqui)
            printf("L");
        else if(tortugaSeEncuentraAqui)
            printf("T");
        else
            printf(" ");
    }
    printf("\n\n");
}

int terminoLaCarrera(){
    const int POSICION_META = LARGO_PISTA - 1;
    return posicionTortuga >= POSICION_META || posicionLiebre >= POSICION_META;
}

void imprimirGanador(){
    const int POSICION_META = LARGO_PISTA - 1;
    int liebreCruzoLaMeta = posicionLiebre >= POSICION_META;
    int tortugaCruzoLaMeta = posicionTortuga >= POSICION_META;
    if(liebreCruzoLaMeta && tortugaCruzoLaMeta)
      puts("Es un empate");
    else if(liebreCruzoLaMeta)
      puts("Ganó la Liebre .Yupi");
    else
      puts("GANO LA TORTUGA!!!!! VIVA !!!!!");
}

void calcularPosicionLiebre(int fileDescriptor){
    return;
}

void calcularPosicionTortuga(int fileDescriptor){
    int probabilidad = randomNum(10);
    int movimientoARealizar;
    if(probabilidad < 5)
        movimientoARealizar = 3;
    else if(probabilidad < 7)
        movimientoARealizar = -6;
    else
        movimientoARealizar = 1;
    int nuevaPosicion = posicionTortuga + movimientoARealizar;
    if(nuevaPosicion < 0)
        nuevaPosicion = 0;

    FILE * file;
    file = fdopen(fileDescriptor, "w");
    fprintf(file, "%d", nuevaPosicion);
    printf("Tortuga = %d\n", nuevaPosicion);
    fclose(file);
}

void leerPipesYActualizarPosiciones(int* pipeLiebre, int* pipeTortuga){
    close(*(pipeLiebre + 1));
    close(*(pipeTortuga + 1));

    FILE * file;
    int c;
    
    file = fdopen(*pipeLiebre, "r");
    while ( (c = fgetc(file)) != EOF )
        posicionLiebre = c;
    fclose(file);

    file = fdopen(*pipeTortuga, "r");
    while ( (c = fgetc(file)) != EOF )
        posicionTortuga = c;
    fclose(file);
}

int main(){
    //Preparación de la carrera
    time_t t;
    srand((unsigned) time(&t));
    int pipeTortuga[2], pipeLiebre[2];
    pid_t pidLiebre, pidTortuga;
    pipe(pipeTortuga);
    pipe(pipeLiebre);

    //Comienza la carrera
    posicionTortuga = 10;    
    posicionLiebre = 10;
    puts("BANG!!!!!\nY ARRANCAN!!!!!");
    clock_t ultimaActualizacionPosiciones = clock();
    while(!terminoLaCarrera()){
        clock_t momentoActual = clock();
        clock_t segundosTranscurridos = (momentoActual - ultimaActualizacionPosiciones) / CLOCKS_PER_SEC;
        if( segundosTranscurridos >= DURACION_CICLO_SEGUNDOS){
            pidLiebre = fork();
            if (pidLiebre == -1)
                ;
            else if(pidLiebre == 0){
                close(*(pipeLiebre + 0));
                calcularPosicionLiebre(*(pipeLiebre + 1));
                exit(0);
            } else {
                pidTortuga = fork();
                if(pidTortuga == -1)
                    ;
                else if(pidTortuga == 0) {
                    close(*(pipeTortuga + 0));
                    calcularPosicionTortuga(*(pipeTortuga + 1));
                    exit(0);
                } else {
                    //leerPipesYActualizarPosiciones(pipeLiebre, pipeTortuga);
                }
            }
            imprimirCarrera();
            ultimaActualizacionPosiciones = momentoActual;
        }
    }
    imprimirGanador();
}

