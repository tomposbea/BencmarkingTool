#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <poll.h>
#include <sys/stat.h>
#include <sched.h>

#include "inc.h"

const char filename[] = "../input.txt";
const char config[] = "../matrix_conf.cfg";
char * myfifo = "/tmp/myfifo";
int size, row, column, speed;
int nr_matrixes_generated;
int nr_matrixes_lcm;
int nr_matrixes_deleted;
int running_time;

void matrix_config(){
    read_matrix_config(config, &row, &column, &speed);
    printf("Matrix config: speed=%d, N(column)=%d, M(row)=%d\n", speed, column, row);
    size=row*column;
}

// generates int matrix, converts into char array, writes in pipe
void *worker(void *argt){
    int cpu=sched_getcpu();
    printf("matrix generator cpu core: %i\n", cpu );

    int array[size], a[row][column];
    char b[CHAR_BUF];	

    int fd;
    mkfifo(myfifo, 0666);
    fd=open(myfifo, O_WRONLY);
	
    while(1) {
	generate_matrix(row, column, a);
	nr_matrixes_generated++;
	
	matrix_to_array(row, column, a,size,array);	
      	convert_array_to_char(size,array,b);

	write(fd, b, sizeof(b));
	long long int sleep=speed*10000;
	usleep(sleep); //microseconds (1sec = 6zeros)
    }

    close(fd);
    unlink(myfifo);
}


// reads char array from pipe, converts into int matrix
void *slave(void *argt){
    int cpu=sched_getcpu();
    printf("operations cpu core: %i\n", cpu );
	
    char buf[MAX_BUF];
    int mess[size];
    int matrix[row][column];

    int fd;
    fd = open(myfifo, O_RDONLY);

    while(1){
        read(fd, buf, MAX_BUF);
        convert_array_to_int(size, buf, mess);
	array_to_matrix(row, column, size, mess,matrix);
	
	lcm(row, column, matrix);
	nr_matrixes_lcm++;
    }

    close(fd);
}

void *log_statistics(void *argt){
    int cpu=sched_getcpu();
    printf("logging cpu core: %i\n", cpu );
    
    while(1){
        usleep(1000000);
	running_time++;
	nr_matrixes_deleted=nr_matrixes_generated-nr_matrixes_lcm;
        printf("%dsec, generated: %d, calculated on: %d, lost: %d\n", 
		running_time, nr_matrixes_generated, nr_matrixes_lcm, nr_matrixes_deleted);
    }
}

// create writes and reader thread
void create_threads() {
        pthread_t thread_id;
        int i;

	// thread 0: generate random matrixes, send them to thread 1
	i=0;
        pthread_create(&thread_id, NULL, worker, (void *)&i);

	// thread 1: read matrixes from fifo pipe, do something with them
	i=1;
	pthread_create(&thread_id, NULL, slave, (void *)&i);

	//thread 2: logging statistics
	i=2;
	pthread_create(&thread_id, NULL, log_statistics, (void *)&i);
	
	int *ptr;
        pthread_join(thread_id, (void**)&ptr);
}

void init(){
    nr_matrixes_generated=0;
    nr_matrixes_lcm=0;
    nr_matrixes_deleted=0;
    running_time=0;
}

int main()
{
    init();
    matrix_config();
    create_threads(); 
    return 0;
}
