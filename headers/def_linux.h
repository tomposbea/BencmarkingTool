#define SleepUni(ms) usleep(ms*1000)
#define AtomicExhange(var, val) __atomic_exchange_n (var, val, __ATOMIC_RELEASE)
#define AtomicLoad(val) __atomic_load_n(val, __ATOMIC_ACQUIRE)
#define strncpy_s(dest,num1,src,num2) strncpy(dest, src, num2)
#define ThreadCreate(handle, funct, param) pthread_create(&handle, NULL, funct, (void*)param)
#define THREADTYPE void*
#define HANDLE pthread_t
#define sprintf_s(a,b,c,d) sprintf(a,c,d)
#define SI_k 1000
#define SI_M (SI_k * SI_k)
#define SI_G (SI_k * SI_k * SI_k)

