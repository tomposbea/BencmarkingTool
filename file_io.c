#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes.h"

void read_file(int m[M][N], const char *filename, int row, int column){
	FILE *fptr;
	if((fptr = fopen(filename,"r"))==NULL) {
		printf("Error opening file\n");
		exit(1);
	}
	
	int i, j;
	//int m[M][N];
	for(i=0;i<row;i++)
		for(j=0;j<column;j++)
			fscanf(fptr, "%d", &m[i][j]);
	
	fclose(fptr);
}

void write_file(const char *filename){
}
