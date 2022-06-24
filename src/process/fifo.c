#ifdef __linux__
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <sys/sysinfo.h>
#else
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>      /* Character Type Classification */
#include <inttypes.h>
#include <time.h>
#include <math.h>

#include "../../headers/defines.h"
#include "../../headers/process/lcm.h"
#include "../../headers/process/fifo.h"
#include "../../headers/process/search_duplicate.h"

#include "../../headers/generate/generate.h"
#include "../../headers/generate/conversions.h"

#if defined(__linux__)
#include "../../headers/def_linux.h"
#else
#include "../../headers/def_windows.h"
#endif

// check if instring is unique
// yes: calculate LCM and GCD, return 0
// no: it's a duplicate, increase DuplicateCtrl and return 1
int ProcessStringAndCalculate(char* instring, int fifonum) {
	// we assume that instring is unique
	hash = table = tree = bstree = 0;

	// create threads for different search methods
	HANDLE threadID[4];
	if(1 == fifonum % 4) SearchInTable(instring);
	else if(2 == fifonum % 4) SearchInHashTable(instring);
	else if(3 == fifonum % 4) SearchInBSTree(instring);
	else SearchInRBTree2(instring);

	// if a search method found a duplicate increase the duplicate number
	if(table || hash || tree || bstree) {
                DuplicateCtrl++;
		return 1;
	}

	//calculate LCM, GCD for matrix
	int mess[MAX_ROWS * MAX_COLUMNS];
	int *matrix = malloc(row * column * sizeof(int));
	
	convert_array_to_int(size, instring, mess);
	array_to_matrix(row, column, size, mess, matrix);
	lcm(row, column, matrix);
	
	free(matrix);
	
	return 0;
}

// read from a fifo indicated by fifonum
// return: value returned by ProcessStringAndCalculate
int GetFromFIFOAndProcess(int fifonum) {
	char intext[STRINGLEN];
	// Simple Mutex is enough, because we have only 1 writer and 1 reader.
	if (ReadPtr[fifonum] == WritePtr[fifonum]) return -1;                   // Fifo is empty
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

	// Process string
	return ProcessStringAndCalculate(intext, fifonum);
}

// write to a fifo indicated by fifonum
// Temp - message to be sent, DroppedCtrl - number of duplicates
// return: -1 - fifo is full and data has to be dropped, 0 - write was successfull
int WriteToFifo(int fifonum, char* Temp, int *DroppedCtrl) {
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
		(*DroppedCtrl)++;
		AtomicExhange(&(FifoBusy[fifonum]), 0);
		return -1;
	}
	// FIFO is NOT full.
	int oldwrite = WritePtr[fifonum];
	WritePtr[fifonum] = (WritePtr[fifonum] + 1) % FIFO;

	// copy message to fifo, indicate that fifo has a value and increase size
	strncpy_s(Fifo[fifonum][oldwrite].string, STRINGLEN, Temp, STRINGLEN);
	Fifo[fifonum][oldwrite].valid = 1;
	FifoLen[fifonum]++;

	AtomicExhange(&(FifoBusy[fifonum]), 0);

	return 0;
}

// generate a random matrix and write to fifonum fifo
// return: same as WriteToFifo
int GenerateAndWriteToFIFO(int fifonum) {
	char Temp[STRINGLEN];

	// generate a random matrix (flatten, convert to char array)
	GenerateString(Temp, row, column, size);

	// number of generated matrixes
	GeneratedCtrl++;

	// return 0 if write was successfull, -1 if not
	return WriteToFifo(fifonum, Temp, &DroppedCtrl);
}
