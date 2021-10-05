void handler(int s);
void establishConnectionWithTrafficLights(int serverFileDescriptor, int* clientFileDescriptors);
int initSocket();
void logErrorAndExit(const char* errorMsg);
