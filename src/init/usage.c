#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/defines.h"
#include "../../headers/init/error.h"
#include "../../headers/init/usage.h"

// print help if parameter is -h or --help
void PrintUsage(){
        printf("Usage: ./Run [options]\n");
        printf("\t-runtime \t runtime in seconds, default = %ds\n", running_time);
        printf("\t-log \t\t log frequency in seconds, default = %ds\n", log_frequency);
        printf("\t-thread \t number of processing threads, default = %d, max = %d\n", thread_nr, MAX_THREADS);
        printf("\t-table \t\t table size, default: %d, max: %d\n", MAXTABLE, MAXTABLE);
        printf("\t-row \t\t matrix row size, default from inputs/matrix_conf = %d\n", row);
        printf("\t-column \t matrix row size, default from inputs/matrix_conf = %d\n", column);
	printf("\t-lower \t\t matrix member lowest value, default between %d - %d\n", lower, upper);
	printf("\t-upper \t\t matrix member highest value, default between %d - %d\n", lower, upper);
	printf("\t-xml \t\t output XML file name, default = %s, ex. MatrixReport\n", output_file_xml);
	printf("\t-csv \t\t output CSV file name, default = %s, ex. MatrixReport\n", output_file_csv);
	printf("\t-counter \t nth run of script\n");
	printf("\t-starttime \t start of first run\n");
}

// check if there is a correct value after parameter
int CheckParameter(int i, int len, char** argv, char* option){
	 if(i==len-1) { error_no_value(option); return 0;}
	 int param = atoi(argv[i+1]);
         if(param==0){ error_not_int(option); return 0;}
	 return 1;
}

// get command line parameters
int GetParameters(int argc, char** argv){
        if(argc>1){
                for(int i=1;i<argc;i+=2){
                        if(!strcmp(argv[i],"--help") || !strcmp(argv[i],"-h")) {
                                PrintUsage();
                                return 0;
                        }
			else if(!strcmp(argv[i], "-runtime")) {
                                if(!CheckParameter(i, argc, argv, "runtime")) return 0;
                                running_time=atoi(argv[i+1]);
                        }
			else if(!strcmp(argv[i], "-log")) {
                                if(!CheckParameter(i, argc, argv, "log")) return 0;
                                log_frequency=atoi(argv[i+1]);
                        }
			else if(!strcmp(argv[i], "-row")) {
                                if(!CheckParameter(i, argc, argv, "matrix row")) return 0;
                                if(atoi(argv[i+1])>6) {error_over_limit("row", 6); return 0;}
				row=atoi(argv[i+1]);
                        }
			else if(!strcmp(argv[i], "-column")) {
                                if(!CheckParameter(i, argc, argv, "matrix column")) return 0;
                                if(atoi(argv[i+1])>6) {error_over_limit("column", 6); return 0;}
				column=atoi(argv[i+1]);
                        }
			else if(!strcmp(argv[i], "-thread")) {
                                if(!CheckParameter(i, argc, argv, "processing thread")) return 0;
				if(atoi(argv[i+1])>MAX_THREADS) {error_over_limit("thread", MAX_THREADS); return 0;}
                                if(atoi(argv[i+1])<1) {error_under_limit("thread", 1); return 0;}
				thread_nr=atoi(argv[i+1]);
				if(thread_nr > max_thread_param) max_thread_param = thread_nr;
                        }
			else if(!strcmp(argv[i], "-table")) {
                                if(!CheckParameter(i, argc, argv, "table")) return 0;
                                if(atoi(argv[i+1])>MAXTABLE) {error_over_limit("table", MAXTABLE); return 0;}
                                table_size=atoi(argv[i+1]);
                        }
			else if(!strcmp(argv[i], "-upper")) {
                                if(!CheckParameter(i, argc, argv, "upper")) return 0;
                                if(atoi(argv[i+1])<lower) {error_lower_upper(); return 0;}
                                upper=atoi(argv[i+1]);
                        }
			else if(!strcmp(argv[i], "-lower")) {
                                if(!CheckParameter(i, argc, argv, "lower")) return 0;
                                if(atoi(argv[i+1])<0) {error_under_limit("lower", 0); return 0;}
				if(atoi(argv[i+1])>upper) {error_lower_upper(); return 0;}
                                lower=atoi(argv[i+1]);
                        }
	
			else if(!strcmp(argv[i], "-xml")) {
                                if(i==argc-1) { error_no_value("xml"); return 0;}
                        	if(strlen(argv[i+1])>=100) {error_over_limit("xml",100); return 0;}
				char param[100];
				strcpy(param,argv[i+1]);//filename
				char path[120];
				if(outside_build) strcpy(path, "/results/");
					else strcpy(path, "../results/");
				strcat(path, param);//add path
				strcat(path, ".xml");
				strcpy(output_file_xml, path);
			}
			else if(!strcmp(argv[i], "-csv")) {
                                if(i==argc-1) { error_no_value("csv"); return 0;}
                                if(strlen(argv[i+1])>=100) {error_over_limit("csv",100); return 0;}
                                char param[100];
                                strcpy(param,argv[i+1]);//filename
				char path[120];
                                if(outside_build) strcpy(path, "/results/");
                                        else strcpy(path, "../results/");
				strcat(path, param);//add path
                                strcat(path, ".csv");
                                strcpy(output_file_csv, path);
                        }
			else if(!strcmp(argv[i], "-counter")) {
				run_counter = atoi(argv[i+1]);
			} 
			 else if(!strcmp(argv[i], "-starttime")) {
                                if(i==argc-1) { error_no_value("startime"); return 0;}
                                strcpy(start_time, argv[i+1]);
				printf("time:%s",  start_time);
                        }
			else {
				error_wrong_parameter(argv[i]);
				return 0;
			}
		}
        }
	size=row*column;
        return 1;
}
