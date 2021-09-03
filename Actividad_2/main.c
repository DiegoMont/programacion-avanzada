/*
  Archana Verma Rodríguez   A01335895
  Diego Montaño Martínez    A01651308
*/

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
  struct Vector* listaEnteros = vectorNCopias(3, &uno);
  push_back(listaEnteros, &dos);
  push_back(listaEnteros, &tres);
  iterator segundoEntero = next(begin(listaEnteros));
  printf("Valor del segundo elemento: %d\n", *((int*)(segundoEntero->valor)));
  erase(listaEnteros, segundoEntero);
  printer(listaEnteros,&printerInt);
  printf("Length arreglo: %zu\n", listaEnteros->length);
  printf("Valor del último elemento: %d\n", *((int*) back(listaEnteros)));


  // lista de caracteres;
  char a = 'a', b = 'b';
  struct Vector* listaCaracteres = vectorN(2);
  iterator primerCaracter = begin(listaCaracteres);
  primerCaracter->valor = &a;
  iterator segundoCaracter = next(primerCaracter);
  segundoCaracter->valor = &b;
  printer(listaCaracteres,&printerChar);
  printf("Valor del primer elemento: %c\n", *((char*) front(listaCaracteres)));

  // Limpiando memoria
  clear(lista);
  free(lista);
  clear(listaEnteros);
  free(listaEnteros);
  clear(listaCaracteres);
  free(listaCaracteres);
}
