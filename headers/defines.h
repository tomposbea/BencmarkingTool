#define MAX_BUF 1024
#define CHAR_BUF 128
#define FIFO 8
#define STRINGLEN 128
#define MAXTABLE 131072
#define ALPHABET 15
#define MAX_THREADS 20
#define THREADS 4
#define SIZE 660000
#define MAX_COLUMNS 32
#define MAX_ROWS 32

//char start_time[100];

// check if tool is started from build folder, change paths if not
int outside_build;

// count for how many seconds the tool has been running
int run_counter;

// after how many seconds should the tool delete old tree nodes (BST, RB)
int tree_limit;

// threads: parameters and defined max
int thread_nr;
int max_thread_param;

// lookup table
int table_size;
char Table[MAXTABLE][STRINGLEN];
int TableBusy;
int TablePtr;

// fifo struct for communication between generate and report
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

// results
int DuplicateCtrl; //number of duplicates
int GeneratedCtrl; //number of generated
int ProcessedCtrl; //number of processed: generated - (duplicate + dropped)
int DroppedCtrl; //number of dropped
int Counter; //seconds since start
int Counter2; //counter for deleting BST nodes
int CounterStop; //counter after tool stabilizes
int MemoryDiff; //memory differenve stibilizwed
int Run; //true/false -> stopping criteria
int Enable; //true/false

// average of results
long long int avg_usage, avg_usage2, avg_generated, avg_dropped, avg_processed, avg_duplicate, avg_hash, avg_table, avg_tree, avg_bstree;

// number of duplicates found by different search methods
int found_hash; 
int found_table;
int found_tree;
int found_bstree;

// parameters: matrix size, values, generating speed
int size, row, column, speed;
int upper, lower;

// count number of duplicates
int tree, table, hash, bstree;

// timer parameters
unsigned long TimerFreq;
unsigned long TimerRef;

// runtime, frequency of reporting
int running_time;
int log_frequency;

// xml and csv output file name
char output_file_xml[120];
char output_file_csv[120];
