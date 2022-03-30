#define MAX_BUF 1024
#define CHAR_BUF 128
#define FIFO 8
#define STRINGLEN 128
#define MAXTABLE 16384
#define ALPHABET 15
#define MAX_THREADS 256
#define THREADS 4

int thread_nr;
int table_size;
struct Transport
{
        int valid;
        char string[STRINGLEN];
};

struct Transport Fifo[MAX_THREADS][FIFO];
int WritePtr[MAX_THREADS];
int ReadPtr[MAX_THREADS];
int FifoBusy[MAX_THREADS];
int FifoLen[MAX_THREADS];

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
int upper, lower;
unsigned long TimerFreq;
unsigned long TimerRef;

int running_time;
int log_frequency;


char output_file_xml[120];
char output_file_csv[120];
