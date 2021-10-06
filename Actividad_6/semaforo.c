#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <unistd.h>

#include "semaforo.h"
#include "EstadoSemaforo.c"

const char* SERVER_IP_ADDRESS = "127.0.0.1";
const int TCP_PORT = 8000;

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
            trafficLight->serverFileDescriptor = -1;
        }
    }
    sleep(2);
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
    sendStatusToConsole();
    alarm(TRAFFIC_LIGHT_DURATION);
}

void logErrorAndExit(const char* errorMsg){
    puts(errorMsg);
    puts("Terminating program");
    exit(1);
}

void beATrafficLight(){
    struct Semaforo* thisTrafficLight = getTrafficLight(trafficLightID);
    printf("Semáforo %lu iniciado con vecino PID: %i\n",
           trafficLightID,
           getNextTrafficLightPID());
    int serverFileDescriptor = connectToConsole();
    thisTrafficLight->serverFileDescriptor = serverFileDescriptor;
    while(1){
        char buffer[14];
        int readedBytes = read(thisTrafficLight->serverFileDescriptor, buffer, sizeof buffer);
        if(readedBytes > 0){
            toggleSpecialState(thisTrafficLight, ROJO_ESTATICO);
        }
        sleep(1);
    }
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

int connectToConsole(){
    // Create socket
    int socketServerFileDescriptor;
    socketServerFileDescriptor = socket(PF_INET, SOCK_STREAM, 0);
    if(socketServerFileDescriptor == -1)
        logErrorAndExit("Couldn't create socket");

    // Connect to server
    struct sockaddr_in socketAddressInfo;
    socketAddressInfo.sin_port = htons(TCP_PORT);
    socketAddressInfo.sin_family = AF_INET;
    inet_aton(SERVER_IP_ADDRESS, &socketAddressInfo.sin_addr);
    socklen_t addressInfoSize = sizeof(socketAddressInfo);
    int connectionError = connect(socketServerFileDescriptor, (struct sockaddr *) &socketAddressInfo, addressInfoSize);
    if(connectionError == -1){
        close(socketServerFileDescriptor);
        logErrorAndExit("Couldn't connect to server");
    }
    printf("Conectado a %s:%d \n",
           inet_ntoa(socketAddressInfo.sin_addr),
           ntohs(socketAddressInfo.sin_port));
    return socketServerFileDescriptor;
}

void toggleSpecialState(struct Semaforo* trafficLight, int specialState){
    if(trafficLight->estado == specialState){
        signal(SIGUSR1, SIGUSR1Handler);
        trafficLight->estado = trafficLight->estadoAnterior;
        if(trafficLight->estadoAnterior == VERDE){
            puts("Activando alarm");
            alarm(TRAFFIC_LIGHT_DURATION);
        }
    } else {
        trafficLight->estadoAnterior = trafficLight->estado == VERDE ? VERDE: ROJO;
        signal(SIGUSR1, SIG_IGN);
        trafficLight->estado = specialState;
    }
    sendStatusToConsole();
}

void sendStatusToConsole(){
    struct Semaforo* trafficLight = getTrafficLight(trafficLightID);
    char msgText[40];
    sprintf(msgText, "El semáforo %lu está en %s\n",
           trafficLightID,
           estadoToString(trafficLight->estado));
    size_t msgSize = sizeof(msgText);
    printf("%s", msgText);
    write(trafficLight->serverFileDescriptor, msgText, msgSize);
}