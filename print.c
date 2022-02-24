#include <stdio.h>
#include <stdlib.h>
#include "includes.h"

void print_res(int nr, int matrix[N], int gcd, int lcm){
	printf("\nRow %d: ", nr);
	int i;
	for(i=0;i<N;i++)
		printf("%d ", matrix[i]);
	printf("\n");
		
	printf("GCD of row is: %d\n", gcd);
	printf("LCM of row is: %d\n", lcm);
}

void print_matrix(int a[M][N]) {
	printf("Input matrix: \n");
	int i,j;
	for(i=0;i<M;i++){
		for(j=0;j<N;j++)
			printf("%d ", a[i][j]);
		printf("\n");
	}
	printf("\n");
}
