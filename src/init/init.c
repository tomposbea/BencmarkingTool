#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "../../headers/defines.h"
#include "../../headers/process/binary_search_tree.h"
#include "../../headers/process/redblacktree.h"
#include "../../headers/process/hash_map.h"
#include "../../headers/report/write_xml.h"
#include "../../headers/report/write_csv.h"
#include "../../headers/init/usage.h"
#include "../../headers/init/read_conf.h"
#include "../../headers/init/init.h"

#include "../../headers/def_linux.h"

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

void ReadConfig(){
        char config[] = "../inputs/matrix_conf.cfg";
	struct stat stat_record;
	if(stat(config, &stat_record)) {
  //    		printf("Config file not found\n");
		strcpy(config, "/inputs/matrix_conf.cfg");
		outside_build = 1;
	}
//	printf("Config file: %s\n", config);
        read_matrix_config(config, &row, &column, &speed);
        size=row*column;
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
        Run = 1;
        Enable = 0;

	found_hash = 0;
	found_tree = 0;
	found_table = 0;
	found_bstree = 0;

        thread_nr = 4;
	max_thread_param = 4;
        running_time = 2;
        log_frequency = 1;
        lower=10;
        upper=99;
	table_size  = 16384;

	if(outside_build){
		strcpy(output_file_xml,"/results/MatrixReports.xml");
        	strcpy(output_file_csv,"/results/MatrixReports.csv");
	} else {
        	strcpy(output_file_xml,"../results/MatrixReports.xml");
        	strcpy(output_file_csv,"../results/MatrixReports.csv");
	}

        ReadConfig();
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

	// hash map deleted item replacer
	InitDummyItem();

	return 1;
}
