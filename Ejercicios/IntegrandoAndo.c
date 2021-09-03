#include <stdlib.h>
#include <stdio.h>
#include<math.h>
#include <unistd.h>
#include<sys/wait.h>

typedef double(*Fx)(double);

double area(Fx f, double a, double diff){
  double altura1 = f(a), altura2 = f(a+diff);
  double promedioAlturas = (altura1 + altura2) / 2;
  return diff * promedioAlturas;
}

double integrar(Fx f, double a, double b, int muestras){
  double resultado = 0;
  const double diff = (b - a) / muestras;
  for(int i = 0; i < muestras; i++){
    resultado += area(f, a + diff * i, diff);
  }
  return resultado; 
} 

double alCuadrado(double x){
  return x * x / 3;
}

void procesamientoHijos(pid_t pid, int idHijo, double a, double b, double* const ptrResultado){
  const int NUM_MUESTRAS = 1000000;
  if(pid == -1){
    printf("Hubo un problema con el hijo %d, terminando el programa", idHijo);
    exit(1);
  } else if(pid == 0){
    double resultado = integrar(alCuadrado, a, b, NUM_MUESTRAS);
    *ptrResultado = resultado;
    printf("%lf\n", *ptrResultado);
    exit(0);
  }
}

int main(){
  pid_t hijo1 = 0, hijo2 = 0;
  const double a = 1, b = 15.3;
  const double mitad = (b - a) / 2;

  double resHijo1;
  hijo1 = fork();
  procesamientoHijos(hijo1, 1, a, mitad, &resHijo1);
  double resHijo2;
  hijo2 = fork();
  procesamientoHijos(hijo2, 2, mitad, b, &resHijo2);

  double resultado = 0;
  int estado = 0;
  if(waitpid(hijo1, &estado, 0) != -1)
    if(WIFEXITED(estado))
      resultado += resHijo1;
  if(waitpid(hijo2, NULL, 0) != -1)
    if(WIFEXITED(estado))
      resultado += resHijo2;
  
  printf("El resultado de integrar f(x): de %.02lf a %.02lf = %lf\n", a, b, resultado);
}
