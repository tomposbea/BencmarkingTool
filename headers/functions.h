
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

//fifo.c
//int WriteToFifo(int fifonum, char* Temp, int *DroppedCtrl);
int GetFromFIFOAndProcess(int fifonum);
int ProcessString(char* instring);
int GenerateAndWriteToFIFO(int fifonum);

//write_xml.c
void init_matrix_reports(const char *filename);
void print_matrix_reports();
void close_matrix_reports();

//error.c
void error_no_value(char* mess);
void error_not_int(char* mess);
void error_over_limit(char* mess, int limit);

//usage.c
void PrintUsage();
int CheckParameter(int i, int len, char** argv, char* option);
int GetParameters(int argc, char** argv);
