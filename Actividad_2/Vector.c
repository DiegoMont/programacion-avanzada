#include<stdlib.h>

#include "Libro.c"

#define iterator struct Nodo*

struct Nodo {
  void* valor;
  struct Nodo* siguiente;
  struct Nodo* anterior;
};

struct Vector {
  struct Nodo* first;
  size_t length;
};

struct Vector* vector(){
  struct Vector* vector = (struct Vector*) malloc(sizeof(struct Vector));
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

struct Vector* vectorNCopias(size_t n, void* element){
  struct Vector* vectorAux = vector();
  for(int i = 0; i < n; i++)
    push_back(vectorAux, element);
  return vectorAux;
}

struct Vector* vectorN(size_t n){
  return vectorNCopias(n, NULL);
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
}

iterator next(iterator pos){
  return pos == NULL ? NULL: pos->siguiente;
}

iterator erase(struct Vector* vector, iterator pos){
  if(pos == NULL)
    return NULL;
  iterator followingElement = pos->siguiente;
  if(pos->anterior != NULL)
    pos->anterior->siguiente = followingElement;
    pos->siguiente->anterior = pos->anterior;
  if(pos == vector->first)
    vector->first = followingElement;
  free(pos);
  vector->length--;
  return followingElement;
}

iterator eraseRange(struct Vector* vector, iterator first, iterator last){
  while(last != first){
    first = erase(vector, first);
  }
  return first;
}

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

void printerChar(struct Vector* vector){
  struct Nodo* aux = vector->first;
    while (aux != NULL)
    {
      printf(" %c", *(char *)aux->valor);
      aux = aux->siguiente;
    }
    printf("\n");
} 




