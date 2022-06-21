//fifo.c
//int WriteToFifo(int fifonum, char* Temp, int *DroppedCtrl);
int GetFromFIFOAndProcess(int fifonum);
int ProcessString(char* instring);
int GenerateAndWriteToFIFO(int fifonum);
