struct Barco {
  char * nombre;
  double eslora;
  double manga;
  int max_tripulantes;

};

/* void agregarBarco(struct Barco* puerto, int* numeroBarcos){
  puerto = realloc(puerto, (*numeroBarcos + 1) * sizeof(struct Barco));
  printf("%p\n", puerto);
  struct Barco* fin = puerto + (*numeroBarcos * sizeof(struct Barco));
  (*numeroBarcos)++;
  printf("El nuevo tamaño del puerto es de %d\n", *numeroBarcos);
  struct Barco barcoNuevo;
  *fin = barcoNuevo;
  char nombre[30];
  printf("Ingresa el nombre del barco: ");
  scanf(" %s", nombre);
  barcoNuevo.nombre = nombre;
  printf("%p\n", nombre);
  printf("%p\n", barcoNuevo.nombre);
} */