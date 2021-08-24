#include "RolTripulante.c"

struct Tripulante{
    char* nombre;
    char* apellidos;
    int edad;
    enum RolTripulante rol;
};

char* rolToString(enum RolTripulante rol){
    if(rol == ARPONERO)
      return "Arponero";
    if(rol == COCINERO)
      return "Cocinero";
    if(rol == CAPITAN)
      return "Capitán";
    return "Vigía";
}

void getRol(struct Tripulante* tripulante){
    printf("1. %s\n", rolToString(ARPONERO));
    printf("2. %s\n", rolToString(COCINERO));
    printf("3. %s\n", rolToString(VIGIA));
    printf("4. %s\n", rolToString(CAPITAN));
    printf("Selecciona el rol de %s: ", (*tripulante).nombre);
    int opcion;
    scanf("%d", &opcion);
    (*tripulante).rol = opcion - 1;
}

void getTripulanteNuevo(struct Tripulante* tripulante){
    (*tripulante).nombre = (char*) malloc(30 * sizeof(char));
    printf("Ingrese el nombre del tripulante: ");
    scanf("%s", (*tripulante).nombre);
    (*tripulante).apellidos = (char*) malloc(60 * sizeof(char));
    printf("Ingrese los apellidos: ");
    scanf("%s", (*tripulante).apellidos);
    printf("Ingrese la edad del tripulante: ");
    scanf("%d", &(*tripulante).edad);
    getRol(tripulante);
}
