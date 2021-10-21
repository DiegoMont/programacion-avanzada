#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "main.h"
#include "EstadoCajero.c"

/*
Hay clientesGeneralesFormados, clientesEmpresarialesFormados
En un arreglo guardamos el estado de los cajeros, los primeros tres cajeros son empresariales el resto de las operaciones.
El estado de los cajeros es disponible u ocupado
Tenemos otro semáforo que indica cuántos cajerosDisponibles tenemos
Cuando un cajero está listo para atender incrementa cajerosDisponibles:
  Los clientes checan cuál cajero está disponible:
    Si es empresarial y son empresariales se van a ese cajero y lo marcan como ocupado
    Si es empresarial y son generales lo toman únicamente si no hay clientes empresariales esperando
    Si es general y son generales lo toman
    Si es general y son empresariales GG
*/

const int NUM_CLIENTES_EMPRESARIALES = 50;
const int NUM_CLIENTES_GENERALES =100;
const int NUM_CAJEROS = 8;
const useconds_t DURACION_DESCANSO = 3000000;
const useconds_t MIN_TIEMPO_ATENCION = 3000000;
const useconds_t MAX_TIEMPO_ATENCION = 5000000;
const useconds_t MIN_TIEMPO_LLEGADA_EMPRESARIALES = 9000000;
const useconds_t MAX_TIEMPO_LLEGADA_EMPRESARIALES = 34000000;
const useconds_t MIN_TIEMPO_LLEGADA_GENERALES = 5000000;
const useconds_t MAX_TIEMPO_LLEGADA_GENERALES = 22000000;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t clientesGeneralesFormados;
sem_t clientesEmpresarialesFormados;
sem_t cajeros_disponibles;
enum EstadoCajero* cajeros;

int main(){
    srand(time(NULL));
    enum EstadoCajero aux[NUM_CAJEROS];
    cajeros = aux;
    iniciarSemaforos();
    cleanSemaforos();
}

void iniciarSemaforos(){
    sem_init(&clientesEmpresarialesFormados, 0, 0);
    sem_init(&clientesGeneralesFormados, 0, 0);
    sem_init(&cajeros_disponibles, 0, NUM_CAJEROS);
}

void* realizarOperacionEmpresarial(void* args){
    pthread_exit(NULL);
}

void* realizarOperacionGeneral(void* args){
    pthread_exit(NULL);
}

void* serUnCajero(void* args){
    pthread_exit(NULL);
}

void cleanSemaforos(){
    sem_destroy(&clientesEmpresarialesFormados);
    sem_destroy(&clientesGeneralesFormados);
    sem_destroy(&cajeros_disponibles);
}

useconds_t getRandomTimePeriod(useconds_t min, useconds_t max){
    return (rand() % (max - min + 1)) + min;
}
