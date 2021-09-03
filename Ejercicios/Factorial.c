#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include<sys/wait.h>

long long factorial(long long n){
  return n > 1 ? n * factorial(n-1): 1;
}

int main(){
  int input;
  do {
    printf("Ingrese un número: ");
    scanf("%d", &input);
    pid_t pid = fork();
    if(pid == 0){
      long long resultado = factorial(input);
      printf("El factorial de %d es %lld\n", input, resultado);
      exit(0);
    }
  } while(input > -1);
}
