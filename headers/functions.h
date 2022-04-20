
//lcm.c
//int lcm, gcd;
//void lcm_init(int matrix[N], int* lcm, int* gcd, int column);
int GCD(int a, int b);
int LCM(int a, int b);
void lcm(int row, int column, int matrix[row][column]);

//read_conf.c
void read_matrix_config(const char *filename, int *n, int *m, int *fr);

//generate.c
int generate_int(int lower, int upper);
void generate_matrix(int row, int column, int m[row][column]);
int GenerateString(char * char_array, int row, int column, int size);
int FindDuplicate(char* instring);

//conversion.c
void convert_array_to_char(int size, int a[size], char b[CHAR_BUF]);
void convert_array_to_int(int size, char *c, int res[size]);
void matrix_to_array(int row, int column, int a[row][column], int size, int res[size]);
void array_to_matrix(int row, int column, int size, int array[size], int res[row][column]);

//print.c
void print_array(int size, int m[size]);
void print_matrix(int row, int column, int a[row][column]);
void print_res(int i, int gcd, int lcm, int column, int matrix[column]);
void  print_report();
void PrintParams();

//fifo.c
//int WriteToFifo(int fifonum, char* Temp, int *DroppedCtrl);
int GetFromFIFOAndProcess(int fifonum);
int ProcessString(char* instring);
int GenerateAndWriteToFIFO(int fifonum);

//write_xml.c
void init_xml(const char *filename);
void print_to_xml();
void close_xml();

//write_csv.c
void init_csv(const char *filename);
void print_to_csv();
void close_csv();

//error.c
void error_no_value(char* mess);
void error_not_int(char* mess);
void error_over_limit(char* mess, int limit);
void error_under_limit(char* mess, int limit);
void error_lower_upper();
void error_wrong_parameter(char* mess);

//usage.c
void PrintUsage();
int CheckParameter(int i, int len, char** argv, char* option);
int GetParameters(int argc, char** argv);

//init.c
void StopProcess();
int InitSubsystem(int argc, char** argv);
void ReadConfig();

//system_data.c
void do_popen(char command[100], char *res);
void get_system_data();

//redblacktree.c
struct node* insert(struct node* trav, struct node * temp);
void fixup(struct node* root, struct node* pt);
void left_rotate(struct node* temp);
void right_rotate(struct node* temp);
int search(struct node* trav, char* instring);

//hash_map.c
struct DataItem *search_in_hash(long int key);
void insert_to_hash(long int key,char* data);
struct DataItem* delete_from_hash(struct DataItem* item);
void display_hash();
long int make_key(char* val);
void empty_hash();
