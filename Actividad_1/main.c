#include<stdio.h>
#include<stdlib.h>

#include "Barco.c"
#include "Tripulante.c"


int main(){
  struct Barco barquito;
  barquito.max_tripulantes = 6;
  printf("%d", barquito.max_tripulantes);

  struct Tripulante tripulante;
  tripulante.rol = CAPITAN;
  printf("%d", tripulante.rol);
}
