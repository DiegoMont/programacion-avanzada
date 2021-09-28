int initSocket();
struct Day* createSharedMemory(size_t size);
clock_t getCurrentSeconds();
void logErrorAndExit(const char* errorMsg);
void showDailyReports();
void startListeningToConnections();
void openSessionWithSensor();
