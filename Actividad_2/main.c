#include <stdio.h>

#include "Vector.c"
#include "Libro.c"

int main(){
  struct Libro libro;
  libro.paginas = 5;
  struct Libro libro2;
  libro2.paginas = 10;
  struct Libro libro3;
  libro3.paginas = 15;
  struct Vector* lista = vector(sizeof libro);
  
  push_back(lista, &libro);
  push_back(lista, &libro2);
  // push_back(lista, &libro3);
  //vector_copies(lista,2,&libro);
  // insert(lista, 3,&libro3);
  // insert(lista, 3,&libro2);
  // insert(lista, 3,&libro2);

  insert_copies(lista, 3,3, &libro3);
  int paginas = ((struct Libro*) back(lista))->paginas;

  printf("El libro tiene %d paginas\n", paginas);
  printf("El vector tiene %zu nodos\n", lista->length);



}