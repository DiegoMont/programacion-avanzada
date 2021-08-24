/*
  Archana Verma Rodríguez   A01335895
  Diego Montaño Martínez    A01651308
*/

#include<stdio.h>
#include<stdlib.h>

#include "Barco.c"

void imprimirMenuPrincipal(struct Barco* puerto, int totalBarcos){
  printf("Qué deseas hacer:\n");
  printf("1. Ver registro de todas las embarcaciones\n");
  printf("2. Agregar un barco al registro\n");
  printf("Opciones para modificar barco individualmente:\n");
  struct Barco *aux = puerto;
  struct Barco *fin = puerto + totalBarcos;
  int i = 0;
  for(; aux < fin; aux++){
    int opcionADesplegar = i + 3;
    printf("%d. %s\n", opcionADesplegar, (*aux).nombre);
    i++;
  }
  printf("Ingrese una opción:");
}

void imprimirRegistro(struct Barco* puerto, int totalBarcos){
  struct Barco *aux = puerto;
  struct Barco *fin = puerto + totalBarcos;
  for(; aux < fin; aux++){
    printf("************************************\n");
    imprimirBarco(aux);
    printf("************************************\n");
  }
}

void irAMenuBarco(struct Barco* barco){
  int seleccion = -1;
  while(seleccion != 1){
    int hayEspaciosDisponibles = (*barco).max_tripulantes > (*barco).totalTripulacion;
    printf("Menú de %s\n", (*barco).nombre);
    printf("1. Regresar al menú principal\n");
    printf("2. Imprimir tripulación\n");
    if(hayEspaciosDisponibles)
      printf("3. Agregar miembro a la tripulación\n");
    printf("Elige una opción: ");
    scanf("%d", &seleccion);
    if(seleccion == 1)
      continue;
    else if(seleccion == 2)
      imprimirTripulacion(barco);
    else if(hayEspaciosDisponibles && seleccion == 3)
      agregarTripulante(barco);
    else
      printf("Opción no válida\n");
  }
}

int main(){
  int maxBarcos = 1;
  int initialSizeOfHarbour = maxBarcos * sizeof(struct Barco);
  struct Barco* puerto = malloc(initialSizeOfHarbour);
  int barcosEnPuerto = 0;
  printf("No hay barcos en el puerto, comencemos agregando uno\n");
  puerto = agregarBarco(puerto, &barcosEnPuerto, &maxBarcos);
  while(1){
    imprimirMenuPrincipal(puerto, barcosEnPuerto);
    int seleccion;
    scanf("%d", &seleccion);
    if(seleccion == 1)
      imprimirRegistro(puerto, barcosEnPuerto);
    else if(seleccion == 2)
      puerto = agregarBarco(puerto, &barcosEnPuerto, &maxBarcos);
    else if(seleccion < barcosEnPuerto + 3)
      irAMenuBarco(puerto + seleccion - 3);
    else
      printf("Opción no válida\n");
  }
}
