#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "main.h"

const int NUM_COMPRADORES = 4;
const int NUM_ROBOTS = 4;
const int NUM_SECCIONES = 10;

const int MIN_PESO_SECCION = 1;
const int MAX_PESO_SECCION = 100;

const int MIN_COMPRA_SECCION = 1;
const int MAX_COMPRA_SECCION = 3;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t consume_t = PTHREAD_COND_INITIALIZER;
pthread_cond_t produce_t = PTHREAD_COND_INITIALIZER;

struct Seccion* arregloSecciones;
struct Robot* arregloRobots;

int main(int argc, const char * argv[]) {
    srand((unsigned) time(NULL));
    arregloSecciones = (struct Seccion*) malloc(sizeof(struct Seccion) * NUM_SECCIONES);
    asignarPesoMaxSecciones(arregloSecciones);

    arregloRobots = (struct Robot*) malloc(sizeof(struct Robot) * NUM_COMPRADORES);
    asignarPesoYListaRobots(arregloRobots);

    int nhilos = NUM_ROBOTS + NUM_ROBOTS;
    pthread_t * robots = (pthread_t *) malloc (sizeof(pthread_t) * nhilos);
    
    struct Robot* auxArregloRobots = arregloRobots;
    pthread_t * aux;



for(struct Seccion* aux = arregloSecciones; aux < (arregloSecciones + NUM_SECCIONES); ++aux)
        printf("Sección %d peso %d\n", aux->id, aux->pesoMax);

printf("\n\n");
    for(struct Robot* aux = arregloRobots; aux < (arregloRobots + NUM_COMPRADORES); ++aux){
                printf("Robot %d peso %d\n", aux->id,aux->pesoActual);
        for( int* i = aux->lista; i < (aux->lista + NUM_SECCIONES); i++){
            printf("%d  ", *i);
        }
        printf("\n");
    } 


     /* Crear los productores */
    for (aux = robots; aux < (robots+NUM_ROBOTS); ++aux) {
        printf("(I) Creando el productor %d\n", auxArregloRobots->id);
        pthread_create(aux, NULL, compraSeccion, auxArregloRobots);
        auxArregloRobots++;
    }

     /* Crear los consumidores */
     auxArregloRobots = arregloRobots;
    for (; aux < (robots+nhilos); ++aux) {
        printf("(I) Creando el consumidor %d\n", auxArregloRobots->id);
        pthread_create(aux, NULL, salirSeccion, auxArregloRobots);
        auxArregloRobots++;
    }

        /* Adjuntar los hilos */
    for (aux = robots; aux < (robots+nhilos); ++aux) {
        pthread_join(*aux, NULL);
    }
    
    free(robots);

    return 0;
}

void asignarPesoMaxSecciones(struct Seccion* arregloSecciones){
    int id;
    for(struct Seccion* aux = arregloSecciones; aux < (arregloSecciones + NUM_SECCIONES); ++aux){
        aux->pesoMax = getRandomNumber(MIN_PESO_SECCION, MAX_PESO_SECCION); 
        aux->id = ++id;
    }
}

void asignarPesoYListaRobots(struct Robot* arregloRobots){
    int id;
    int minValueSeccion = getMinValueSeccion();
    for(struct Robot* aux = arregloRobots; aux < (arregloRobots + NUM_COMPRADORES); ++aux){
        aux->pesoActual = getRandomNumber(MIN_PESO_SECCION,minValueSeccion);
        aux->lista = (int*) malloc(sizeof(int)*NUM_SECCIONES);
        aux->id = ++id;
        for( int* i = aux->lista; i < (aux->lista + NUM_SECCIONES); i++){
            int temp = getRandomNumber(MIN_COMPRA_SECCION, MAX_COMPRA_SECCION);
            *i = temp;
        }
    }     
}

