#include "Tripulante.c"

struct Barco {
  char* nombre;
  float eslora;
  float manga;
  int max_tripulantes;
};

void imprimirBarco(struct Barco* barco){
  printf("Barco: %s\n", (*barco).nombre);
  printf("Eslora: %.2f\n", (*barco).eslora);
  printf("Manga: %.2f\n", (*barco).manga);
  printf("Máximo de tripulantes permitidos: %d\n", (*barco).max_tripulantes);
}

void getBarcoNuevo(struct Barco* barco){
  char* tempNombre = (char*) malloc(30 * sizeof(char));
  printf("Ingrese el nombre del barco: ");
  scanf("%s", tempNombre);
  (*barco).nombre = tempNombre;
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
