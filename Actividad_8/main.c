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
Hay dos filas clientesEmpresariales, clientesGenerales.
Hay un semáforo clientesEsperando
Cada cliente es un hilo, cuando se crea va a esperar un tiempo aleatorio dependiendo del tipo de operacion a realizar
Cuando termine la espera, se agrega a su fila correspondiente e incrementa el semáforo correspondiente

Para los cajeros:
Mientras haya clientes por atender esperarán a que sus semáforos se incrementen
Atienden al cliente correspondiente
*/

const int NUM_CLIENTES_EMPRESARIALES = 50;
const int NUM_CLIENTES_GENERALES =100;
const int TOTAL_CLIENTES = NUM_CLIENTES_EMPRESARIALES + NUM_CLIENTES_GENERALES;
const int CAJEROS_EMPRESARIALES = 3;
const int CAJEROS_GENERALES = 5;
const int NUM_CAJEROS = CAJEROS_EMPRESARIALES + CAJEROS_GENERALES;
const useconds_t DURACION_DESCANSO = 3000000;
const useconds_t MIN_TIEMPO_ATENCION = 3000000;
const useconds_t MAX_TIEMPO_ATENCION = 5000000;
const useconds_t MIN_TIEMPO_LLEGADA_EMPRESARIALES = 9000000;
const useconds_t MAX_TIEMPO_LLEGADA_EMPRESARIALES = 34000000;
const useconds_t MIN_TIEMPO_LLEGADA_GENERALES = 5000000;
const useconds_t MAX_TIEMPO_LLEGADA_GENERALES = 22000000;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t clientesFormados;
struct Queue* clientesEmpresariales;
struct Queue* clientesGenerales;
struct Cajero* cajeros;
int clientesPorAtender = TOTAL_CLIENTES;

int main(){
    srand(time(NULL));
    cajeros = (struct Cajero*) malloc(sizeof(struct Cajero) * NUM_CAJEROS);
    pthread_t cajero_threads[NUM_CAJEROS];
    pthread_t clientes[TOTAL_CLIENTES];
    clientesEmpresariales = queue();
    clientesGenerales = queue();
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
}

void iniciarCajeros(pthread_t* cajero_threads){
    for(size_t i = 0; i < NUM_CAJEROS; i++){
        struct Cajero* cajero = cajeros + i;
        cajero->id = i + 1;
        cajero->clientesAtendidos = 0;
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

int idClientes = 1;
void* realizarOperacionEmpresarial(void* args){
    struct Cliente* cliente = (struct Cliente*) malloc(sizeof(struct Cliente));
    cliente->tipoCliente = CLIENTE_EMPRESARIAL;
    useconds_t timeToWait = getRandomTimePeriod(MIN_TIEMPO_LLEGADA_EMPRESARIALES, MAX_TIEMPO_LLEGADA_EMPRESARIALES);
    usleep(timeToWait);
    pthread_mutex_lock(&mutex);
    cliente->id = idClientes++;
    enqueue(clientesEmpresariales, cliente);
    sem_post(&clientesFormados);
    printf("Se ha formado el cliente %d en Empresariales\n", cliente->id);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* realizarOperacionGeneral(void* args){
    struct Cliente* cliente = (struct Cliente*) malloc(sizeof(struct Cliente));
    cliente->tipoCliente = CLIENTE_GENERAL;
    useconds_t timeToWait = getRandomTimePeriod(MIN_TIEMPO_LLEGADA_GENERALES, MAX_TIEMPO_LLEGADA_GENERALES);
    usleep(timeToWait);
    pthread_mutex_lock(&mutex);
    cliente->id = idClientes++;
    enqueue(clientesGenerales, cliente);
    sem_post(&clientesFormados);
    printf("Se ha formado el cliente %d en Generales\n", cliente->id);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void* serUnCajero(void* args){
    struct Cajero* cajero = (struct Cajero*) args;
    printf("Yo soy el cajero %d\n", cajero->id);
    while(faltanClientesPorAtender()){
        sem_wait(&clientesFormados);
        struct Cliente* clienteAAtender = NULL;
        pthread_mutex_lock(&mutex);
        if(cajero->id <= CAJEROS_EMPRESARIALES)
            clienteAAtender = dequeue(clientesEmpresariales);
        if(clienteAAtender == NULL)
            clienteAAtender = dequeue(clientesGenerales);
        clientesPorAtender--;
        pthread_mutex_unlock(&mutex);
        logOperacion(cajero, clienteAAtender);
        useconds_t duracionAtencion = getRandomTimePeriod(MIN_TIEMPO_ATENCION, MAX_TIEMPO_ATENCION);
        usleep(duracionAtencion);
        cajero->clientesAtendidos++;
        if(cajero->clientesAtendidos == 3){
            printf("El cajero %d ya atendió mucho cliente, se va a tomar un descanso\n", cajero->id);
            usleep(DURACION_DESCANSO);
            cajero->clientesAtendidos = 0;
        }
    }
    pthread_exit(NULL);
}

void cleanSemaforos(){
    sem_destroy(&clientesFormados);
}

useconds_t getRandomTimePeriod(useconds_t min, useconds_t max){
    return (rand() % (max - min + 1)) + min;
}

int faltanClientesPorAtender(){
    int faltanClientes;
    pthread_mutex_lock(&mutex);
    faltanClientes = 0 < clientesPorAtender;
    pthread_mutex_unlock(&mutex);
    return faltanClientes;
}

void logOperacion(void* cajero, void* cliente){
    struct Cajero* aux1 = (struct Cajero*) cajero;
    struct Cliente* aux2 = (struct Cliente*) cliente;
    if(aux2->tipoCliente == CLIENTE_EMPRESARIAL)
        printf("El cajero %d está atendiendo al cliente %d con una operación empresarial\n", aux1->id, aux2->id);
    else
        printf("El cajero %d está atendiendo al cliente %d con una operación general\n", aux1->id, aux2->id);
}
