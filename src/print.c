#include <stdio.h>
#include <stdlib.h>
#include "../headers/defines.h"
#include "../headers/functions.h"
void print_res(int nr, int gcd, int lcm, int column, int matrix[column]){
	printf("\nRow %d: ", nr);
	int i;
	for(i=0;i<column;i++)
		printf("%d ", matrix[i]);
	
	printf(", GCD: %d", gcd);
	printf(", LCM: %d", lcm);
}

void print_matrix(int row, int column, int a[row][column]) {
	int i,j;
	for(i=0;i<row;i++){
		for(j=0;j<column;j++)
			printf("%d ", a[i][j]);
		printf("\n");
	}
}

void print_array(int size, int m[size]){
	for(int i=0;i<size;i++)
            printf("%d ",m[i]);
 	printf("\n");
}

void print_report(){
	printf("\nCounter:%6.6d Gen:%8.8d   Dropped: %8.8d  Processed:%8.8d   Duplicates: %8.8d\nFifo: ", Counter, GeneratedCtrl, DroppedCtrl, ProcessedCtrl, DuplicateCtrl);
        for (int i = 0; i < THREADS; i++) printf("%d:%3.3d  ", i, FifoLen[i]);
}
