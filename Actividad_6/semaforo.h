void alarmHandler(int s);
void beATrafficLight();
pid_t* createSharedMemory(size_t numberOfElements);
pid_t getNextTrafficLightPID();
void logErrorAndExit(const char* errorMsg);
void SIGUSR1Handler(int s);
