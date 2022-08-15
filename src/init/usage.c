#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../headers/defines.h"
#include "../../headers/init/error.h"
#include "../../headers/init/usage.h"
#include "../../headers/init/init.h"

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
	printf("\t-memory \t difference between 2 consecutive memory usages under which tool can be considered stabil, default = %d\n", mem_delta);
	printf("\t-tree \t\t tree size limit, after how many seconds should the tool delete old nodes, default = %ds \n", tree_limit);
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
        // if there are parameters check them
	// stop the program if any of the paramters are missing/wrong
	if(argc>1){
                for(int i=1;i<argc;i+=2){
                        if(!strcmp(argv[i],"--help") || !strcmp(argv[i],"-h")) {
                                // print the help, possible parameters
				PrintUsage();
                                return 0;
                        }
			else if(!strcmp(argv[i], "-runtime")) {
				// check if it is a positive integer
                                if(!CheckParameter(i, argc, argv, "runtime")) return 0;

				// save runtime
                                running_time=atoi(argv[i+1]);
                        }
			else if(!strcmp(argv[i], "-log")) {
				 // check if it is a positive integer
                                if(!CheckParameter(i, argc, argv, "log")) return 0;
                                
				// save logging frequency
				log_frequency=atoi(argv[i+1]);
                        }
			else if(!strcmp(argv[i], "-row")) {
				 // check if it is a positive integer
                                if(!CheckParameter(i, argc, argv, "matrix row")) return 0;
                                
				// check if it under the max row value
				if(atoi(argv[i+1])>6) {
					error_over_limit("row", 6); 
					return 0;
				}

				// save matrix row number
				row=atoi(argv[i+1]);
                        }
			else if(!strcmp(argv[i], "-column")) {
				 // check if it is a positive integer
                                if(!CheckParameter(i, argc, argv, "matrix column")) return 0;
                                
				// check if it is under the max column value
				if(atoi(argv[i+1])>6) {
					error_over_limit("column", 6); 
					return 0;
				}

				// save matrix column number
				column=atoi(argv[i+1]);
                        }
			else if(!strcmp(argv[i], "-thread")) {
				 // check if it is a positive integer
                                if(!CheckParameter(i, argc, argv, "processing thread")) return 0;
				// check if it is under max thread value
				if(atoi(argv[i+1])>MAX_THREADS) {
					error_over_limit("thread", MAX_THREADS); 
					return 0;
				}
                                
				// check if it is over min thread value
				if(atoi(argv[i+1])<1) {error_under_limit("thread", 4); return 0;}
				// save thread number
				thread_nr=atoi(argv[i+1]);
				if(thread_nr > max_thread_param) 
					max_thread_param = thread_nr;
                        }
			else if(!strcmp(argv[i], "-table")) {
				 // check if it is a positive integer
                                if(!CheckParameter(i, argc, argv, "table")) return 0;
                                
				// check if it is under max table size
				if(atoi(argv[i+1])>MAXTABLE) {
					error_over_limit("table", MAXTABLE); 
					return 0;
				}

				// save table size
                                table_size=atoi(argv[i+1]);
                        }
			else if(!strcmp(argv[i], "-upper")) {
				 // check if it is a positive integer
                                if(!CheckParameter(i, argc, argv, "upper")) return 0;
                                
				// check if it is big than lower interval value
				if(atoi(argv[i+1])<lower) {
					error_lower_upper(); 
					return 0;
				}

				// save upper generating limit
                                upper=atoi(argv[i+1]);
                        }
			else if(!strcmp(argv[i], "-lower")) {
				 // check if it is a positive integer
                                if(!CheckParameter(i, argc, argv, "lower")) return 0;

				// check if it a positive number
                                if(atoi(argv[i+1])<0) {
					error_under_limit("lower", 0); 
					return 0;
				}

				// check if it is smaller than the upper limit
				if(atoi(argv[i+1])>upper) {
					error_lower_upper(); 
					return 0;
				}

				// save lower generating limit
                                lower=atoi(argv[i+1]);
                        }
	
			else if(!strcmp(argv[i], "-xml")) {
				// no file name
                                if(i==argc-1) { 
					error_no_value("xml"); 
					return 0;
				}

				// file name too long
                        	if(strlen(argv[i+1])>=100) {
					error_over_limit("xml",100); 
					return 0;
				}

				// file name is a path
                                if(strstr(argv[i+1],"/")!=NULL) {
                                        printf("Error: output file name can't be a path\n");
                                        return 0;
                                }

				// save filename into param
				char param[100];
				strcpy(param,argv[i+1]);//filename

				// build path to results folder
				char path[120];
				if(outside_build) strcpy(path, "/results/");
					else strcpy(path, "../results/");
				strcat(path, param);//add path

				// add extension
				strcat(path, ".xml");

				// save as xml output file for the program
				strcpy(output_file_xml, path);
			}
			else if(!strcmp(argv[i], "-csv")) {
                                // no file name
				if(i==argc-1) { 
					error_no_value("csv"); 
					return 0;
				}

				// file name too long
                                if(strlen(argv[i+1])>=100) {
					error_over_limit("csv",100); 
					return 0;
				}

				// file name is a path
				if(strstr(argv[i+1],"/")!=NULL) {
					printf("Error: output file name can't be a path\n");
					return 0;
				}
				
				// save filename into param
				char param[100];
                                strcpy(param,argv[i+1]);//filename

				// build path to the results folder
				char path[120];
                                if(outside_build) strcpy(path, "/results/");
                                        else strcpy(path, "../results/");
				strcat(path, param);//add path

				// add extension
                                strcat(path, ".csv");

				//save the parameter as output file of the program
                                strcpy(output_file_csv, path);
                        }
			else if(!strcmp(argv[i], "-counter")) {
				// save counter
				run_counter = atoi(argv[i+1]);
			} 
			else if(!strcmp(argv[i], "-memory")) {
                                if(i==argc-1) { 
					error_no_value("mem-diff"); 
					return 0;
				}
				mem_delta = atoi(argv[i+1]);
                        } 
			else if(!strcmp(argv[i], "-tree")) {
                                 // check if it is a positive integer
                                if(!CheckParameter(i, argc, argv, "tree limit")) return 0;

                                // save thread number
                                tree_limit=atoi(argv[i+1]);
                        }
			else {
				// parameter is not defined, ask for parameters again
				error_wrong_parameter(argv[i]);
				return 0;
			}
		}
        }

	// calculate matrix size after row and column number
	size=row*column;
        return 1;
}
