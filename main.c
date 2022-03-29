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
#include <stdbool.h>
#include "headers/defines.h"
#include "headers/functions.h"
#include "headers/def_linux.h"

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
#include "headers/defines.h"
#include "headers/functions.h"
#include "headers/def_windows.h"

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
	//int fifonum=*((int *)data);
	int fifonum=(int)data;
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
			print_matrix_reports();
			printf("\nCounter:%6.6d Gen:%8.8d   Dropped: %8.8d  Processed:%8.8d   Duplicates: %8.8d\nFifo: ", Counter, GeneratedCtrl, DroppedCtrl, ProcessedCtrl, DuplicateCtrl);
			Counter++;
			for (i = 0; i < THREADS; i++) printf("%d:%3.3d  ", i, FifoLen[i]);
			GeneratedCtrl = DroppedCtrl = ProcessedCtrl = DuplicateCtrl = 0;
		
			 if(Counter>=running_time){
                                Enable = 0;
                                Run = 0;
			
				SleepUni(3000); //wait 3s for all processes to finish
        			printf("\n\nStopped...\n");	
				close_matrix_reports();
				exit(0);
                        }

		}
		int sleep = log_frequency * 1000;
		SleepUni(sleep);
	}
	return 0;
}

void InitSubsystem(){
	TableBusy = 0;
	TablePtr = 0;

	DuplicateCtrl = 0;
	GeneratedCtrl = 0;
	ProcessedCtrl = 0;
	DroppedCtrl = 0;
	Counter = 0;
	Run = 1;
	Enable = 0;


	// Init Fifo System.
        for (int i = 0; i < THREADS; i++) {
                for (int j = 0; j < FIFO; j++) Fifo[i][j].valid = 0;
                WritePtr[i] = 0;
                ReadPtr[i] = 0;
                FifoBusy[i] = 0;
                FifoLen[i] = 0;
        }
        // Init table
	table_size  = 16384;
        for (int i = 0; i < table_size; i++) Table[i][0] = 0;

	threads_nr=4;
	running_time = 5;
        log_frequency = 1;

	output_file="../results/MatrixReports.xml";
	init_matrix_reports(output_file);
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

void InitXML(){
	const char filename[] = "../results/MatrixReports.xml";
	init_matrix_reports(filename);
}

void ReadConfig(){
	const char config[] = "../inputs/matrix_conf.cfg";
	read_matrix_config(config, &row, &column, &speed);
        size=row*column;
}

void PrintParams(){
	printf("\nRuntime: %d", running_time);
	printf("\nLog frequency: %d", log_frequency);
	printf("\nThread nr: %d", THREADS);
	printf("\nTable size: %d", table_size);
	printf("\nMatrix size: row-%d, column-%d, size-%d\n", row, column, size);
}

int main(int argc, char** argv) {
	HANDLE threadReport;
	HANDLE threadGen;
	HANDLE threadWorker[THREADS];

	InitSubsystem();
        ReadConfig();
	if(GetParameters(argc, argv)==0) return 0;
	InitTiming();
	PrintParams();

	// Create Threads
	ThreadCreate(threadGen, ThreadGen, 0);
	ThreadCreate(threadReport, ThreadReport, 0);
	for(int i=0; i<THREADS; i++) ThreadCreate((threadWorker[i]), ThreadProcess,i);
	

	if(Counter>running_time) return 0;
	StopProcess();
	close_matrix_reports();
	return 0;
}
