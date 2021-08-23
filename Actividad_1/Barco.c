#include "Tripulante.c"

struct Barco {
  char* nombre;
  char* propietario;
  float eslora;
  float manga;
  int max_tripulantes;
};

void imprimirBarco(struct Barco* barco){
  printf("Barco: %s\n", (*barco).nombre);
  printf("Propietario: %s\n", (*barco).propietario);
  printf("Eslora: %.2f\n", (*barco).eslora);
  printf("Manga: %.2f\n", (*barco).manga);
  printf("Máximo de tripulantes permitidos: %d\n", (*barco).max_tripulantes);
}

void getBarcoNuevo(struct Barco* barco){
  (*barco).nombre = (char*) malloc(30 * sizeof(char));
  printf("Ingrese el nombre del barco: ");
  scanf("%s", (*barco).nombre);
  (*barco).propietario = (char*) malloc(50 * sizeof(char));
  printf("Ingrese el nombre del propietario: ");
  scanf("%s", (*barco).propietario);
  printf("Ingrese el tamaño de la eslora: ");
  scanf("%f", &(*barco).eslora);
  printf("Ingrese el tamaño de la manga: ");
  scanf("%f", &(*barco).manga);
  printf("Ingrese el máximo de tripulantes que acepta el barco: ");
  scanf("%d", &(*barco).max_tripulantes);
}

struct Barco* agregarBarco(struct Barco* puerto, int* numeroBarcos){
  int sizeOfPuertoNuevo = (*numeroBarcos + 1) * sizeof(struct Barco);
  puerto = (struct Barco*) realloc(puerto, sizeOfPuertoNuevo);
  struct Barco* refANuevoBarco = puerto + *numeroBarcos;
  getBarcoNuevo(refANuevoBarco);
  (*numeroBarcos)++;
  return puerto;
}
