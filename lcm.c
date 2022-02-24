#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes.h"

struct Results{
	int res_lcm;
	int res_gcd;
};

// calculates GCD(greatest common divisor)
int GCD(int a, int b) {
	int i, gcd;
	for(i=1; i<=a && i<=b; ++i) {
		if(a%i==0 && b%i==0) gcd=i;
	}
	return gcd;
}

// calculates LCM(lowest common multiple)
int LCM(int a, int b) {
	int lcm;
	lcm=(a>b) ? a:b;
	while(1){
		if(lcm%a==0 && lcm%b==0) return lcm;
		lcm++;
	}
	return lcm;
}

//calculates lcm, gcd to the row
void lcm_init(int matrix[N], int* lcm, int* gcd) {
	struct Results res[N]; //stores all result (lcm, gcd) pairs
	int i, counter;
	int a,b,c,d;
	int len;
	
	if(N%2==0) len=N;
	else len=N-1; //if N is odd, last one has to be saved separatly to Results struct
	
	for(i=0,counter=0;i<len;i+=2,counter++) { //calculates results for pairs of the matrix, saves in struct Results
		a=matrix[i];
		b=matrix[i+1];
		
		res[counter].res_lcm=LCM(a,b);
		res[counter].res_gcd=GCD(a,b);
		
		printf("%d --- (%d and %d) lcm: %d, gcd: %d\n", counter, a, b, res[counter].res_lcm, res[counter].res_gcd);
	}
	
	if(N%2>0) {
		res[counter].res_lcm=matrix[i];
		res[counter].res_gcd=matrix[i];
		printf("%d --- (%d and %d) lcm: %d, gcd: %d\n", counter, matrix[i], matrix[i], res[counter].res_lcm, res[counter].res_gcd);
		counter++;
	}
	
	
	len=counter; //we are saving in the same struct, len is after the last written position
	int repetition=len; //we are calculating until there are no more pairs, only one result
	while(repetition!=1) {
		for(i=0;i<len-1;i++,counter++) { //calculates results for pairs of the struct to get final lcm and gcd of a line
			a=res[i].res_lcm;
			b=res[i+1].res_lcm;
			res[counter].res_lcm=LCM(a,b);
			
			c=res[i].res_gcd;
			d=res[i+1].res_gcd;
			res[counter].res_gcd=GCD(c,d);
			
			printf("%d --- (%d and %d) (%d and %d) lcm: %d, gcd: %d\n", counter, a, b, c,d,res[counter].res_lcm, res[counter].res_gcd);
		}
		len=counter;
		repetition/=2;
	}
	
	*lcm=res[counter-1].res_lcm;
	*gcd=res[counter-1].res_gcd;
}
