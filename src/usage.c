#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/defines.h"
#include "../headers/functions.h"

void PrintUsage(){
        printf("Usage: ./Run [options]\n");
        printf("\t-runtime \t runtime in seconds, default = %ds\n", running_time);
        printf("\t-log \t\t log frequency in seconds, default = %ds\n", log_frequency);
        printf("\t-thread \t number of processing threads, default = %d, max = %d\n", THREADS, MAX_THREADS);
        printf("\t-table \t\t table size, default: %d, max: %d\n", MAXTABLE, MAXTABLE);
        printf("\t-row \t\t matrix row size, default from inputs/matrix_conf = %d\n", row);
        printf("\t-column \t matrix row size, default from inputs/matrix_conf = %d\n", column);
	//printf("\t-output \t output XML file name, default = %s\n", output_file);
}

int CheckParameter(int i, int len, char** argv, char* option){
	 if(i==len-1) { error_no_value(option); return 0;}
	 int param = atoi(argv[i+1]);
         if(param==0){ error_not_int(option); return 0;}
	 return 1;
}

int GetParameters(int argc, char** argv){
        if(argc>1){
                for(int i=1;i<argc;i++){
                        if(!strcmp(argv[i],"--help") || !strcmp(argv[i],"-h")) {
                                PrintUsage();
                                return 0;
                        }
                        if(!strcmp(argv[i], "-runtime")) {
                                if(!CheckParameter(i, argc, argv, "runtime")) return 0;
                                running_time=atoi(argv[i+1]);
                        }
                        if(!strcmp(argv[i], "-log")) {
                                if(!CheckParameter(i, argc, argv, "log")) return 0;
                                log_frequency=atoi(argv[i+1]);
                        }
                        if(!strcmp(argv[i], "-row")) {
                                if(!CheckParameter(i, argc, argv, "matrix row")) return 0;
                                row=atoi(argv[i+1]);
                        }
                        if(!strcmp(argv[i], "-column")) {
                                if(!CheckParameter(i, argc, argv, "matrix column")) return 0;
                                column=atoi(argv[i+1]);
                        }
			if(!strcmp(argv[i], "-thread")) {
                                if(!CheckParameter(i, argc, argv, "processing thread")) return 0;
				if(atoi(argv[i+1])>MAX_THREADS) {error_over_limit("thread", MAX_THREADS); return 0;}
                                threads_nr=atoi(argv[i+1]);
                        }
			if(!strcmp(argv[i], "-table")) {
                                if(!CheckParameter(i, argc, argv, "table")) return 0;
                                if(atoi(argv[i+1])>MAXTABLE) {error_over_limit("table", MAXTABLE); return 0;}
                                table_size=atoi(argv[i+1]);
                        }	
                	/*if(!strcmp(argv[i], "-output")) {
                                if(i==argc-1) { error_no_value("output"); return 0;}
                        	char* param=argv[i+1];//filename
				char *path="../results/";
				strcat(path, param);//add path
				//int len=strlen(path);
				//char* last4=&path[len-4];
				//if(strcmp(last4, ".xml")) strcat(path, ".xml"); //add extension if missing
				printf("Name: %s", path);
				//strcpy(output_file, path);
			}*/
		}
        }
	size=row*column;
        return 1;
}

