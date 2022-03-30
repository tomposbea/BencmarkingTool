#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../headers/defines.h"
#include "../headers/functions.h"

FILE *fp;

void init_csv(const char *filename){
        fp = fopen(filename,"w");
        fprintf (fp,"nr, time, date, gen-freq, row, column, size, lower, upper, runtime, log-freq, table, op, arch, gen, drop, proc, dup");
	 for (int i = 0; i < thread_nr; i++)
		 fprintf(fp, ", thread%d", i);
	 fprintf(fp,"\n");
}

void print_to_csv(){
	time_t t=time(NULL);
        struct tm *tm=localtime(&t);
        char* time= asctime(tm);
        int len=strlen(time);
        time[len-1]='\0';

	fprintf(fp, "%6.6d, %6.6d, %s, %d, %d, %d, %d, %d, %d, %d, %d, %d, %s, %s, %8.8d, %8.8d, %8.8d, %8.8d",
		   Counter, Counter*log_frequency, time, 
		   speed, row, column, size, lower, upper, running_time, log_frequency, MAXTABLE, 
	       	   "Linux", "x86_64",
    		   GeneratedCtrl, DroppedCtrl, ProcessedCtrl, DuplicateCtrl);
	 for (int i = 0; i < thread_nr; i++)
	 	   fprintf(fp, ", %3.3d", FifoLen[i]);
	fprintf(fp,"\n");	 
}

void close_csv(){
	fclose(fp);
}

