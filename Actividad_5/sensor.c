/*
  Archana Verma Rodríguez   A01335895
  Diego Montaño Martínez    A01651308
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void logErrorAndExit(const char* errorMsg);

const char* SERVER_IP_ADDRESS = "127.0.0.1";
const int TCP_PORT = 8000;

int main() {
    srand(time(NULL));
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

    // Start sending input to server
    while(1){
        short sensorRead = (short) (rand() % 256);
        printf("Sending a reading of %d to server\n", sensorRead);
        write(socketServerFileDescriptor, &sensorRead, sizeof sensorRead);
        sleep(3);
    }
    
    // Cleaning up
    puts("Closing sockets");
    close(socketServerFileDescriptor);
}

void logErrorAndExit(const char* errorMsg){
    puts(errorMsg);
    puts("Terminating program");
    exit(1);
}