enum EstadoSemaforo {
    ROJO, VERDE, INTERMITENTE
};

char* estadoToString(enum EstadoSemaforo estado){
    if(estado == ROJO)
        return "rojo";
    if(estado == VERDE)
        return "verde";
    return "intermitente";
}
