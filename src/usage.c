#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/defines.h"
#include "../headers/functions.h"

void PrintUsage(){
        printf("Usage: ./Run [options]\n");
        printf("\t-runtime \t runtime in seconds, default = 10s\n");
        printf("\t-log \t\t log frequency in seconds, default = 1s\n");
        printf("\t-thread \t number of processing threads, default = 4, max = %d\n", MAX_THREADS);
        printf("\t-table \t\t table size, default: %d, max: %d\n", TABLE, MAXTABLE);
        printf("\t-row \t\t matrix row size, default from inputs/matrix_conf = %d\n", row);
        printf("\t-column \t matrix row size, default from inputs/matrix_conf = %d\n", column);
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
                                THREADS=atoi(argv[i+1]);
                        }
			if(!strcmp(argv[i], "-table")) {
                                if(!CheckParameter(i, argc, argv, "table")) return 0;
                                if(atoi(argv[i+1])>MAXTABLE) {error_over_limit("table", MAXTABLE); return 0;}
                                TABLE=atoi(argv[i+1]);
                        }	
                }
        }
        return 1;
}

