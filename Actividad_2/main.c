#include <stdio.h>

#include "Vector.c"
//#include "Libro.c"


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
  struct Vector* lista = vector(sizeof libro);
  
  push_back(lista, &libro);
  push_back(lista, &libro2);
  push_back(lista, &libro3);

  struct Libro libro4;
  libro4.titulo = "Libro 4";
  libro4.paginas = 30;

  insertN(lista, end(lista), 3, &libro4);
  printer(lista,&printerStruct);





}
