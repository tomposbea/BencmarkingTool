#include <stdio.h>
#include <stdlib.h>
#include "includes.h"

void print_res(int nr, int matrix[N], int gcd, int lcm, int column){
	printf("\nRow %d: ", nr);
	int i;
	for(i=0;i<column;i++)
		printf("%d ", matrix[i]);
	printf("\n");
		
	printf("GCD of row is: %d\n", gcd);
	printf("LCM of row is: %d\n", lcm);
}

void print_matrix(int a[M][N], int row, int column) {
	printf("Input matrix: \n");
	int i,j;
	for(i=0;i<row;i++){
		for(j=0;j<column;j++)
			printf("%d ", a[i][j]);
		printf("\n");
	}
	printf("\n");
}
