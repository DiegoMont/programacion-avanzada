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

struct Queue* vector(){
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
    if(q->first == NULL)
        return NULL;
    else {
        struct Cliente* cliente = q->last->cliente;
        q->last = q->last->anterior;
        free(q->last->siguiente);
        q->length--;
        return cliente;
    }
}
