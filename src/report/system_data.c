#ifdef __linux__
#include "../../headers/def_linux.h"
#else
#include "../../headers/def_windows.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../../headers/defines.h"
#include "../../headers/report/system_data.h"

// file pointer for popen
FILE *pop;

// popen for a line a command, result is written in res
void do_popen(char command[100], char *res){
        pop = popen(command, "r");
        if(fgets(res, 100, pop)!=NULL);
        int len=strlen(res);
        res[len-1]='\0';
	pclose(pop);
}

// get platform parameters
void get_system_data() {
	
#ifdef __linux__
	// model name
        sprintf(command,"cat /proc/cpuinfo | grep \"model name\" | uniq | cut -d : -f 2");
        do_popen(command, model);

	// cache size
        sprintf(command,"cat /proc/cpuinfo | grep \"cache size\" | uniq | cut -d : -f 2");
        do_popen(command, cache);
        int len=strlen(cache);
        cache[len-2]='\0';
        cache[len-3]='\0';
        cache[len-1]='\0';

	// cpu cores
        sprintf(command,"cat /proc/cpuinfo | grep \"cpu cores\" | uniq | cut -d : -f 2");
        do_popen(command, cpu);

        //docker limits
        sprintf(command, "cat /sys/fs/cgroup/cpu/cpu.cfs_quota_us");
        do_popen(command, quota);

        sprintf(command, "cat /sys/fs/cgroup/cpu/cpu.cfs_period_us");
        do_popen(command, period);

        sprintf(command, "cat /sys/fs/cgroup/cpu/cpuacct.usage");
	do_popen(command, usage);

        sprintf(command, "cat /sys/fs/cgroup/memory/memory.usage_in_bytes");
        do_popen(command, usage2);

        sprintf(command, "cat /sys/fs/cgroup/memory/memory.limit_in_bytes");
        do_popen(command, limit);
#endif
}
