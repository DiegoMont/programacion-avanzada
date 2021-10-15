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
const int MAX_PRODUCTOS_POR_SECCION = 3;
const useconds_t MAX_TIEMPO_ESPERA = 3000000;
const useconds_t MIN_TIEMPO_ESPERA = 500000;

struct Seccion* secciones;
size_t numSecciones;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t variableCondicion = PTHREAD_COND_INITIALIZER;

int main(){
    srand(time(NULL));
    crearSecciones();
    const size_t NUMERO_ROBOTS = 1;//getRandomNumber(1, MAX_NUMERO_ROBOTS);
    pthread_t robots[NUMERO_ROBOTS];
    printf("Tenemos un total de %lu robots\n", NUMERO_ROBOTS);
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
    int i = 1;
    for(struct Seccion* seccion = secciones; seccion < end; seccion++){
        iniciarSeccion(seccion);
        printf("Sección %d tiene un peso máximo de %d y sus productos pesan %d\n", i, seccion->pesoMaximo, seccion->pesoDeProducto);
        i++;
    }
}

int getRandomNumber(int min, int max){
    return (rand() % (max - min + 1)) + min;
}

void* activarRobot(void* args){
    int lista[numSecciones];
    int pesoRobot = 0;
    getLista(lista);
    printProductosAComprar(lista);
    for(int seccionId = 0; seccionId < numSecciones; seccionId++){
        int entroALaSeccion = 0;
        while(!entroALaSeccion){
            pthread_mutex_lock(&mutex);
            struct Seccion* seccion = secciones + seccionId;
            int puedeEntrar = seccion->pesoActual + pesoRobot <= seccion->pesoMaximo;
            if(puedeEntrar){
                entroALaSeccion = 1;
                liberarSeccionAnterior(seccionId, pesoRobot);
                seccion->pesoActual += pesoRobot;
                int productosAComprar = getProductosAComprar(seccionId, pesoRobot, lista[seccionId]);
                apartarEspacioNecesario(seccion, productosAComprar);
                printf("Entrando a seccion %d. Peso actual de: %d\n", seccionId + 1, seccion->pesoActual);
                pthread_cond_broadcast(&variableCondicion);
                pthread_mutex_unlock(&mutex);
                esperarYRecibirProductos(productosAComprar, &pesoRobot, seccion);
            } else {
                pthread_cond_wait(&variableCondicion, &mutex);
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    puts("Hemos terminado el shopping");
    pthread_exit(NULL);
}

void iniciarSeccion(void* aux){
    struct Seccion* seccion = (struct Seccion*) aux;
    seccion->pesoActual = 0;
    seccion->pesoMaximo = getRandomNumber(2 * PESO_MAXIMO_PRODUCTO, PESO_MAXIMO_PRODUCTO * MAX_PRODUCTOS_POR_SECCION);
    seccion->pesoDeProducto = getRandomNumber(1, PESO_MAXIMO_PRODUCTO);
    seccion->tiempoQueTardaEnDespachar = getRandomNumber(MIN_TIEMPO_ESPERA, MAX_TIEMPO_ESPERA);
}

void getLista(int* lista){
    int* listaEnd = lista + numSecciones;
    for(int* seccion = lista; seccion < listaEnd; seccion++){
        int productosAComprar = getRandomNumber(0, MAX_PRODUCTOS_POR_SECCION);
        *seccion = productosAComprar;
    }
}

void printProductosAComprar(int* lista){
    int productosAComprar = 0;
    for(size_t i = 0; i < numSecciones; i++)
        productosAComprar += lista[i];
    printf("Tengo que comprar %d productos\n", productosAComprar);
}

void liberarSeccionAnterior(size_t seccionALiberar, int pesoRobot){
    if(seccionALiberar == 0)
        return;
    seccionALiberar--;
    struct Seccion* seccion = secciones + seccionALiberar;
    seccion->pesoActual -= pesoRobot;
    printf("Un robot salió de la sección %lu su peso ahora es de %d\n", seccionALiberar+1, seccion->pesoActual);
}

int getProductosAComprar(size_t seccionIndx, int pesoRobot, int productosDeseados){
    struct Seccion* seccion = secciones + seccionIndx;
    int pesoMaximoDelRobot = seccion->pesoMaximo;
    struct Seccion* end = secciones + numSecciones;
    for(; seccion < end; seccion++)
        pesoMaximoDelRobot = min(pesoMaximoDelRobot, seccion->pesoMaximo);
    int productosPermitidos = 0;
    seccion = secciones + seccionIndx;
    while(seccion->pesoDeProducto + pesoRobot <= pesoMaximoDelRobot){
        productosPermitidos++;
        pesoRobot += seccion->pesoDeProducto;
    }
    return productosPermitidos;
}

void apartarEspacioNecesario(void* aux, int productos){
    struct Seccion* seccion = (struct Seccion*) aux;
    seccion->pesoActual += productos * seccion->pesoDeProducto;
}

void esperarYRecibirProductos(int totalProductos, int* pesoRobot, void* aux){
    struct Seccion* seccion = (struct Seccion*) aux;
    *pesoRobot += totalProductos * seccion->pesoDeProducto;
    useconds_t tiempoAEsperar = totalProductos * seccion->tiempoQueTardaEnDespachar;
    usleep(tiempoAEsperar);
}

int min(int a, int b){
    return a < b ? a: b;
}
