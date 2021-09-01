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

struct Vector* vector_elements( size_t dataTypeSize, size_t times){
  struct Vector* newVector = vector(dataTypeSize);
  while(newVector->length < times){
    struct Nodo* nodoNuevo = (struct Nodo*) malloc(sizeof(struct Nodo));
    nodoNuevo->siguiente = NULL;
    if(isEmpty(newVector)){
      newVector->first = nodoNuevo;
      nodoNuevo->anterior = NULL;
    }else{
      struct Nodo* nodoFinal = end(newVector);
      nodoFinal->siguiente = nodoNuevo;
      nodoNuevo->anterior = nodoFinal;
    }
    newVector->length++;
  }
return newVector;
}



void vector_copies(struct Vector* vector, size_t times, void* element){
  size_t i = 0;
  while(i < times){
    push_back(vector,element);
    i++;
  }
  
}

void* front(struct Vector* vector){
  struct Nodo* nodoFirst = begin(vector);
  if(nodoFirst == NULL)
    return NULL;
  return nodoFirst->valor;
}

void* back(struct Vector* vector){
  struct Nodo* nodoFinal = end(vector);
  return nodoFinal->valor;
}


void insert(struct Vector* vector, size_t pos, void* element){
  size_t counter = 1;
  if(pos == vector->length + 1)
    push_back(vector, element);
  else{
    struct Nodo* nodoNuevo = (struct Nodo*) malloc(sizeof(struct Nodo));
    nodoNuevo->valor = element;
    if( counter == pos){
      nodoNuevo->anterior = NULL;
      nodoNuevo->siguiente=vector->first;
      vector->first= nodoNuevo;
    }else{
      struct Nodo* aux = vector->first;
      while(counter < pos ){
      aux = aux->siguiente;
      counter++;
      }
      nodoNuevo->siguiente = aux;
      nodoNuevo->anterior =aux->anterior;
      aux->anterior->siguiente = nodoNuevo;
      aux->anterior = nodoNuevo;
      
    }
  vector->length++;
  }
}

void insert_copies(struct Vector* vector, size_t pos,size_t times, void* element){
  size_t i = 0;
  while( i < times){
    insert(vector,pos,element);
    i++;
  }
}
