#include<stdlib.h>

struct Nodo {
  void* valor;
  struct Nodo* siguiente;
  struct Nodo* anterior;
};

struct Vector {
  struct Nodo* first;
  size_t dataTypeSize;
  size_t length;
};

const size_t VECTOR_INITIAL_SIZE = 10;

/* struct Vector* vector(size_t n, void* t, size_t dataTypeSize){
}

void* vector(size_t n, size_t dataTypeSize){

} */

struct Vector* vector(size_t dataTypeSize){
  struct Vector* vector = (struct Vector*) malloc(sizeof(struct Vector));
  vector->dataTypeSize = dataTypeSize;
  vector->length = 0;
  vector->first = NULL;
  return vector;
}

int isEmpty(struct Vector* vector){
  return vector->first == NULL;
}

struct Nodo* begin(struct Vector* vector){
  return vector->first;
}

struct Nodo* end(struct Vector* vector){
  struct Nodo* aux = vector->first;
    while(aux->siguiente != NULL){
      aux = aux->siguiente;
    }
    return aux;
}

void push_back(struct Vector* vector, void* element){
  struct Nodo* nodoNuevo = (struct Nodo*) malloc(sizeof(struct Nodo));
  nodoNuevo->valor = element;
  nodoNuevo->siguiente = NULL;
  if (isEmpty(vector)) {
    vector->first = nodoNuevo;
    nodoNuevo->anterior = NULL;
  } else {
    struct Nodo* nodoFinal = end(vector);
    nodoFinal->siguiente = nodoNuevo;
    nodoNuevo->anterior = nodoFinal;
  }
  vector->length++;
}

void* front(struct Vector* vector){
  struct Nodo* aux = begin(vector);
  if(aux == NULL){
    return NULL;
  }
  return aux->valor;
}

void* back(struct Vector* vector){
  struct Nodo* nodoFinal = end(vector);
  return nodoFinal->valor;
}
