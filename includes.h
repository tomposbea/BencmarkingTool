int lcm, gcd;
#define N 5
#define M 3

//lcm.c
void lcm_init(int matrix[N], int* lcm, int* gcd);

//file_io.c
void read(int m[M][N], const char *filename);
void write(const char *filename);

//print.c
void print_matrix(int a[M][N]);
void print_res(int nr, int matrix[N], int gcd, int lcm);

//generate.c
void generate_matrix(int m[M][N]);
