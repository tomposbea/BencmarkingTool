#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../../headers/defines.h"
#include "../../headers/report/write_csv.h"
#include "../../headers/report/system_data.h"

// filepointer
FILE *fp;

// initialize csv
void init_csv(const char *filename){
        // open file for read and write
	fp = fopen(filename,"a");

	// write header if new file
	fseek(fp, 0, SEEK_END);
	if( ftell(fp) == 0) {
		fprintf(fp, "runsec, runcounter, lognr, logsec, "); //time data
		fprintf(fp, "runtime, logfreq, genfreq, row, column, size, lower, upper, table, threads, "); //configuration parameeters
		fprintf(fp, "model, cache, cpu, cpuquota, cpuperiod, cpuusage, memusage, memlimit, "); //platform parameters
		fprintf(fp, "gen, drop, proc, dup, duphash, duptable, duprbtree, dupbstree"); //program output
		
		for (int i = 0; i < MAX_THREADS; i++) //write header for max threadnumber
			fprintf(fp, ", thread%d, cpucore%d, cpufreq%d", i, i, i);
	 	fprintf(fp,"\n");
	}
}

// print one line to csv
void print_to_csv(){
	// get conatiner, platform configuration
	//get_system_data();

	// calculate output time since first run (different runs with different configurations)
	int run_sec = (run_counter-1) * running_time + Counter*log_frequency;
	
	//print to CSV
	fprintf(fp, "%d, %d, %6.6d, %6.6d, ", run_sec, run_counter, Counter, Counter*log_frequency); //time data
        fprintf(fp, "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, ", running_time, log_frequency, speed, row, column, size, lower, upper, table_size, thread_nr); //configuration parameters
        fprintf(fp, "%s, %s, %s, %s, %s, %s, %s, %s, ", model, cache, cpu, quota, period, usage, usage2, limit); //platform parameters
	fprintf(fp, "%8.8d, %8.8d, %8.8d, %8.8d, %8.8d, %8.8d, %8.8d, %8.8d", GeneratedCtrl, DroppedCtrl, ProcessedCtrl, DuplicateCtrl, found_hash, found_table, found_tree, found_bstree); //program output

	// print for every thread
	for (int i = 0; i < thread_nr; i++){
		// cpu mhz for thread
		#ifdef __linux__
                sprintf(comm,"cat /proc/cpuinfo | grep -E 'processor|cpu MHz' | cut -d : -f 2 | paste - - | sed '%dq;d' | cut -d \" \" -f 3", i+2);
                do_popen(comm, mhz);

		fprintf(fp, ", %3.3d, %d, %s",FifoLen[i], i+2, mhz); //thread info
		#endif
	}

	for (int i = thread_nr; i < MAX_THREADS; i++) //write something to all other threads, so csv is formatted
		 fprintf(fp, ", %s, %s, %s", "", "", "");
	
	fprintf(fp,"\n");	 
}

// close file pointer
void close_csv(){
	fclose(fp);
}

