#define NUMBER_OF_TRAFFIC_LIGHTS 2

enum EstadoSemaforo {
    ROJO, VERDE, INTERMITENTE, ROJO_ESTATICO
};

struct Semaforo {
    enum EstadoSemaforo estadoAnterior;
    enum EstadoSemaforo estado;
    pid_t pid;
    int serverFileDescriptor;
};

char* estadoToString(enum EstadoSemaforo estado){
    if(estado == ROJO)
        return "rojo";
    if(estado == VERDE)
        return "verde";
    if(estado == INTERMITENTE);
        return "intermitente";
    return "rojo estático";
}
