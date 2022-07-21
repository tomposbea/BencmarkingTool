#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../../headers/defines.h"
#include "../../headers/report/write_xml.h"
#include "../../headers/report/system_data.h"

// file pointer
FILE *f;

void init_xml(const char *filename){
	// open file for read and write
	f = fopen(filename,"a");

	//write header if new file
	fseek(f, 0, SEEK_END);
	if( ftell(f) == 0) {
		fprintf (f,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\n");
	}
}

void print_to_xml(){
	//get_system_data();	
	int run_sec = (run_counter-1) * running_time + Counter*log_frequency;
	
	//print to XML
	fprintf(f,"<stat>\n");

	//time data
	fprintf(f,"\t<runsec>%6.6d</runsec>\n", run_sec);
	fprintf(f,"\t<runcounter>%6.6d</runcounter>\n", run_counter);
	fprintf(f,"\t<lognr>%6.6d</lognr>\n", Counter);
	fprintf(f,"\t<logsec>%6.6d</logsec>\n", Counter*log_frequency);

	//configuration parameters
	fprintf(f,"\t<runtime>%d</runtime>\n", running_time);
	fprintf(f,"\t<logfreq>%d</logfreq>\n", log_frequency);
	fprintf(f,"\t<genfreq>%d</genfreq>\n", speed);
        fprintf(f,"\t<row>%d</row>\n", row);
        fprintf(f,"\t<column>%d</column>\n", column);
        fprintf(f,"\t<size>%d</size>\n", size);
	fprintf(f,"\t<lower>%d</lower>\n", lower);
 	fprintf(f,"\t<upper>%d</upper>\n", upper);
	fprintf(f,"\t<table>%d</table>\n", table_size);
	fprintf(f,"\t<threads>%d</threads>\n", thread_nr);

	//platform parameters
	fprintf(f,"\t<model>%s</model>\n", model);
	fprintf(f,"\t<cache>%s</cache>\n", cache);
	fprintf(f,"\t<cpu>%s</cpu>\n", cpu);
	fprintf(f,"\t<cpuquota>%s</cpuquota>\n", quota);
	fprintf(f,"\t<cpuperiod>%s</cpuperiod>\n", period);
	fprintf(f,"\t<cpuusage>%s</cpuusage>\n", usage);
	fprintf(f,"\t<memusage>%s</memusage>\n", usage2);
	fprintf(f,"\t<memlimit>%s</memlimit>\n", limit);

	//program output
	fprintf(f,"\t<gen>%8.8d</gen>\n", GeneratedCtrl);
	fprintf(f,"\t<drop>%8.8d</drop>\n", DroppedCtrl);
	fprintf(f,"\t<proc>%8.8d</proc>\n", ProcessedCtrl);
	fprintf(f,"\t<dup>%8.8d</dup>\n", DuplicateCtrl);
	fprintf(f,"\t<duphash>%8.8d</duphash>\n", found_hash);
	fprintf(f,"\t<duptable>%8.8d</duptable>\n", found_table);
	fprintf(f,"\t<duprbtree>%8.8d</duprbtree>\n", found_tree);
	fprintf(f,"\t<dupbstree>%8.8d</dupbstree>\n", found_bstree);

	//threads
	#ifdef __linux__
	for (int i = 0; i < thread_nr; i++){ 
		sprintf(comm,"cat /proc/cpuinfo | grep -E 'processor|cpu MHz' | cut -d : -f 2 | paste - - | sed '%dq;d' | cut -d \" \" -f 3", i+2);
                do_popen(comm, mhz);

		fprintf(f,"\t<thread%d>%3.3d</thread%d>\n", i, FifoLen[i], i); //thread info
		fprintf(f,"\t<cpucore%d>%d</cpucore%d>\n", i, i+2, i);
		fprintf(f,"\t<cpufreq%d>%s</cpufreq%d>\n", i, mhz, i);
	}
	#endif

	fprintf(f,"</stat>\n\n");
}

// close file pointer
void close_xml() {
	fclose(f);
}
