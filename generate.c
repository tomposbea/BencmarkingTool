#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "inc.h"

int generate_int(int lower, int upper){
	return ((rand() % (upper-lower+1)) + lower);
}

void generate_matrix(int row, int column, int m[row][column]){
	int i,j;
	for(i=0;i<row;i++)
		for(j=0;j<column;j++)
			m[i][j]=generate_int(1,99);
}

int GenerateString(char * char_array, int row, int column, int size) {
        int array[size], matrix[row][column];

        generate_matrix(row, column, matrix);
        matrix_to_array(row, column, matrix,size,array);
        convert_array_to_char(size,array,char_array);

        return sizeof(char_array);
}

