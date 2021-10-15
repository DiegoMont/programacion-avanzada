#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "mainAlt.h"
#include "Seccion.c"

/*
Crear secciones
Crear listas
Crear hilos para cada lista, el hilo representa el robot
Para cada sección a la que un robot quiere entrar:
  Bloquea el peso actual de la sección
  Si puede entrar a la sección
    Entra y actualiza el peso actual
    libera el peso de la sección en la que está
    Manda un broadcast a los otros hilos
    Desbloque el mutex
    Espera a que le entreguen los productos
  Si no puede entrar
    se pone en wait hasta que pueda entrar a la seccion
*/

const size_t MAX_NUMERO_SECCIONES = 5;
const size_t MAX_NUMERO_ROBOTS = 6;
const int MAX_PRODUCTOS_POR_SECCION = 5;
const useconds_t MAX_TIEMPO_ESPERA = 3000000;
const useconds_t MIN_TIEMPO_ESPERA = 500000;

struct Seccion* secciones;
size_t numSecciones;

int main(){
    srand(time(NULL));
    crearSecciones();
    const size_t NUMERO_ROBOTS = getRandomNumber(1, MAX_NUMERO_ROBOTS);
    pthread_t robots[NUMERO_ROBOTS];
    pthread_t* robotsEnd = robots + NUMERO_ROBOTS;
    for (pthread_t* robot = robots; robot < robotsEnd; robot++)
        pthread_create(robot, NULL, (void *) activarRobot, NULL);
    for (pthread_t* robot = robots; robot < robotsEnd; robot++)
        pthread_join(*robot, NULL);
    // Cleaning up
    free(secciones);
}

void crearSecciones(){
    numSecciones = getRandomNumber(1, MAX_NUMERO_SECCIONES);
    size_t sizeSecciones = sizeof(struct Seccion) * numSecciones;
    secciones = (struct Seccion*) malloc(sizeSecciones);
    struct Seccion* end = secciones + numSecciones;
    for(struct Seccion* seccion = secciones; seccion < end; seccion++)
        iniciarSeccion(seccion);
}

int getRandomNumber(int min, int max){
    return (rand() % (max - min + 1)) + min;
}

void* activarRobot(void* args){
    int lista[numSecciones];
    getLista(lista);
    int productosAComprar = 0;
    for(size_t i = 0; i < numSecciones; i++)
        productosAComprar += lista[i];
    printf("Tengo que comprar %d productos\n", productosAComprar);
    pthread_exit(NULL);
}

void iniciarSeccion(void* aux){
    struct Seccion* seccion = (struct Seccion*) aux;
    seccion->pesoActual = 0;
    seccion->pesoMaximo = getRandomNumber(2 * PESO_MAXIMO_PRODUCTO, PESO_MAXIMO_PRODUCTO * MAX_PRODUCTOS_POR_SECCION);
    seccion->tiempoQueTardaEnDespachar = getRandomNumber(MIN_TIEMPO_ESPERA, MAX_TIEMPO_ESPERA);
}

void getLista(int* lista){
    int* listaEnd = lista + numSecciones;
    for(int* seccion = lista; seccion < listaEnd; seccion++){
        int productosAComprar = getRandomNumber(0, MAX_PRODUCTOS_POR_SECCION);
        *seccion = productosAComprar;
    }
}
