#if defined(__linux__)
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sched.h>
#include <sys/sysinfo.h>
#include <errno.h>
#include <ctype.h>      /* Character Type Classification */
#include <inttypes.h>
#include <time.h>
#include <math.h>
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

#ifdef __aarch64__  // Linux ARM
inline int64_t GetClockValue() {
	int64_t virtual_timer_value;
	asm volatile("mrs %0, cntvct_el0" : "=r"(virtual_timer_value));
	return virtual_timer_value;
}

inline int64_t GetFrequency() {
	int64_t freq;
	asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));
	return freq;
}
#else    // Linux x86
inline u_int64_t GetClockValue() {
	u_int32_t low, high;
	__asm__ volatile("rdtsc" : "=a"(low), "=d"(high));
	return (((u_int64_t)high) << 32) | ((u_int64_t)low);
}


inline int64_t ts_nanosec(const struct timespec* ts)
{
	return (((int64_t)ts->tv_sec) * SI_G + (int64_t)ts->tv_nsec);
}

int64_t GetFrequency() {
	struct timespec ts0, ts1, ts_tmo = { 0, SI_G / 5 };
	u_int64_t hp0, hp1;
	int64_t ns, cycles;

	hp0 = GetClockValue();
	clock_gettime(CLOCK_MONOTONIC, &ts0);
	nanosleep(&ts_tmo, NULL);
	clock_gettime(CLOCK_MONOTONIC, &ts1);
	hp1 = GetClockValue();

	ns = ts_nanosec(&ts1) - ts_nanosec(&ts0);
	cycles = hp1 - hp0;
	//	printf("hp0:%lu, hp1:%lu, ts0:%lu, ts1:%lu, ns:%lu, cycles:%lu\n",hp0, hp1, ts0.tv_nsec, ts1.tv_nsec, ns, cycles);
	return (SI_G * cycles) / ns;
}

#endif

#else 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <Windows.h>
#include <profileapi.h>
#define SleepUni(ms) Sleep(ms)
#define AtomicExhange(var, val) InterlockedExchange(var, val)
#define AtomicLoad(val) InterlockedAdd(val, 0)
#define ThreadCreate(handle, funct, param) handle=CreateThread(NULL, 0, funct, (void*)param, 0, NULL);
#define THREADTYPE DWORD WINAPI

unsigned long GetFrequency() {
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	return (unsigned long) freq.QuadPart;
}

unsigned long GetClockValue() {
	LARGE_INTEGER val;
	QueryPerformanceCounter(&val);
	return (unsigned long) val.QuadPart;
}

#endif

#define THREADS 4
#define FIFO 8
#define STRINGLEN 128
#define MAXTABLE 16384
#define ALPHABET 15


struct Transport
{
	int valid;
	char string[STRINGLEN];
};

struct Transport Fifo[THREADS][FIFO];
int WritePtr[THREADS];
int ReadPtr[THREADS];
int FifoBusy[THREADS];
int FifoLen[THREADS];
int TableBusy = 0;

char Table[MAXTABLE][STRINGLEN];
int TablePtr = 0;

int DuplicateCtrl = 0;
int GeneratedCtrl = 0;
int ProcessedCtrl = 0;
int DroppedCtrl = 0;
int Counter = 0;
int Run = 1;
int Enable = 0;

int size, row, column, speed;

unsigned long TimerFreq;
unsigned long TimerRef;

int CpuAllocTable[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20 };
//                      G,P,P,P,P,P,P,P,P,P,P,P....

void SetProcessAffinity(int cpu)
{
#if defined(__linux__)
	cpu_set_t  mask;
	CPU_ZERO(&mask);
	CPU_SET(cpu, &mask);
	pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask);	
#else
	SetThreadAffinityMask(GetCurrentThread(), 1 << cpu );
#endif
}

int FindDuplicate(char* instring) {
	int i;
	for (i = 0; i < MAXTABLE; i++) if (!strcmp(Table[i], instring)) return i;  // Found.
	return -1;  // Not Found
}

int ProcessString(char* instring) {
	if (FindDuplicate(instring) != -1) {
		// Duplicate found, step counter. 
	    // Step counters.
		ProcessedCtrl++;
		DuplicateCtrl++;
		return 1; // Found, can skip it.
	}

	//calculate LCM, GCD for matrix

        //char array->int array->matrix
        int mess[size];
        int matrix[row][column];
        convert_array_to_int(size, instring, mess);
        array_to_matrix(row, column, size, mess,matrix);

        lcm(row, column, matrix);

	// Critical section
	int oldbusy = 1;            // Always check busy flag
	while (oldbusy) {          
		oldbusy = AtomicExhange(&TableBusy, 1); // Read TableBusy AND write 1 to it.
		if (oldbusy) {          // Check if ours  
			// No. Someone is already in the loop.
			while (AtomicLoad(&TableBusy)) SleepUni(0); // Table is Busy. Wait.
			oldbusy = 1;        // Table is free, but there is a race for it. Try to catch again.
		}
	}

	// Write to table
	strncpy_s(Table[TablePtr], STRINGLEN, instring, STRINGLEN);
	// Set pointer
	TablePtr = (TablePtr + 1) % MAXTABLE;

	// Step counter.
	ProcessedCtrl++;
	// Release table
	AtomicExhange(&TableBusy, 0);
	return 0;
}

