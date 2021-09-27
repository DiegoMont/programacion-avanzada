#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include "servidor.h"

const char* SERVER_IP_ADDRESS = "127.0.0.1";
const int TCP_PORT = 8000;

struct Day{
    short int min;
    short int max;
    int sum;
    int counterData;
};

struct Day* sensorsReadings;

int main(){
    const int MAX_NUMBER_OF_SENSORS = 10;
    const size_t DAYS_IN_MONTH = 30;
    size_t bytesNeededToStoreInfo = MAX_NUMBER_OF_SENSORS * DAYS_IN_MONTH * sizeof(struct Day);
    sensorsReadings = createSharedMemory(bytesNeededToStoreInfo);
    
    pid_t pid;
    pid = fork();

    if(pid == 0){
        //child's process
        int socketServerFileDescriptor = initSocket();
        int proceed = 1;
        pid_t pidc;

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
            
            pidc = fork();
            if (pidc == 0) proceed = 0;
        }
        if (pidc == 0) {
            //child's process
            close(socketServerFileDescriptor);
            if(socketClientFileDescriptor >= 0){
                //falta lo de recibir la información

            }
        }
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
    bind(socketServerFileDescriptor, (struct sockaddr *) &socketAddressInfo, addressInfoSize);
    
    // Start listening
    int listeningStatus = listen(socketServerFileDescriptor, 10);
    if(listeningStatus == -1)
      logErrorAndExit("Can't listen in that port");
    puts("Socket is ready and listening to requests");
    return socketServerFileDescriptor;
}

void logErrorAndExit(const char* errorMsg){
    puts(errorMsg);
    puts("Terminating program");
    exit(1);
}
