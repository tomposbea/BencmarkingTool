#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "includes.h"

int generate_int(int lower, int upper){
	return ((rand() % (upper-lower+1)) + lower);
}

void generate_matrix(int m[M][N]){
	int i,j;
	for(i=0;i<M;i++)
		for(j=0;j<N;j++)
			m[i][j]=generate_int(1,100);
}
