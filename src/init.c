#include <time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "../headers/defines.h"
#include "../headers/functions.h"
#include "../headers/def_linux.h"

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
}

void ReadConfig(){
        const char config[] = "../inputs/matrix_conf.cfg";
        read_matrix_config(config, &row, &column, &speed);
        size=row*column;
}

int InitSubsystem(int argc, char** argv){
        TableBusy = 0;
        TablePtr = 0;

        DuplicateCtrl = 0;
        GeneratedCtrl = 0;
        ProcessedCtrl = 0;
        DroppedCtrl = 0;
        Counter = 0;
        Run = 1;
        Enable = 0;

        thread_nr = 4;
	max_thread_param = 4;
        running_time = 2;
        log_frequency = 1;
        lower=1;
        upper=99;
	table_size  = 16384;

        strcpy(output_file_xml,"../results/MatrixReports.xml");
        strcpy(output_file_csv,"../results/MatrixReports.csv");

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

        return 1;
}

