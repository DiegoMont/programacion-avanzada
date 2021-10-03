#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "semaforo.h"

const char* SERVER_IP_ADDRESS = "127.0.0.1";
const int TCP_PORT = 8000;

const int NUMBER_OF_TRAFFIC_LIGHTS = 4;

pid_t* traffic_light_pids;

int main(){
    traffic_light_pids = createSharedMemory(NUMBER_OF_TRAFFIC_LIGHTS);
    
    // Cleaning up
    munmap(traffic_light_pids, NUMBER_OF_TRAFFIC_LIGHTS * sizeof(pid_t));
}

pid_t* createSharedMemory(size_t numberOfElements) {
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
    const size_t size = numberOfElements * sizeof(pid_t);
    void* mappedMemory = mmap(NULL, size, protection, visibility, 0, 0);
    if(mappedMemory == MAP_FAILED){
        printf("Mapping Failed\n");
        exit(1);
    }
    return (pid_t*) mappedMemory;
}
