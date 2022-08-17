#ifdef __linux__
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sched.h>
#include <sys/sysinfo.h>
#include <errno.h>
#include <ctype.h>      /* Character Type Classification */
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "../../headers/defines.h"
#include "../../headers/process/binary_search_tree.h"
#include "../../headers/process/redblacktree.h"
#include "../../headers/process/redblacktree2.h"
#include "../../headers/process/hash_map.h"
#include "../../headers/report/write_xml.h"
#include "../../headers/report/write_csv.h"
#include "../../headers/init/usage.h"
#include "../../headers/init/read_conf.h"
#include "../../headers/init/init.h"

#include "../../headers/def_linux.h"

#else
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <Windows.h>
#include <profileapi.h>
#include <sys/stat.h>
#include "windows.h"
#include "../../headers/defines.h"
#include "../../headers/process/binary_search_tree.h"
#include "../../headers/process/redblacktree.h"
#include "../../headers/process/redblacktree2.h"
#include "../../headers/process/hash_map.h"
#include "../../headers/report/write_xml.h"
#include "../../headers/report/write_csv.h"
#include "../../headers/init/usage.h"
#include "../../headers/init/read_conf.h"
#include "../../headers/init/init.h"
#endif

void StopProcess(){
        SleepUni(1);
        printf("\nRUNNING\n");
        Enable = 1;

        // Wait for enter
        getchar();

        // Destroy.
        Enable = 0;
        Run = 0;
        SleepUni(1);
        close_xml();
        close_csv();
	printf("\n\nSTOPPED\n");
	printf("-------------------------------------------------------------------\n");
	exit(0);
}

bool file_exists (char *filename) {
  	struct stat   buffer;   
  	return (stat (filename, &buffer) == 0);
}

void ReadConfig(){
	#ifdef __linux__
    		char config[] = "../inputs/matrix_conf.cfg";
	#else
    		char config[] = "..\\inputs\\matrix_conf.cfg";
	#endif
	
	if (file_exists(config)){
		struct stat stat_record;
		if(stat(config, &stat_record)) {
		#ifdef __linux__
        		strcpy(config, "/inputs/matrix_conf.cfg");
		#else
        		strcpy(config, "inputs\\matrix_conf.cfg");
		#endif
		outside_build = 1;
		}

		read_matrix_config(config, &row, &column, &speed);
        	size=row*column;
	}	
	else // config file not in the location/dufferent name than config[]
	{
		printf("\nError: config file not found. Using default values\n");
		row = 2;
        	column = 2;
        	speed = 1;
        	size = 4;
	}
}

int InitSubsystem(int argc, char** argv){
        outside_build = 0;
	
	TableBusy = 0;
        TablePtr = 0;

        DuplicateCtrl = 0;
        GeneratedCtrl = 0;
        ProcessedCtrl = 0;
        DroppedCtrl = 0;
        Counter = 0;
	Counter2 = 0;
	CounterStop = 0;
	MemoryDiff = 0;
        Run = 1;
        Enable = 0;

	found_hash = 0;
	found_tree = 0;
	found_table = 0;
	found_bstree = 0;

        thread_nr = 4;
	max_thread_param = 4;
        running_time = 10;
        log_frequency = 1;
        lower=10;
        upper=99;
	table_size  = 16384;

	mem_delta = 10000;
	tree_limit = 10;
	
	ReadConfig();
	

	#ifdef __linux__
	if(outside_build){
		strcpy(output_file_xml,"/results/MatrixReports.xml");
        	strcpy(output_file_csv,"/results/MatrixReports.csv");
	} else {
        	strcpy(output_file_xml,"../results/MatrixReports.xml");
        	strcpy(output_file_csv,"../results/MatrixReports.csv");
	}
	#else
	if (outside_build) {
      		strcpy(output_file_xml, "results\\MatrixReports.xml");
        	strcpy(output_file_csv, "results\\MatrixReports.csv");
    	}
    	else {
        	strcpy(output_file_xml, "..\\results\\MatrixReports.xml");
        	strcpy(output_file_csv, "..\\results\\MatrixReports.csv");
    	}
	#endif

	// read and process command line parameters
	if(GetParameters(argc, argv)==0) return 0;

        // Init Fifo System.
        for (int i = 0; i < thread_nr; i++) {
                for (int j = 0; j < FIFO; j++) Fifo[i][j].valid = 0;
                WritePtr[i] = 0;
                ReadPtr[i] = 0;
                FifoBusy[i] = 0;
                FifoLen[i] = 0;
        }
        // Init table
        for (int i = 0; i < table_size; i++) Table[i][0] = 0;

        init_xml(output_file_xml);
        init_csv(output_file_csv);
	
	InitBSTree();
	InitRBTree();
	root = NULL;

	// hash map deleted item replacer
	InitDummyItem();

	return 1;
}

