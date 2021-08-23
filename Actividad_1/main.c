#include<stdio.h>
#include<stdlib.h>

#include "Barco.c"
#include "Tripulante.c"


int main(){
  struct Barco puerto[1];
  puerto[0].nombre = "nombre";
  printf("%s\n", puerto[0].nombre);
  /* int totalBarcos = 0;
  agregarBarco(puerto, &totalBarcos);
  printf("%p\n", puerto[0].nombre);
  printf("%p\n", puerto); */
}
