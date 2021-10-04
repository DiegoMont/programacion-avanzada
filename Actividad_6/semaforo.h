void alarmHandler(int s);
void beATrafficLight();
struct Semaforo* createSharedMemory(size_t numberOfElements);
pid_t getNextTrafficLightPID();
struct Semaforo* getTrafficLight(size_t id);
void logErrorAndExit(const char* errorMsg);
void SIGUSR1Handler(int s);
