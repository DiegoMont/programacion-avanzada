#include <stdio.h>

#include "Vector.c"


int main(){
  struct Libro libro;
  libro.titulo="Libro1";
  libro.paginas = 5;
  struct Libro libro2;
  libro2.titulo="Libro2";
  libro2.paginas = 10;
  struct Libro libro3;
  libro3.titulo="Libro3";
  libro3.paginas = 15;
  struct Libro libro4;
  libro4.titulo = "Libro 4";
  libro4.paginas = 30;

  struct Vector* lista = vector();
  
  push_back(lista, &libro);
  push_back(lista, &libro2);
  iterator segundoElemento = next(begin(lista));
  insert(lista, end(lista), &libro3);
  insertN(lista, segundoElemento, 3, &libro4);
  printer(lista,&printerStruct);

  if(isEmpty(lista))
    puts("lista está vacía.");
  else
    printf("La lista contiene %zu elementos\n\n", lista->length);

  // lista de enteros;
  int uno = 1, dos = 2, tres = 3;
  struct Vector* listaEnteros = vector();
  push_back(listaEnteros, &uno);
  push_back(listaEnteros, &dos);
  push_back(listaEnteros, &tres);
  iterator segundoEntero = next(begin(listaEnteros));
  printf("Valor: %d\n", *((int*)(segundoEntero->valor)));
  erase(listaEnteros, segundoEntero);
  printer(listaEnteros,&printerInt);
  printf("Length arreglo: %zu\n", listaEnteros->length);
}
