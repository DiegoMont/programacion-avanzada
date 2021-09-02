#include<stdlib.h>

#include "Libro.c"

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

struct Nodo* insert(struct Vector* vector, struct Nodo* pos, void* x){
  if(pos == NULL)
    return NULL;
  void* valorActual = pos->valor;
  struct Nodo* nodoNuevo = (struct Nodo*) malloc(sizeof(struct Nodo));
  nodoNuevo->valor = valorActual;
  nodoNuevo->anterior = pos;
  nodoNuevo->siguiente = pos->siguiente;
  pos->siguiente = nodoNuevo;
  pos->valor = x;
  vector->length++;
  return pos;
}

void insertN(struct Vector* vector, struct Nodo* pos, size_t n, void* x){
  if(pos == NULL)
    return;
  void* valorActual;
  for(int i = 0; i < n; i++)
    insert(vector, pos, x);
}

void pop_back(struct Vector* vector){
  struct Nodo* nodoFinal = end(vector);
  if(nodoFinal == NULL)
    return;
  if(nodoFinal->anterior == NULL){
    free(nodoFinal);
    vector->first = NULL;
  }else{
    nodoFinal->anterior->siguiente = NULL;
    free(nodoFinal);
  }
  vector->length--;
}


void clear(struct Vector* vector){
  while(vector->first!=NULL){
    pop_back(vector);
  } 
  free(vector);
}

void* next(struct Vector* vector){
  struct Nodo* aux= vector->first;
  return aux->siguiente;
}



/* void errase(struct Vector* vector, size_t pos){

} */



typedef void (*t_print)(struct Vector* vector);

void printer(struct Vector* vector, t_print algorithm){
  (*algorithm)(vector);
}

void printerStruct(struct Vector* vector){
  struct Nodo* aux = vector->first;
    while (aux != NULL)
    {
      printf("%s tiene %d paginas\n", ((struct Libro*) aux->valor)->titulo,((struct Libro*) aux->valor)->paginas);
      aux = aux->siguiente;
    }
} 

void printerInt(struct Vector* vector){
  struct Nodo* aux = vector->first;
    while (aux != NULL)
    {
      printf(" %4d", *(int *)aux->valor);
      aux = aux->siguiente;
    }
    printf("\n");
} 

void printerFloat(struct Vector* vector){
  struct Nodo* aux = vector->first;
    while (aux != NULL)
    {
      printf(" %7.2f", *(float *)aux->valor);
      aux = aux->siguiente;
    }
    printf("\n");
} 




