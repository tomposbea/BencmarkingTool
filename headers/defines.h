#define MAX_BUF 1024
#define CHAR_BUF 128
#define FIFO 8
#define STRINGLEN 128
#define MAXTABLE 131072
#define ALPHABET 15
#define MAX_THREADS 64
#define THREADS 4
#define SIZE 660000

char start_time[100];
int outside_build;
int run_counter;

int thread_nr;
int max_thread_param;
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

int found_hash;
int found_table;
int found_tree;
int found_bstree;

int size, row, column, speed;
int upper, lower;
unsigned long TimerFreq;
unsigned long TimerRef;

int running_time;
int log_frequency;

char output_file_xml[120];
char output_file_csv[120];
