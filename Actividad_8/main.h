void cleanSemaforos();
useconds_t getRandomTimePeriod(useconds_t min, useconds_t max);
void iniciarCajeros(pthread_t* cajero_threads);
void iniciarSemaforos();
void mandarClientesAlBanco(pthread_t* clientes);
void* realizarOperacionEmpresarial(void* args);
void* realizarOperacionGeneral(void* args);
void* serUnCajero(void* args);
