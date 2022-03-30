#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../headers/defines.h"
#include "../headers/functions.h"

FILE *fp;

void init_csv(const char *filename){
        fp = fopen(filename,"w");
        fprintf (fp,"nr, time, date, gen-freq, row, column, size, lower, upper, runtime, log-freq, table, arch, gen, drop, proc, dup");
	 for (int i = 0; i < thread_nr; i++)
		 fprintf(fp, ", thread%d", i);
	 fprintf(fp,"\n");
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
        FILE *pop;
        pop = popen(command, "r");
        
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

        pclose(pop);

	//print
	fprintf(fp, "%6.6d, %6.6d, %s, %d, %d, %d, %d, %d, %d, %d, %d, %d, %s, %s, %s, %8.8d, %8.8d, %8.8d, %8.8d",
		   Counter, Counter*log_frequency, time, 
		   speed, row, column, size, lower, upper, running_time, log_frequency, table_size,
		   model, cache, cpu,
    		   GeneratedCtrl, DroppedCtrl, ProcessedCtrl, DuplicateCtrl);
	 for (int i = 0; i < thread_nr; i++)
	 	   fprintf(fp, ", %3.3d", FifoLen[i]);
	fprintf(fp,"\n");	 
}

void close_csv(){
	fclose(fp);
}

