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
    int probabilidad = randomNum(10);
    int movimientoARealizar;
    if(probabilidad < 2)
        movimientoARealizar = 0;
    else if(probabilidad < 4)
        movimientoARealizar = 9;
    else if(probabilidad < 5)
        movimientoARealizar = -12;
    else if(probabilidad < 8)
        movimientoARealizar = 1;
    else
        movimientoARealizar = -2;
    int nuevaPosicion = posicionLiebre + movimientoARealizar;
    if(nuevaPosicion < 0)
        nuevaPosicion = 0;

    FILE * file;
    file = fdopen(fileDescriptor, "w");
    fprintf(file, "%d", nuevaPosicion);
    printf("Liebre = %d\n", nuevaPosicion);
    fclose(file);
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
    int posicionAsChar;
    int posicionAsInt;
    const int VALOR_ASCII_DE_CERO = 48;

    file = fdopen(*pipeLiebre, "r");
    while ( (posicionAsChar = fgetc(file)) != EOF )
        posicionAsInt = posicionAsChar - VALOR_ASCII_DE_CERO;
    posicionLiebre = posicionAsInt;
    fclose(file);

    file = fdopen(*pipeTortuga, "r");
    while ( (posicionAsChar = fgetc(file)) != EOF )
        posicionAsInt = posicionAsChar - VALOR_ASCII_DE_CERO;
    posicionTortuga = posicionAsInt;
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
    posicionTortuga = 0;    
    posicionLiebre = 0;
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
                close(*(pipeLiebre));
                calcularPosicionLiebre(*(pipeLiebre + 1));
                exit(0);
            } else {
                pidTortuga = fork();
                if(pidTortuga == -1)
                    ;
                else if(pidTortuga == 0) {
                    close(*(pipeTortuga));
                    calcularPosicionTortuga(*(pipeTortuga + 1));
                    exit(0);
                } else {
                    leerPipesYActualizarPosiciones(pipeLiebre, pipeTortuga);
                }
            }
            imprimirCarrera();
            ultimaActualizacionPosiciones = momentoActual;
        }
    }
    imprimirGanador();
}

