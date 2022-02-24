#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes.h"

void read(int m[M][N], const char *filename){
	FILE *fptr;
	if((fptr = fopen(filename,"r"))==NULL) {
		printf("Error opening file\n");
		exit(1);
	}
	
	int i, j;
	//int m[M][N];
	for(i=0;i<M;i++)
		for(j=0;j<N;j++)
			fscanf(fptr, "%d", &m[i][j]);
	
	fclose(fptr);
}

void write(const char *filename){
}
