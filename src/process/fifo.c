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
#include "../../headers/process/binary_search_tree.h"
#include "../../headers/process/redblacktree.h"
#include "../../headers/process/hash_map.h"
#include "../../headers/process/lcm.h"

#include "../../headers/generate/generate.h"
#include "../../headers/generate/conversions.h"

#if defined(__linux__)
#include "../../headers/def_linux.h"
#else
#include "../../headers/def_windows.h"
#endif

int tree, table, hash, bstree;

int insertion(char* data);
void deletion(char* data);

// RB tree by Zsigmond
void* SearchInRBTree2(void* str) {
	if (NULL == str) {
		printf("OOPs. RB Tree zero parameter.\n");
		return NULL;
	}

	int ret = insertion(str);
	if (ret == 0) {
		// Duplicate found/
		found_tree++;
	}
	return NULL;
}

// if matrix not in red and black tree, a new node is created for it
void *SearchInRBTree(void *str){
	char *instring = str;
	if(!search(root_node, instring)) {
                struct node* temp = (struct node*)malloc(sizeof(struct node));
                temp->r = NULL;
                temp->l = NULL;
                temp->p = NULL;
		int len = strlen(instring);
                strncpy(temp->d, instring, len);
                temp->c = 1;
                root_node = insert(root_node, temp);
		fixup(root_node, temp);
        	tree=0;
	} else {
		tree=1;
		found_tree++;
        }
}

// searching for matrix in table, insert if not found
// found_table: number of duplicates in the table
// table: 0 - matrix not found, 1 - matrix found
void *SearchInTable(void *str){
	 //char *instring = str;
	 
	 if (FindDuplicate(str) != -1) { //duplicate
		table=1;
		found_table++;
	 } else { //not found
		 table=0;
	 } 
}

// searching for matrix in hash table, insert if not found
// found_hash: number of duplicates in the hash table
// hash: 0 - matrix not found, 1 - matrix found
void *SearchInHashTable(void *str){
	if(!search_in_hash(make_key(str))) { //not found
		insert_to_hash(make_key(str), str);
		hash=0;
	} else { //duplicate
		hash=1;
		found_hash++;
	}
}

// searching for matrix in binary search tree, insert if not found
// found_bstree: number of duplicates in the hash table
// bstree: 0 - matrix not found, 1 - matrix found
void *SearchInBSTree(void *str) {
	if(search_bs(bsroot, str)){ //found it, duplicate
		bstree = 1;
                found_bstree++;
	} else { //not found
		bsroot = insert_bs(bsroot, str);
                bstree = 0;
	}
}

// search for duplicates using: table, hash table, red black tree, binary search tree
// calculate LCM, GCD for new matrixes
// return: 1 - instring is a duplicate, 0 - instring is processed
int ProcessStringAndCalculate(char* instring, int fifonum) {
	 hash = table = tree = bstree = 0;

	// create threads for different search methods
	/*int search_threads = 3;
	pthread_t threadID[search_threads];
	pthread_create(&threadID[0], NULL, SearchInTable, instring);
	pthread_create(&threadID[1], NULL, SearchInHashTable, instring);
	pthread_create(&threadID[2], NULL, SearchInBSTree, instring);
	//pthread_create(&threadID[3], NULL, SearchInRBTree, instring);
	
	//wait for threads to finish
        for(int i=0;i<search_threads;i++)
                pthread_join(threadID[i], NULL);
	*/

	HANDLE threadID[4];
	if(1 == fifonum % 4) SearchInTable(instring);
	else if(2 == fifonum % 4) SearchInTable(instring);
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
	
	free(matrix);
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
	return ProcessStringAndCalculate(intext, fifonum);
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


