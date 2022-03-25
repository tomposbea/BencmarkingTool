#include <stdio.h>
#include <stdlib.h>
#include "../headers/defines.h"
#include "../headers/functions.h"


void init_matrix_reports(const char *filename){
	f = fopen(filename,"w");
	fprintf (f,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\n");
}

void print_matrix_reports(){
	fprintf(f,"<matrix-stat counter=%6.6d>\n", Counter);

	fprintf(f,"\t<generated>%8.8d</generated>\n", GeneratedCtrl);
	fprintf(f,"\t<dropped>%8.8d</dropped>\n", DroppedCtrl);
	fprintf(f,"\t<processed>%8.8d</processed>\n", ProcessedCtrl);
	fprintf(f,"\t<duplicates>%8.8d</duplicates>\n", DuplicateCtrl);

	fprintf(f,"\t<fifo>\n");
	for (int i = 0; i < THREADS; i++){ 
		fprintf(f,"\t\t<lenght i=%d>%3.3d</length>\n", i, FifoLen[i]);
	}
	fprintf(f,"\t</fifo>\n");

	fprintf(f,"\t<matrix-config>\n");
	fprintf(f,"\t\t<generating-frequency>%d</generating-frequency>\n", speed);
	fprintf(f,"\t\t<row>%d</row>\n", row);
	fprintf(f,"\t\t<column>%d</column>\n", column);
	fprintf(f,"\t</matrix-config>\n");

	fprintf(f,"</matrix-stat>\n\n");
}

void close_matrix_reports() {
	fclose(f);
}
