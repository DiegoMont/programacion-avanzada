#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void manejadorCrtlZ(int ids){
    printf("Los procesos no pueden ser detenidos\n");
}

int main(){
    
    if(signal(SIGTSTP, manejadorCrtlZ) == SIG_ERR)
        puts("Error");
    
    pid_t pidh;
    pidh = fork();
    
    if(pidh == 0){
        if(signal(SIGINT, SIG_IGN) == SIG_ERR )
            puts("Error");
        sleep(2);
        pid_t ppid = getppid();
        pidh = getpid();
        printf("PPID: %d y PID: %d\n", ppid, pidh);
        for (int i = 10; i > 0; i--){
            printf("%d ", i);
            sleep(1);
        }
        puts("\nTerminando al hijo");
    }else if(pidh == -1){
        puts("Error creando hijo");
    } else {
        sleep(10);
        wait(NULL);
    }
}
