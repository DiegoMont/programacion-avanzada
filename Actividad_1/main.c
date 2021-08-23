#include<stdio.h>
#include<stdlib.h>

#include "Barco.c"

void imprimirMenuPrincipal(){
  printf("Qué deseas hacer:\n");
  printf("1. Ver registro de todas las embarcaciones\n");
  printf("2. Agregar un barco al registro\n");
  printf("Barcos disponibles:\n");
}

void imprimirRegistro(struct Barco* puerto, int totalBarcos){
  for(int i = 0; i < totalBarcos; i++){
    printf("************************************\n");
    struct Barco* referenciaBarco = puerto + i;
    imprimirBarco(referenciaBarco);
    printf("************************************\n");
  }
}

int main(){
  struct Barco* puerto = malloc(0);
  int barcosEnPuerto = 0;
  printf("No hay barcos en el puerto, comencemos agregando uno\n");
  puerto = agregarBarco(puerto, &barcosEnPuerto);
  while(1){
    imprimirMenuPrincipal();
    int seleccion;
    scanf("%d", &seleccion);
    if(seleccion == 1)
      imprimirRegistro(puerto, barcosEnPuerto);
    else if(seleccion == 2)
      puerto = agregarBarco(puerto, &barcosEnPuerto);
  }
}
