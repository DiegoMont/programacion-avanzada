#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "semaforo.h"
#include "EstadoSemaforo.c"

const char* SERVER_IP_ADDRESS = "127.0.0.1";
const int TCP_PORT = 8000;

const int NUMBER_OF_TRAFFIC_LIGHTS = 4;
const unsigned int TRAFFIC_LIGHT_DURATION = 2;

struct Semaforo* trafficLights;
size_t trafficLightID;

int main(){
    trafficLights = createSharedMemory(NUMBER_OF_TRAFFIC_LIGHTS);
    signal(SIGALRM, alarmHandler);
    signal(SIGUSR1, SIGUSR1Handler);
    for(trafficLightID = 1; trafficLightID <= NUMBER_OF_TRAFFIC_LIGHTS; trafficLightID++){
        pid_t pid;
        pid = fork();
        if(pid == -1)
            logErrorAndExit("Couldn't create process");
        else if(pid == 0){
            sleep(1);
            beATrafficLight();
            break;
        } else {
            struct Semaforo* trafficLight = getTrafficLight(trafficLightID);
            trafficLight->estadoAnterior = VERDE;
            trafficLight->estado = ROJO;
            trafficLight->pid = pid;
        }
    }
    kill(getTrafficLight(1)->pid, SIGUSR1);
    while(1)
        sleep(1);
}

struct Semaforo* createSharedMemory(size_t numberOfElements) {
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
    const size_t size = numberOfElements * sizeof(struct Semaforo);
    void* mappedMemory = mmap(NULL, size, protection, visibility, 0, 0);
    if(mappedMemory == MAP_FAILED){
        printf("Mapping Failed\n");
        exit(1);
    }
    return (struct Semaforo*) mappedMemory;
}

void alarmHandler(int s){
    pid_t nextTrafficLightPID = getNextTrafficLightPID();
    kill(nextTrafficLightPID, SIGUSR1);
}

void SIGUSR1Handler(int s){
    setTrafficLightsToRojo();
    struct Semaforo* trafficLight = getTrafficLight(trafficLightID);
    trafficLight->estado = VERDE;
    printf("El semáforo %lu está en %s\n",
           trafficLightID,
           estadoToString(trafficLight->estado));
    alarm(TRAFFIC_LIGHT_DURATION);
}

void logErrorAndExit(const char* errorMsg){
    puts(errorMsg);
    puts("Terminating program");
    exit(1);
}

void beATrafficLight(){
    printf("Semáforo %lu iniciado con vecino PID: %i\n",
           trafficLightID,
           getNextTrafficLightPID());
    while(1)
        sleep(1);
}

struct Semaforo* getTrafficLight(size_t id){
    const size_t index = id - 1;
    struct Semaforo* trafficLight = trafficLights + index;
    return trafficLight;
}

pid_t getNextTrafficLightPID(){
    size_t nextTrafficLightID = trafficLightID == NUMBER_OF_TRAFFIC_LIGHTS ? 1: trafficLightID + 1;
    pid_t nextPID = getTrafficLight(nextTrafficLightID)->pid;
    return nextPID;
}

void setTrafficLightsToRojo(){
    for(int i = 0 ; i < NUMBER_OF_TRAFFIC_LIGHTS; i++){
        struct Semaforo* aux = trafficLights + i;
        aux->estadoAnterior = aux->estado;
        aux->estado = ROJO;
    }
}
