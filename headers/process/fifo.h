// search methods
void* SearchInRBTree2(void* str);
void* SearchInRBTree(void* str);
void *SearchInTable(void *str);
void *SearchInHashTable(void *str);
void *SearchInBSTree(void *str);

// communication methods
int ProcessStringAndCalculate(char* instring, int fifonum);
int GetFromFIFOAndProcess(int fifonum);
int WriteToFifo(int fifonum, char* Temp, int *DroppedCtrl);
int GenerateAndWriteToFIFO(int fifonum);
