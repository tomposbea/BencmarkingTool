#define SleepUni(ms) Sleep(ms)
#define AtomicExhange(var, val) InterlockedExchange(var, val)
#define AtomicLoad(val) InterlockedAdd(val, 0)
#define ThreadCreate(handle, funct, param) handle=CreateThread(NULL, 0, funct, (void*)param, 0, NULL);
#define THREADTYPE DWORD WINAPI

