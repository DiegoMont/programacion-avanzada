#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "main.h"

/*
Hay clientesGeneralesFormados, clientesEmpresarialesFormados
En un arreglo guardamos el estado de los cajeros, los primeros tres cajeros son empresariales el resto de las operaciones.
El estado de los cajeros es disponible u ocupado
Tenemos otro semáforo que indica cuántos cajerosDisponibles tenemos
Cuando un cajero está listo para atender incrementa cajerosDisponibles:
  Los clientes checan cuál cajero está disponible:
    Si es empresarial y son empresariales se van a ese cajero y lo marcan como ocupado
    Si es empresarial y son generales lo toman únicamente si no hay clientes empresariales esperando
    Si es general y son generales lo toman
    Si es general y son empresariales GG
*/

int main(){
    srand(time(NULL));
}