int GetFromFIFOAndProcess(int fifonum) {
	char intext[STRINGLEN];
	// Simple Mutex is enough, because we have only 1 writer and 1 reader.
	if (ReadPtr[fifonum] == WritePtr[fifonum]) return -1;			// Fifo is empty
	// Not empty, read it.
	int oldbusy = 1;
	while (oldbusy)
	{
		oldbusy = AtomicExhange(&(FifoBusy[fifonum]), 1); // Read Busy Flag
		if (oldbusy) {    // Check if ours  
			while (AtomicLoad(&(FifoBusy[fifonum]))) SleepUni(0); //  FIFO is busy
			oldbusy = 1;  // Try again
		}
	}
	// Push data to the fifo
	struct Transport* income = Fifo[fifonum] + ReadPtr[fifonum];
	// Process.
	if (income->valid) {
		// Save data to the stack
		strncpy_s(intext, STRINGLEN, income->string, STRINGLEN);
	}
	// Release the FIFO
	income->valid = 0;
	ReadPtr[fifonum] = (ReadPtr[fifonum] + 1) % FIFO;
	FifoLen[fifonum]--;

	AtomicExhange(&(FifoBusy[fifonum]), 0);
	// Process
	return ProcessString(intext);
}

int WriteToFifo(int fifonum, char* Temp) {
	int oldbusy = 1;
	while (oldbusy)
	{
		oldbusy = AtomicExhange(&(FifoBusy[fifonum]), 1); // Read Busy Flag
		if (oldbusy) {    // Check if ours  
			while (AtomicLoad(&(FifoBusy[fifonum]))) SleepUni(0); //  FIFO is busy
			oldbusy = 1;  // Try again
		}
	}

	if (((WritePtr[fifonum] + 1) % FIFO) == ReadPtr[fifonum]) {
		// FIFO is full. Drop data.
		DroppedCtrl++;
		AtomicExhange(&(FifoBusy[fifonum]), 0);
		return -1;
	}
	// FIFO is NOT full.
	int oldwrite = WritePtr[fifonum];
	WritePtr[fifonum] = (WritePtr[fifonum] + 1) % FIFO;

	strncpy_s(Fifo[fifonum][oldwrite].string, STRINGLEN, Temp, STRINGLEN);
	Fifo[fifonum][oldwrite].valid = 1;
	FifoLen[fifonum]++;

	AtomicExhange(&(FifoBusy[fifonum]), 0);

	return 0;
}

int GenerateAndWriteToFIFO(int fifonum) {
	char Temp[STRINGLEN];
    	GenerateString(Temp, row, column, size);
	GeneratedCtrl++;
	return WriteToFifo(fifonum, Temp);
}

THREADTYPE ThreadGen(void* data) {
	int roundrobin = 0;
	SetProcessAffinity(CpuAllocTable[0]);
	while (Run) {
		if (Enable) {
			GenerateAndWriteToFIFO(roundrobin);
			roundrobin++;
			if (roundrobin >= THREADS) roundrobin = 0;
			SleepUni(speed);
		}
		else SleepUni(0);
	}
	return 0;
}

THREADTYPE ThreadProcess(void* data) {
	int fifonum = (int)data;
	SetProcessAffinity(CpuAllocTable[1+fifonum]);
	while (Run) {
		if (Enable) {
			if (GetFromFIFOAndProcess(fifonum)) SleepUni(0);
		}
		else SleepUni(0);
	}
	return 0;
}

THREADTYPE ThreadReport(void* data) {
	int i;
	while (Run) {
		if (Enable)
		{
			printf("\nCounter:%6.6d Gen:%8.8d   Dropped: %8.8d  Processed:%8.8d   Duplicates: %8.8d\nFifo: ", Counter++, GeneratedCtrl, DroppedCtrl, ProcessedCtrl, DuplicateCtrl);
			for (i = 0; i < THREADS; i++) printf("%d:%3.3d  ", i, FifoLen[i]);
			GeneratedCtrl = DroppedCtrl = ProcessedCtrl = DuplicateCtrl = 0;
		}

		SleepUni(1000);
	}
	return 0;
}

void InitSubsystem(){
	printf("\nInit subsystem...\n");
	// Init Fifo System.
        for (int i = 0; i < THREADS; i++) {
                for (int j = 0; j < FIFO; j++) Fifo[i][j].valid = 0;
                WritePtr[i] = 0;
                ReadPtr[i] = 0;
                FifoBusy[i] = 0;
                FifoLen[i] = 0;
        }
        // Init table
        for (int i = 0; i < MAXTABLE; i++) Table[i][0] = 0;
}

void InitTiming(){
        SetProcessAffinity(0);
        printf("\nHigh-speed timers...\n");
        TimerFreq = GetFrequency();
        printf("Reference Freq: %ld MHz\n", TimerFreq / 1000000);
        TimerRef = GetClockValue();
        SleepUni(1000);
        printf("Reference Time Measurement for 1 sec: %ld ms\n", (((GetClockValue() - TimerRef) * 1000) / TimerFreq));
}

void StopProcess(){
	SleepUni(1);
        printf("\nRun...\n");
        Enable = 1;
        
	// Wait for enter
        getchar();
        
	// Destroy.
        Enable = 0;
        Run = 0;
        SleepUni(1);
        printf("\n\nStopped...\n");

}

void ReadConfig(){
	const char config[] = "matrix_conf.cfg";
	read_matrix_config(config, &row, &column, &speed);
	printf("Matrix config: speed=%d, N(column)=%d, M(row)=%d\n", speed, column, row);
        size=row*column;
}

int main(int argc, char** argv) {
	HANDLE threadReport;
	HANDLE threadGen;
	HANDLE threadWorker[THREADS];

	InitSubsystem();
	ReadConfig();
	InitTiming();
	
	// Create Threads
	ThreadCreate(threadGen, ThreadGen, 0);
	ThreadCreate(threadReport, ThreadReport, 0);
	for(int i=0; i<THREADS; i++) ThreadCreate((threadWorker[i]), ThreadProcess, i);
	
	StopProcess();
	return 0;
}



