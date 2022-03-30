#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../headers/defines.h"
#include "../headers/functions.h"

FILE *f;

void init_xml(const char *filename){
	f = fopen(filename,"w");
	fprintf (f,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\n");
}

void print_to_xml(){
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
	
	//write
	fprintf(f,"<stat>\n");

	fprintf(f,"\t<nr>%6.6d</nr>\n", Counter);
	fprintf(f,"\t<time>%6.6d</time>\n", Counter*log_frequency);
	fprintf(f,"\t<date>%s</date>\n", time);

	fprintf(f,"\t<gen-freq>%d</gen-freq>\n", speed);
        fprintf(f,"\t<row>%d</row>\n", row);
        fprintf(f,"\t<column>%d</column>\n", column);
        fprintf(f,"\t<size>%d</size>\n", size);
	fprintf(f,"\t<lower>%d</lower>\n", lower);
 	fprintf(f,"\t<upper>%d</upper>\n", upper);
 	fprintf(f,"\t<runtime>%d</runtime>\n", running_time);
        fprintf(f,"\t<log-freq>%d</log-freq>\n", log_frequency);
        fprintf(f,"\t<table>%d</table>\n", table_size);

	fprintf(f,"\t<model>%s</model>\n", model);
	fprintf(f,"\t<cache>%s</cache>\n", cache);
	fprintf(f,"\t<cpu>%s</cpu>\n", cpu);

	fprintf(f,"\t<gen>%8.8d</gen>\n", GeneratedCtrl);
	fprintf(f,"\t<drop>%8.8d</drop>\n", DroppedCtrl);
	fprintf(f,"\t<proc>%8.8d</proc>\n", ProcessedCtrl);
	fprintf(f,"\t<dup>%8.8d</dup>\n", DuplicateCtrl);

	for (int i = 0; i < thread_nr; i++){ 
		fprintf(f,"\t<fifo i=%d>%3.3d</fifo>\n", i, FifoLen[i]);
	}

	fprintf(f,"</stat>\n\n");
}

void close_xml() {
	fclose(f);
}
