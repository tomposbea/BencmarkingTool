int lcm, gcd;
#define N 10
#define M 10

//lcm.c
void lcm_init(int matrix[N], int* lcm, int* gcd, int column);

//file_io.c
void read_file(int m[M][N], const char *filename, int row, int column);
void write_file(const char *filename);

//print.c
void print_matrix(int a[M][N], int row, int column);
void print_res(int nr, int matrix[N], int gcd, int lcm, int column);

//generate.c
void generate_matrix(int m[M][N],int row, int column);

//read_config.c
void read_matrix_config(const char *filename, int *row, int *column, int *fr);

//conversion.
void convert_array_to_char(int size, int a[size], char b[128]);
void convert_array_to_int(int size, char *c, int res[size]);
void matrix_to_array(int row, int column, int a[M][N], int size, int res[size]);void array_to_matrix(int row, int column, int size, int array[size], int res[M][N]);
