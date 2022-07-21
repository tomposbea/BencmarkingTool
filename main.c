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
#include "headers/process/binary_search_tree.h"
#include "headers/process/redblacktree.h"
#include "headers/process/hash_map.h"
#include "headers/process/fifo.h"
#include "headers/report/write_xml.h"
#include "headers/report/write_csv.h"
#include "headers/report/print.h"
#include "headers/report/system_data.h"
#include "headers/init/init.h"

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
			if (roundrobin >= thread_nr) roundrobin = 0;
			//SleepUni(speed);
		}
		else SleepUni(0);
	}
	return 0;
}

THREADTYPE ThreadProcess(void* data) {
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

// print to terminal, write to output files
void PrintOneLine() {
	 print_to_xml();
         print_to_csv();
         print_report();
}

// increase counter, restart performance metrics
void IncreaseCounter() {
	 // increase run counter
         Counter++;

	 // restart counting from last log
         GeneratedCtrl = DroppedCtrl = ProcessedCtrl = DuplicateCtrl = found_tree = found_hash = found_table = found_bstree = 0;

         // empty search trees, hash
         delete_tree(root_node);
         root_node=NULL;
         bsroot=NULL;
         void empty_hash();

	 // save memory usage
	 strcpy(old_usage2, usage2);
}

// stop the current run of the tool, close output files
void StopTool() {
	// stop execution
	Enable = 0;
        Run = 0;

	//wait for all processes to finish
        SleepUni(500);
        
	printf("\n\nSTOPPED\n--------------------------------------------------\n");

	// close output files
        close_xml();
        close_csv();

	// stop the tool
        exit(0);
}

THREADTYPE ThreadReport(void* data) {
	int i;
	while (Run) {
		if (Enable)
		{
			// get platform parameters, memory usage: usage2
			get_system_data();

			// first run, no previous value to compare to
			if(Counter == 0){
				printf("Counter: %d, old_mem: %ld, mem: %ld\n", Counter, atol(usage2), atol(usage2));
				IncreaseCounter();
			
			// later runs, old memory usage is saved
			} else {
				long int old_mem = atol(old_usage2);
				long int mem = atol(usage2);

				printf("Counter: %d, old_mem: %ld, mem: %ld\n", Counter, old_mem, mem);
				
				// increase counter if difference between last memory usages is more than delta
				// write last line to output files+stop tool if difference is smaller than delta
				if(old_mem > mem) {
					if((old_mem - mem) > mem_delta) {
						IncreaseCounter();
					} else {
						PrintOneLine();
						StopTool();
					}

				} else {
					if((mem - old_mem) > mem_delta) {
						IncreaseCounter();
                                        } else {
						PrintOneLine();
						StopTool();
					}
				}
			}
		}

		// sleep until next log
		int sleep = log_frequency * 1000;
		SleepUni(sleep);
	}
	return 0;
}

void InitTiming(){
        SetProcessAffinity(0);
        TimerFreq = GetFrequency();
        //printf("Reference Freq: %ld MHz\n", TimerFreq / 1000000);
        TimerRef = GetClockValue();
        //SleepUni(500);
        //printf("Reference Time Measurement for 1 sec: %ld ms\n", (((GetClockValue() - TimerRef) * 1000) / TimerFreq));
}

int main(int argc, char** argv) {
	HANDLE threadReport;
	HANDLE threadGen;
	HANDLE threadWorker[thread_nr];

	if(!InitSubsystem(argc, argv)) return 0;
	InitTiming();
	PrintParams();

	// Create Threads
	ThreadCreate(threadGen, ThreadGen, 0);
	ThreadCreate(threadReport, ThreadReport, 0);
	for(int i=0; i<thread_nr; i++) ThreadCreate((threadWorker[i]), ThreadProcess,i);
	
	if(Counter>running_time) return 0;
	StopProcess();

	return 0;
}
