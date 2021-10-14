struct Seccion {
    int pesoMax;
    int pesoActual;  
    int id;  
};

struct Robot {
    int *lista;
    int pesoActual;
    int id;
};


int getRandomNumber();
void asignarPesoMaxSecciones(struct Seccion* arregloSecciones);
void asignarPesoYListaRobots(struct Robot* arregloRobots);

void * compraSeccion(void *);
void * salirSeccion(void * args);
int getMinValueSeccion();