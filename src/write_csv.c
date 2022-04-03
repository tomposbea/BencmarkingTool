#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../headers/defines.h"
#include "../headers/functions.h"

FILE *fp;
FILE *pop;

void init_csv(const char *filename){
        fp = fopen(filename,"a");

	// write header if new file
	fseek(fp, 0, SEEK_END);
	if( ftell(fp) == 0) {
        	fprintf (fp,"nr, time, date, gen-freq, row, column, size, lower, upper, runtime, log-freq, table, model, cache, cpu, gen, drop, proc, dup");
	 	for (int i = 0; i < MAX_THREADS; i++)
			fprintf(fp, ", thread%d, cpucore-t%d, cpumhz-t%d", i, i, i);
	 	fprintf(fp,"\n");
	}
}

void print_to_csv(){
	//get system time
	time_t t=time(NULL);
        struct tm *tm=localtime(&t);
        char* time= asctime(tm);
        int len=strlen(time);
        time[len-1]='\0';

	//get system info
	char command[100];
        
	sprintf(command,"cat /proc/cpuinfo | grep \"model name\" | uniq | cut -d : -f 2");
	char model[100];
        pop = popen(command, "r");
	if(fgets(model, 100, pop)!=NULL);
	len=strlen(model);
        model[len-1]='\0';

	sprintf(command,"cat /proc/cpuinfo | grep \"cache size\" | uniq | cut -d : -f 2");
        char cache[100];
	pop = popen(command, "r");
        if(fgets(cache, 100, pop)!=NULL);
	len=strlen(cache);
        cache[len-1]='\0';

        sprintf(command,"cat /proc/cpuinfo | grep \"cpu cores\" | uniq | cut -d : -f 2");
        char cpu[100];
	pop = popen(command, "r");
        if(fgets(cpu, 100, pop)!=NULL);
	len=strlen(cpu);
        cpu[len-1]='\0';

	//print
	fprintf(fp, "%6.6d, %6.6d, %s, %d, %d, %d, %d, %d, %d, %d, %d, %d, %s, %s, %s, %8.8d, %8.8d, %8.8d, %8.8d",
		   Counter, Counter*log_frequency, time, 
		   speed, row, column, size, lower, upper, running_time, log_frequency, table_size,
		   model, cache, cpu,
    		   GeneratedCtrl, DroppedCtrl, ProcessedCtrl, DuplicateCtrl);
	 for (int i = 0; i < thread_nr; i++){
	 	fprintf(fp, ", %3.3d, %d", FifoLen[i], i+2);
		 //mhz
		char comm[150];
                sprintf(comm,"cat /proc/cpuinfo | grep -E 'processor|cpu MHz' | cut -d : -f 2 | paste - - | sed '%dq;d' | cut -d \" \" -f 3", i+2);
                char mhz[100];
                pop = popen(comm, "r");
                if(fgets(mhz, 100, pop)!=NULL);
                len=strlen(mhz);
                mhz[len-1]='\0';
		fprintf(fp, ", %s", mhz);
	 }
	 for (int i = thread_nr; i < MAX_THREADS; i++)
		 fprintf(fp, ", %3.3d, %d, %s", 0, 0, "");
	fprintf(fp,"\n");	 
}

void close_csv(){
	fclose(fp);
	pclose(pop);
}

