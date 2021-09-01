#include <stdio.h>

#include "Vector.c"
#include "Libro.c"

int main(){
  struct Libro libro;
  libro.paginas = 5;
  struct Libro libro2;
  libro.paginas = 10;
  struct Libro libro3;
  libro.paginas = 15;
  struct Vector* lista = vector(sizeof libro);
  push_back(lista, &libro);
  push_back(lista, &libro2);
  push_back(lista, &libro3);
  int paginas = ((struct Libro*) front(lista))->paginas;
  printf("El libro tiene %d paginas\n", paginas);
}