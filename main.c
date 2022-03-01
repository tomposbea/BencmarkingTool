#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>
#include "includes.h"

const char filename[] = "../input.txt";

//matrix
int matrix[M][N];
//int matrix[3][N] = { {72, 120}, {80, 13}, {34, 16}};
//int matrix[M][N] = { {72, 120, 39}, {80, 13, 24}, {34, 16, 85}};
//int matrix[M][N] = { {72, 120, 39, 12}, {80, 13, 24, 18}, {34, 16, 85, 20}};
//int matrix[M][N] = { {72, 120, 39, 12, 5}, {80, 13, 24, 18, 6}, {34, 16, 85, 20, 7}};

// passess arguments to threads
typedef struct {
	int id;
	int line[N];
} thread_args;

void *thread(void *argt){
	
	int thread_nr=* (int*)argt;
	printf("thread %i\n", thread_nr);
	
	int cpu=sched_getcpu();
	printf("core: %i\n", cpu );
	
	lcm_init(matrix[thread_nr], &lcm, &gcd);
	print_res(thread_nr, matrix[thread_nr], gcd, lcm);
	
	//read from struct -> segmentation fault at array
	/*thread_args *actual_args=argt;
	int thread_nr=* (int*)actual_args->id;
	
	int a,b;
	a=* (int *)actual_args->line[0];
	b=* (int *)actual_args->line[1];
	
	//free(actual_args);*/
	printf("\n");
	return NULL;
}

int main(int argc, char **argv) {
 printf("Before\n\n");
 
 //read(matrix,filename);
 generate_matrix(matrix);
 print_matrix(matrix);
 
 pthread_t thread_id;
 int i;
 for(i=0;i<M;i++){
	 //struct passing id+line
	/*thread_args *args = malloc(sizeof *args);
	args->id=i;
	* int j;
    for(j=0;j<N;j++){
	   args->line[j]=matrix[i][j];}
	//printf("%d %d %d\n", args->id, args->line[0], args->line[1]);
	pthread_create(&thread_id, NULL, thread, (void *)&args);*/
	
	//passing just id, global matrix
	pthread_create(&thread_id, NULL, thread, (void *)&i);
	
	int *ptr;
	pthread_join(thread_id, (void**)&ptr);
}
 
 printf("After\n");
 return 0;
}
