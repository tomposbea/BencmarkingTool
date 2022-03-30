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
	fprintf(f,"<stat>\n");

	fprintf(f,"\t<nr>%6.6d</nr>\n", Counter);
	fprintf(f,"\t<time>%6.6d</time>\n", Counter*log_frequency);
	time_t t=time(NULL);
	struct tm *tm=localtime(&t);
	char* time= asctime(tm);
	int len=strlen(time);
	time[len-1]='\0';
	fprintf(f,"\t<date>%s</date>\n", time);

	fprintf(f,"\t<gen-freq>%d</gen-freq>\n", speed);
        fprintf(f,"\t<row>%d</row>\n", row);
        fprintf(f,"\t<column>%d</column>\n", column);
        fprintf(f,"\t<size>%d</size>\n", size);
	fprintf(f,"\t<lower>%d</lower>\n", lower);
 	fprintf(f,"\t<upper>%d</upper>\n", upper);
 	fprintf(f,"\t<runtime>%d</runtime>\n", running_time);
        fprintf(f,"\t<log-freq>%d</log-freq>\n", log_frequency);
        fprintf(f,"\t<table>%d</table>\n", MAXTABLE);

	fprintf(f,"\t<op>%s</op>\n", "Linux");
	fprintf(f,"\t<arch>%s</arch>\n", "x86_64");

	fprintf(f,"\t<gen>%8.8d</gen>\n", GeneratedCtrl);
	fprintf(f,"\t<drop>%8.8d</drop>\n", DroppedCtrl);
	fprintf(f,"\t<proc>%8.8d</proc>\n", ProcessedCtrl);
	fprintf(f,"\t<dup>%8.8d</dup>\n", DuplicateCtrl);

	for (int i = 0; i < THREADS; i++){ 
		fprintf(f,"\t<fifo i=%d>%3.3d</fifo>\n", i, FifoLen[i]);
	}

	fprintf(f,"</stat>\n\n");
}

void close_xml() {
	fclose(f);
}
