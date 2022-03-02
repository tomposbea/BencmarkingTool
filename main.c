#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "includes.h"

const char filename[] = "../input.txt";
const char config[] = "../matrix_conf.cfg";
int matrix[M][N];
int column, row, speed;

volatile sig_atomic_t stop;

void *thread(void *argt){
	int thread_nr=* (int*)argt;
	printf("thread %i\n", thread_nr);
	
	int cpu=sched_getcpu();
	printf("core: %i\n", cpu );
	
	lcm_init(matrix[thread_nr], &lcm, &gcd, column);
	print_res(thread_nr, matrix[thread_nr], gcd, lcm, column);
	
	printf("\n");
	return NULL;
}

void stop_loop(int signum){
	stop=1;
}

// creates row number threads
void create_threads() {
	pthread_t thread_id;
        int i;
        for(i=0;i<row;i++){
               pthread_create(&thread_id, NULL, thread, (void *)&i);
               int *ptr;
               pthread_join(thread_id, (void**)&ptr);
        }
}

int main(int argc, char **argv) {
 	printf("START\n\n");
 
	read_matrix_config(config, &row, &column, &speed);//read config file: matrix size, how often new matrix should be generated
	printf("Matrix config: speed=%d, N(column)=%d, M(row)=%d\n\n", speed, column, row);
 	
	generate_matrix(matrix, row, column); // generate a random matrix
        print_matrix(matrix, row, column);

	create_threads();
	
 	//infinite loop, stop at ctrl+C
 	stop=0;
 	signal(SIGINT, stop_loop);
 	while(!stop){
		generate_matrix(matrix, row, column); // generate a random matrix
        	print_matrix(matrix, row, column);
	 	sleep(speed);
	}

 	printf("STOP\n");
 	return 0;
}
