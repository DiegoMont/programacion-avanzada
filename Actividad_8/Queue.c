#include <stdlib.h>

struct Nodo {
  struct Cliente* cliente;
  struct Nodo* siguiente;
  struct Nodo* anterior;
};

struct Queue {
  struct Nodo* first;
  struct Nodo* last;
  size_t length;
};

struct Queue* queue(){
  struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
  queue->length = 0;
  queue->first = NULL;
  queue->last = NULL;
  return queue;
}

int isEmpty(struct Queue* queue){
  return queue->first == NULL;
}

void enqueue(struct Queue* q, struct Cliente* cliente){
    struct Nodo* nodoNuevo = (struct Nodo*) malloc(sizeof(struct Nodo));
    nodoNuevo->cliente = cliente;
    if(q->first == NULL){
        q->first = nodoNuevo;
        q->last = nodoNuevo;
    } else {
        q->first->anterior = nodoNuevo;
        nodoNuevo->siguiente = q->first;
        q->first = nodoNuevo;
    }
    q->length++;
}

struct Cliente* dequeue(struct Queue* q) {
    if(q->last == NULL)
        return NULL;
    else {
        struct Nodo* nodoAQuitar = q->last;
        struct Cliente* cliente = nodoAQuitar->cliente;
        q->last = nodoAQuitar->anterior;
        if(q->last == NULL)
          q->first = NULL;
        free(nodoAQuitar);
        q->length--;
        return cliente;
    }
}
