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

int ProcessStringAndCalculate(char* instring) {
        if (FindDuplicate(instring) != -1) {
                // Duplicate found, step counter.
            // Step counters.
                ProcessedCtrl++;
                DuplicateCtrl++;
                return 1; // Found, can skip it.
        }

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
        TablePtr = (TablePtr + 1) % TABLE;

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


