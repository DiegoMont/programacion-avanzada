#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "main.h"
#include "Cliente.c"
#include "TipoCliente.c"
#include "EstadoCajero.c"
#include "Cajero.c"
#include "Queue.c"

/*
Cada cliente es un hilo, cuando se crea va a esperar un tiempo aleatorio dependiendo del tipo de operacion a realizar
Cuando termine la espera, se agrega a la fila
Un semáforo clientesFormados indicará cuantos clientes quedan en la fila
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
const int TOTAL_CLIENTES = NUM_CLIENTES_EMPRESARIALES + NUM_CLIENTES_GENERALES;
const int NUM_CAJEROS = 8;
const useconds_t DURACION_DESCANSO = 3000000;
const useconds_t MIN_TIEMPO_ATENCION = 3000000;
const useconds_t MAX_TIEMPO_ATENCION = 5000000;
const useconds_t MIN_TIEMPO_LLEGADA_EMPRESARIALES = 9000000;
const useconds_t MAX_TIEMPO_LLEGADA_EMPRESARIALES = 34000000;
const useconds_t MIN_TIEMPO_LLEGADA_GENERALES = 5000000;
const useconds_t MAX_TIEMPO_LLEGADA_GENERALES = 22000000;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t clientesFormados;
struct Cajero* cajeros;
int clientesPorAtender = TOTAL_CLIENTES;

int main(){
    srand(time(NULL));
    cajeros = (struct Cajero*) malloc(sizeof(struct Cajero) * NUM_CAJEROS);
    pthread_t cajero_threads[NUM_CAJEROS];
    pthread_t clientes[TOTAL_CLIENTES];
    iniciarSemaforos();
    iniciarCajeros(cajero_threads);
    mandarClientesAlBanco(clientes);
    for (size_t i = 0; i < TOTAL_CLIENTES; i++)
        pthread_join(*(clientes + i), NULL);
    
    for(size_t i = 0; i < NUM_CAJEROS; i++)
        pthread_join(*(cajero_threads + i), NULL);
    // Cleaning up
    cleanSemaforos();
    free(cajeros);
}

void iniciarSemaforos(){
    sem_init(&clientesFormados, 0, 0);
    sem_init(&cajerosDisponibles, 0, NUM_CAJEROS);
}

void iniciarCajeros(pthread_t* cajero_threads){
    for(size_t i = 0; i < NUM_CAJEROS; i++){
        struct Cajero* cajero = cajeros + i;
        cajero->id = i + 1;
        cajero->clientesAtendidos = 0;
        setDisponible(cajero);
        pthread_create(cajero_threads + i, NULL, serUnCajero, (void*) cajero);
    }
}

void mandarClientesAlBanco(pthread_t* clientes){
    int empresarialesFormados = 0;
    int generalesFormados = 0;
    for (size_t i = 0; i < TOTAL_CLIENTES; i++){
        int randNum = getRandomTimePeriod(1, 10);
        int tipoCliente = randNum & 1;
        if(empresarialesFormados == NUM_CLIENTES_EMPRESARIALES)
            tipoCliente = CLIENTE_GENERAL;
        if(generalesFormados == NUM_CLIENTES_GENERALES)
            tipoCliente == CLIENTE_EMPRESARIAL;
        if(tipoCliente == CLIENTE_EMPRESARIAL){
            empresarialesFormados++;
            pthread_create(clientes + i, NULL, realizarOperacionEmpresarial, NULL);
        } else {
            generalesFormados++;
            pthread_create(clientes + i, NULL, realizarOperacionGeneral, NULL);
        }
    }
    
}

void* realizarOperacionEmpresarial(void* args){
    pthread_exit(NULL);
}

void* realizarOperacionGeneral(void* args){
    pthread_exit(NULL);
}

void* serUnCajero(void* args){
    struct Cajero* cajero = (struct Cajero*) args;
    printf("Yo soy el cajero %d\n", cajero->id);
    int cerrarElChangarro = 0;
    pthread_exit(NULL);
}

void cleanSemaforos(){
    sem_destroy(&clientesFormados);
    sem_destroy(&cajerosDisponibles);
}

useconds_t getRandomTimePeriod(useconds_t min, useconds_t max){
    return (rand() % (max - min + 1)) + min;
}

void setDisponible(void* aux){
    struct Cajero* cajero = (struct Cajero*) aux;
    cajero->estado = DISPONIBLE;
    sem_post(&cajerosDisponibles);
}
