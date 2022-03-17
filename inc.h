#define MAX_BUF 1024
#define CHAR_BUF 128
#define THREADS 4
#define FIFO 8
#define STRINGLEN 128
#define MAXTABLE 16384
#define ALPHABET 15


//lcm.c
//int lcm, gcd;
//void lcm_init(int matrix[N], int* lcm, int* gcd, int column);
void lcm(int row, int column, int matrix[row][column]);

//read_conf.c
void read_matrix_config(const char *filename, int *n, int *m, int *fr);

//generate.c
int generate_int(int lower, int upper);
void generate_matrix(int row, int column, int m[row][column]);
int GenerateString(char * char_array, int row, int column, int size);

//conversion.c
void convert_array_to_char(int size, int a[size], char b[CHAR_BUF]);
void convert_array_to_int(int size, char *c, int res[size]);
void matrix_to_array(int row, int column, int a[row][column], int size, int res[size]);
void array_to_matrix(int row, int column, int size, int array[size], int res[row][column]);

//print.c
void print_array(int size, int m[size]);
void print_matrix(int row, int column, int a[row][column]);
void print_res(int i, int gcd, int lcm, int column, int matrix[column]);