void * compraSeccion(void * args){
    struct Robot* tempRobot = (struct Robot*) args;
    
    while(arregloSecciones->pesoMax >= (arregloSecciones->pesoActual + tempRobot->pesoActual) ){
        printf("(P) Entra a sección robot %d\n", tempRobot->id);
        pthread_mutex_lock(&mutex);
        if(arregloSecciones->pesoMax >= (arregloSecciones->pesoActual + tempRobot->pesoActual)&& *tempRobot->lista > 0){
            arregloSecciones->pesoActual += tempRobot->pesoActual;
            printf("+++ Inicia compra (Robot: %d  Peso actual: %d Lista: %d) entró a (Sección: %d Peso actual: %d Peso máximo: %d) \n",
            tempRobot->id,tempRobot->pesoActual,*(tempRobot->lista), arregloSecciones->id,arregloSecciones->pesoActual, arregloSecciones->pesoMax);
            int final = *tempRobot->lista;
            for(int aux = 0; aux < final; aux++){
                if((arregloSecciones->pesoMax - arregloSecciones->pesoActual) == 0){
                    printf("+++++ Robot: %d no es posible comprar producto, peso actual de sección %d está  en su capacidad máxima\n", 
                    tempRobot->id, arregloSecciones->id);
                    *tempRobot->lista -= 1;
                    continue;
                }   
                else{
                    int pesoProductoCompra = getRandomNumber(1, arregloSecciones->pesoMax - arregloSecciones->pesoActual);
                    printf("+++++ Robot: %d  Peso producto: %d\n",tempRobot->id, pesoProductoCompra);
                    tempRobot->pesoActual += pesoProductoCompra;
                    *tempRobot->lista -= 1;
                    arregloSecciones->pesoActual += pesoProductoCompra;
                } 
            }
            printf("+++ Termina compra(Robot: %d  Peso actual: %d Lista: %d  ) compró en (Sección: %d Peso actual: %d Peso máximo: %d) \n",
            tempRobot->id,tempRobot->pesoActual, *(tempRobot->lista), arregloSecciones->id,arregloSecciones->pesoActual, arregloSecciones->pesoMax);

            if(arregloSecciones->pesoActual>0)
                pthread_cond_broadcast(&consume_t);
        } else {
            printf("-------------Robot %d con Peso actual: %d se durmió (Sección:%d Peso actual:%d  Peso max:%d  )--------------\n",
             tempRobot->id, tempRobot->pesoActual, arregloSecciones->id, arregloSecciones->pesoActual, arregloSecciones->pesoMax);
            pthread_cond_wait(&produce_t, &mutex);
            printf("-------------Robot %d se despertó--------------\n", tempRobot->id);
        }
        pthread_mutex_unlock(&mutex);
    }
pthread_exit(NULL);
}

void * salirSeccion(void * args){
    struct Robot* tempRobot = (struct Robot*) args;
    while(arregloSecciones->pesoActual > 0 ){
        pthread_mutex_lock(&mutex);
        if(arregloSecciones->pesoActual > 0 && *tempRobot->lista == 0){
            arregloSecciones->pesoActual -= tempRobot->pesoActual;
            printf("(F)--- (Robot %d) se sale de (Sección: %d  Peso actual: %d)\n",
             tempRobot->id, arregloSecciones->id, arregloSecciones->pesoActual);
            *tempRobot->lista -= 1;
            if(arregloSecciones->pesoActual==0)
               pthread_cond_broadcast(&produce_t);

                

        }else{
                        /* La sección está vacía, se va a dormir */
            printf("-------------- Consumidor Robot %d se durmió ------------\n", tempRobot->id);
            pthread_cond_wait(&consume_t, &mutex);
            printf("-------------- Consumidor  Robot %d se despertó ------------\n", tempRobot->id);
        }  
        pthread_mutex_unlock(&mutex); 
    }
    pthread_exit(NULL);
}

int getRandomNumber(int min, int max){
    return (rand() % (max - min + 1)) + min;
}

int getMinValueSeccion(){
    int minValue;
    for(struct Seccion* aux = arregloSecciones; aux < (arregloSecciones + NUM_SECCIONES); aux++)
        minValue = (minValue < aux->pesoMax) ? minValue : aux->pesoMax;
    return minValue;
}