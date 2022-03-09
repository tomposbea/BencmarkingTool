#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "inc.h"

// int array -> char array
void convert_array_to_char(int size, int a[size], char b[128]){
        int n=0;
        for(int i=0;i<size;i++){
                n+=sprintf(&b[n], "%d", a[i]);
                n+=sprintf(&b[n], "%s", " ");
        }
}

// char array -> int array
void convert_array_to_int(int size, char *c, int res[size]){
        int len=strlen(c);
        int count=0;
        int nr, nr2;
        for(int i=0;i<len;i++){
                if(c[i]!=' ') {
                        nr=c[i]-'0';
                        while(c[i+1]!=' '){
                                nr*=10;
                                nr2 = c[i+1]-'0';
                                nr+=nr2;
                                i++;
                        }
                        res[count++]=nr;
                }
        }
}


// flattens matrix into an array, it can be sent in one line

void matrix_to_array(int row, int column, int a[row][column], int size, int res[size]){
        int nr=0;
        for(int i=0;i<row;i++)
                for(int j=0;j<column;j++)
                        res[nr++]=a[i][j];
}

void array_to_matrix(int row, int column, int size, int array[size], int res[row][column]) {
        int nr=0;
        for(int i=0;i<row;i++)
                for(int j=0;j<column;j++)
                        res[i][j]=array[nr++];
}

