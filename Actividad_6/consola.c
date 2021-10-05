#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "consola.h"
#include "EstadoSemaforo.c"

const char* SERVER_IP_ADDRESS = "127.0.0.1";
const int TCP_PORT = 8000;

int trafficLightsFileDescriptors[NUMBER_OF_TRAFFIC_LIGHTS];

int main(){
    int socketServerFileDescriptor = initSocket();
    establishConnectionWithTrafficLights(socketServerFileDescriptor, trafficLightsFileDescriptors);
    signal(SIGTSTP, handler);
    while(1){
        char buffer[40];
        for(int trafficLight = 0; trafficLight < NUMBER_OF_TRAFFIC_LIGHTS; trafficLight++){
            int readedBytes = read(*(trafficLightsFileDescriptors + trafficLight), buffer, sizeof buffer);
            if (readedBytes > 0)
                puts(buffer);
        }
    }
}

int initSocket(){
    puts("Iniciando consola...");
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

void establishConnectionWithTrafficLights(int serverFileDescriptor, int* clientFileDescriptors){
    struct sockaddr_in socketAddressInfo;
    socklen_t addressInfoSize = sizeof(socketAddressInfo);
    for(int i = 0; i < NUMBER_OF_TRAFFIC_LIGHTS; i++){
        int clientFileDescriptor = accept(
            serverFileDescriptor,
            (struct sockaddr*) &socketAddressInfo,
            &addressInfoSize);
        if(clientFileDescriptor == -1){
            close(serverFileDescriptor);
            logErrorAndExit("Couldn't accept client");
        }
        *(clientFileDescriptors + i) = clientFileDescriptor;
    }
    puts("Connection with traffic lights established");
}

void handler(int s){
    for(int trafficLight = 0; trafficLight < NUMBER_OF_TRAFFIC_LIGHTS; trafficLight++){
        char command[14];
        if(s == SIGTSTP)
            strcpy(command, "ROJO");
        else
            strcpy(command, "INTERMITENTE");
        write(*(trafficLightsFileDescriptors + trafficLight), command, sizeof command);
    }
}

void logErrorAndExit(const char* errorMsg){
    puts(errorMsg);
    puts("Terminating program");
    exit(1);
}
