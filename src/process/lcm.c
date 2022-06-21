// calculate LCM and GCD for a matrix, a line and two values

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/defines.h"
#include "../../headers/process/lcm.h"

// calculates GCD(greatest common divisor)
int GCD(int a, int b) {
	int i, gcd;

	// while i smaller than the two numbers
	for(i=1; i<=a && i<=b; ++i) {
		// i is the gcd if it can divide one of the numbers
		if(a%i==0 && b%i==0) 
			gcd=i;
	}

	return gcd;
}

// calculates LCM(lowest common multiple)
int LCM(int a, int b) {
	int lcm;

	// lcm is the biggest number
	lcm=(a>b) ? a:b;

	while(1){
		// if lcm can be divided by one of the parameters, than it is lcm
		if(lcm%a==0 && lcm%b==0) 
			return lcm;
		lcm++;
	}

	return lcm;
}

//calculates lcm, gcd for one column
void lcm_init(int* lcm, int* gcd, int column, int matrix[column]) {
	struct Results res[column]; //stores all result (lcm, gcd) pairs
	int i, counter; // iteration helpers
	int a,b,c,d; // get matrix values
	int len; //column lenght
	
	//if column has odd nr of elements, last one has to be saved separatly to Results struct
	if(column%2==0) 
		len=column;
	else 
		len=column-1; 
	
	//calculates results for pairs of the matrix, saves in struct Results
	for(i=0,counter=0;i<len;i+=2,counter++) { 
		// get two matrix values
		a=matrix[i];
		b=matrix[i+1];
		
		// save lcm and gcd of the two values into the Results structure
		res[counter].res_lcm=LCM(a,b);
		res[counter].res_gcd=GCD(a,b);
	}

	// for odd number of element columns: last element os just saved in the Reseults structure
	if(column%2>0) {
		res[counter].res_lcm=matrix[i];
		res[counter].res_gcd=matrix[i];
		counter++;
	}
	
	//we are saving in the same struct, len is after the last written position
	len=counter; 
	
	//we are calculating until there are no more pairs, only one result
	int repetition=len; 
	
	while(repetition!=1) {
		//calculates results for pairs of the struct to get final lcm and gcd of a line
		for(i=0;i<len-1;i++,counter++) { 
			a=res[i].res_lcm;
			b=res[i+1].res_lcm;
			res[counter].res_lcm=LCM(a,b);
			
			c=res[i].res_gcd;
			d=res[i+1].res_gcd;
			res[counter].res_gcd=GCD(c,d);
		}
		len=counter;
		repetition/=2;
	}
	*lcm=res[counter-1].res_lcm;
	*gcd=res[counter-1].res_gcd;
}

// calculate lcm and gcd for a matrix line by line
void lcm(int row, int column, int matrix[row][column]){
	int lcm, gcd;
	for(int i=0;i<row;i++){
		lcm_init(&lcm, &gcd, column, matrix[i]);
	}
}
