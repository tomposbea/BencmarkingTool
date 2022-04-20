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
#include "../headers/defines.h"
#include "../headers/functions.h"

#if defined(__linux__)
#include "../headers/def_linux.h"
#else
#include "../headers/def_windows.h"
#endif

int found_tree, found_table, found_hash;

// if matrix not in red and black tree, a new node is created for it
void *SearchInRBTree(void *str){
	char *instring = str;
	if(!search(root_node, instring)) {
                struct node* temp = (struct node*)malloc(sizeof(struct node));
                temp->r = NULL;
                temp->l = NULL;
                temp->p = NULL;
                strncpy(temp->d, instring, strlen(instring));
                temp->c = 1;

                root_node = insert(root_node, temp);
                fixup(root_node, temp);
        	found_tree=0;
	} else {
                ProcessedCtrl++;
                DuplicateCtrl++;
		found_tree=1;
        }
	//found_tree=0;
}

// searching for matrix in table, insert if not found
void *SearchInTable(void *str){
	 char *instring = str;
	 if (FindDuplicate(instring) != -1) {
                ProcessedCtrl++;
                DuplicateCtrl++;
		found_table=1;
	 } else found_table=0;
}

void *SearchInHashTable(void *str){
	if(!search_in_hash(make_key(str))) {
		insert_to_hash(make_key(str), str);
		found_table=0;
	} else found_table=1;
}

int ProcessStringAndCalculate(char* instring) {
	// create threads fo different search methods, wait for them to finish
	pthread_t threadID;
	pthread_create(&threadID, NULL, SearchInRBTree, instring);
	pthread_create(&threadID, NULL, SearchInTable, instring);
	pthread_create(&threadID, NULL, SearchInHashTable, instring);
	pthread_join(threadID, NULL);

	if(found_table || found_tree) return 1;

	//calculate LCM, GCD for matrix
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
	TablePtr = (TablePtr + 1) % table_size;

	// Step counter.
	ProcessedCtrl++;
	// Release table
	AtomicExhange(&TableBusy, 0);
	return 0;
}

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
	return ProcessStringAndCalculate(intext);
}

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
	return WriteToFifo(fifonum, Temp, &DroppedCtrl);
}


