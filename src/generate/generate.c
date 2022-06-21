// Generate random number, matrix, array, check if array is unique
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include "../../headers/defines.h"
#include "../../headers/generate/generate.h"

// generate a random int between a lower and upper value
int generate_int(int lowerl, int upperl){
	return ((rand() % (upperl-lowerl+1)) + lowerl);
}

// generate a random matrix with given row and column numbers
void generate_matrix(int row, int column, int m[row][column]){
	int i,j;
	for(i=0;i<row;i++)
		for(j=0;j<column;j++)
			m[i][j]=generate_int(lower, upper);
}

// generate a random string: generate matrix, flatten it and convert to char array
int GenerateString(char * char_array, int row, int column, int size) {
        int array[size], matrix[row][column];
	//usleep(100000);
        generate_matrix(row, column, matrix);
        matrix_to_array(row, column, matrix,size,array);
        convert_array_to_char(size,array,char_array);

        return sizeof(char_array);
}

// find duplicate of a string in a table
int FindDuplicate(char* instring) {
        int i;
        for (i = 0; i < MAXTABLE; i++) 
		if (!strcmp(Table[i], instring)) 
			return i;  // Found.
        
	return -1;  // Not Found
}
