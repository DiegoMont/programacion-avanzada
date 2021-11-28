#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

void* irAlEste(void* args);
void* irAlOeste(void* args);

const unsigned int TIEMPO_QUE_TARDAN_EN_CRUZAR = 1;
sem_t ocupadoDeEsteAOeste, ocupadoDeOesteAEste;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
    srand(time(NULL));
    size_t babuinosDelEste = rand();
    size_t babuinosDelOeste = rand();
    size_t totalBabuinos = babuinosDelEste + babuinosDelOeste;
    pthread_t babuinos[totalBabuinos];
    pthread_t* fin = babuinos + babuinosDelEste;
    pthread_t* aux;
    sem_init(&ocupadoDeEsteAOeste, 0, 1);
    sem_init(&ocupadoDeOesteAEste, 0, 1);
    for(aux = babuinos; aux < fin; aux++)
        pthread_create(aux, NULL, (void*)irAlOeste, NULL);
    fin = babuinos + totalBabuinos;
    for(; aux < fin; aux++)
        pthread_create(aux, NULL, (void*)irAlEste, NULL);
    for(aux = babuinos; aux < fin; aux++)
      pthread_join(*aux, NULL);
}

void* irAlEste(void* args){
    int valor;
    pthread_mutex_lock(&mutex);
    sem_getvalue(&ocupadoDeOesteAEste, &valor);
    if(valor == 1){
        puts("Babuino viajando al Este");
        
    }
}

void* irAlOeste(void* args){
    
}
