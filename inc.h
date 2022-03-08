#define MAX_BUF 1024
#define N 5
#define M 4
#define CHAR_BUF 128

//lcm.c
//int lcm, gcd;
//void lcm_init(int matrix[N], int* lcm, int* gcd, int column);
void lcm(int matrix[M][N], int row, int column);

//read_conf.c
void read_matrix_config(const char *filename, int *n, int *m, int *fr);

//generate.c
int generate_int(int lower, int upper);
void generate_matrix(int m[M][N], int row, int column);

//conversion.c
void convert_array_to_char(int size, int a[size], char b[CHAR_BUF]);
void convert_array_to_int(int size, char *c, int res[size]);
void matrix_to_array(int row, int column, int a[M][N], int size, int res[size]);
void array_to_matrix(int row, int column, int size, int array[size], int res[M][N]);

//print.c
void print_array(int size, int m[size]);
void print_matrix(int a[M][N], int row, int column);
void print_res(int i, int matrix[N], int gcd, int lcm, int column);
