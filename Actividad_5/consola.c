/*
  Archana Verma Rodríguez   A01335895
  Diego Montaño Martínez    A01651308
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <limits.h>

#include "servidor.h"

const char* SERVER_IP_ADDRESS = "127.0.0.1";
const int TCP_PORT = 8000;

const int MAX_NUMBER_OF_SENSORS = 10;
const size_t DAYS_IN_MONTH = 30;
const clock_t SECONDS_IN_DAY = 10; //24 * 60 * 60;

struct Day{
    short int min;
    short int max;
    int sum;
    int counterData;
};

struct Day* sensorsReadings;
clock_t readingStart;

int main(){
    size_t bytesNeededToStoreInfo = MAX_NUMBER_OF_SENSORS * DAYS_IN_MONTH * sizeof(struct Day);
    sensorsReadings = createSharedMemory(bytesNeededToStoreInfo);
    readingStart = getCurrentSeconds();
    pid_t pid;
    pid = fork();

    if(pid == -1){
        logErrorAndExit("No se pudo crear un proceso");
    } else if(pid == 0){
        showDailyReports();
    } else {
        startListeningToConnections();
    }

    // Cleaning up
    munmap(sensorsReadings, bytesNeededToStoreInfo);
}

struct Day* createSharedMemory(size_t size) {
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
    void* mappedMemory = mmap(NULL, size, protection, visibility, 0, 0);
    if(mappedMemory == MAP_FAILED){
        printf("Mapping Failed\n");
        exit(1);
    }
    return (struct Day*) mappedMemory;
}

clock_t getCurrentSeconds(){
    return clock() / CLOCKS_PER_SEC;
}

void showDailyReports(){
    for(int day = 0; day < DAYS_IN_MONTH; day++){
        int dayIsOver;
        do {
            clock_t currentTime = getCurrentSeconds();
            clock_t dayEnd = day * SECONDS_IN_DAY + SECONDS_IN_DAY;
            dayIsOver = currentTime > dayEnd;
        } while(!dayIsOver);
        printf("Información del día %d:\n", day + 1);
        for(int sensor = 0; sensor < MAX_NUMBER_OF_SENSORS; sensor++){
            struct Day sensorDailyInfo = *(sensorsReadings + (sensor * DAYS_IN_MONTH + day ));
            if(sensorDailyInfo.counterData == 0)
              break;
            double media = sensorDailyInfo.sum / (double) sensorDailyInfo.counterData;
            printf("Max: %d    Min: %d   Promedio: %lf\n",
                   sensorDailyInfo.max,
                   sensorDailyInfo.min,
                   media);
        }
    }
}

int initSocket(){
    puts("Iniciando el Servidor Central...");
    // Create socket
    int socketServerFileDescriptor;
    socketServerFileDescriptor = socket(PF_INET, SOCK_STREAM, 0);
    if(socketServerFileDescriptor == -1)
        logErrorAndExit("Couldn't create socket");
    
    // Bind IP address to socket
    struct sockaddr_in socketAddressInfo;
    socketAddressInfo.sin_port = htons(TCP_PORT);
    socketAddressInfo.sin_family = AF_INET;
    inet_aton(SERVER_IP_ADDRESS, &socketAddressInfo.sin_addr);
    socklen_t addressInfoSize = sizeof(socketAddressInfo);
    bind(socketServerFileDescriptor,
         (struct sockaddr *) &socketAddressInfo,
         addressInfoSize);
    
    // Start listening
    int listeningStatus = listen(socketServerFileDescriptor, 10);
    if(listeningStatus == -1)
      logErrorAndExit("Can't listen in that port");
    puts("Socket is ready and listening to requests");
    return socketServerFileDescriptor;
}

void openSessionWithSensor(int socketClientFileDescriptor, int sensorIndex){
    printf("Start connection with sensor %d\n",sensorIndex);
    for(int day = 0; day < DAYS_IN_MONTH; day++){
        clock_t dayEnd = day * SECONDS_IN_DAY + SECONDS_IN_DAY;
        struct Day* sensorDailyInfo = sensorsReadings + sensorIndex * DAYS_IN_MONTH + day;
        sensorDailyInfo->min = SHRT_MAX;
        int dayIsOver = 0;
        while(!dayIsOver){
            short buffer;
            size_t bufferSize = sizeof buffer;
            int readedBytes = read(socketClientFileDescriptor, &buffer, bufferSize);
            if( readedBytes == bufferSize){
                printf("Receiving %d from sensor %d\n", buffer, sensorIndex);
                short maxValue = (buffer > sensorDailyInfo->max) ? buffer: sensorDailyInfo->max;
                short minValue = (buffer < sensorDailyInfo->min) ? buffer: sensorDailyInfo->min;
                sensorDailyInfo->max = maxValue;
                sensorDailyInfo->min = minValue;
                sensorDailyInfo->sum += buffer;
                sensorDailyInfo->counterData++;
            }
            clock_t currentTime = getCurrentSeconds();
            dayIsOver = currentTime > dayEnd;  
        }
    }
}

void startListeningToConnections(){
    int socketServerFileDescriptor = initSocket();
    int proceed = 1;
    pid_t pidc;
    int counterSensors = -1;

    struct sockaddr_in socketAddressInfo;
    int socketClientFileDescriptor;
    socklen_t addressInfoSize = sizeof(socketAddressInfo);
    while(proceed){
        //Establish connection with client
        socketClientFileDescriptor = accept(socketServerFileDescriptor, (struct sockaddr *) &socketAddressInfo, &addressInfoSize);
        if(socketClientFileDescriptor == -1){
            close(socketServerFileDescriptor);
            logErrorAndExit("Couldn't accept client");
        }
        printf("Client connected to %s:%d \n",
            inet_ntoa(socketAddressInfo.sin_addr),
            ntohs(socketAddressInfo.sin_port));
        
        counterSensors++;
        pidc = 0;
        pidc = fork();
        if (pidc == 0) proceed = 0;
    }
    if (pidc == 0) {
        //child's process
        close(socketServerFileDescriptor);
        if(socketClientFileDescriptor >= 0){
            openSessionWithSensor(socketClientFileDescriptor, counterSensors);
            
        }
    }
}

void logErrorAndExit(const char* errorMsg){
    puts(errorMsg);
    puts("Terminating program");
    exit(1);
}