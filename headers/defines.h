#define MAX_BUF 1024
#define CHAR_BUF 128
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
char Table[MAXTABLE][STRINGLEN];

int TableBusy;
int TablePtr;

int DuplicateCtrl;
int GeneratedCtrl;
int ProcessedCtrl;
int DroppedCtrl;
int Counter;
int Run;
int Enable;

int size, row, column, speed;

unsigned long TimerFreq;
unsigned long TimerRef;

