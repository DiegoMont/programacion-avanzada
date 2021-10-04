enum EstadoSemaforo {
    ROJO, VERDE, INTERMITENTE
};

struct Semaforo {
    enum EstadoSemaforo estadoAnterior;
    enum EstadoSemaforo estado;
    pid_t pid;
    int serverFileDescriptor;
};

const int NUMBER_OF_TRAFFIC_LIGHTS = 2;

char* estadoToString(enum EstadoSemaforo estado){
    if(estado == ROJO)
        return "rojo";
    if(estado == VERDE)
        return "verde";
    return "intermitente";
}
