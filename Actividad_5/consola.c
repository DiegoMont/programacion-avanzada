#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#include "servidor.h"

const char* SERVER_IP_ADDRESS = "127.0.0.1";
const int TCP_PORT = 8000;

int main(){
    int socketServerFileDescriptor = initSocket();
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
