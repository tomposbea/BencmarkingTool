#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../headers/defines.h"
#include "../headers/functions.h"

FILE *f;
FILE *p;

void init_xml(const char *filename){
	f = fopen(filename,"a");

	//writer header if new file
	fseek(f, 0, SEEK_END);
	if( ftell(f) == 0) {
		fprintf (f,"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n\n");
	}
}

void print_to_xml(){
	//get system time
	time_t t=time(NULL);
        struct tm *tm=localtime(&t);
        char* time= asctime(tm);
        int len=strlen(time);
        time[len-1]='\0';

	//get system info
        char command[100];

        sprintf(command,"cat /proc/cpuinfo | grep \"model name\" | uniq | cut -d : -f 2");
        char model[100];
        p = popen(command, "r");
        if(fgets(model, 100, p)!=NULL);
	len=strlen(model);
	model[len-1]='\0';

        sprintf(command,"cat /proc/cpuinfo | grep \"cache size\" | uniq | cut -d : -f 2");
        char cache[100];
        p = popen(command, "r");
        if(fgets(cache, 100, p)!=NULL);
	len=strlen(cache);
	cache[len-1]='\0';
	cache[len-2]='\0';
        cache[len-3]='\0';
        cache[len-4]='\0';

        sprintf(command,"cat /proc/cpuinfo | grep \"cpu cores\" | uniq | cut -d : -f 2");
        char cpu[100];
        p = popen(command, "r");
        if(fgets(cpu, 100, p)!=NULL);
	len=strlen(cpu);
	cpu[len-1]='\0';
	
	//write
	fprintf(f,"<stat>\n");

	fprintf(f,"\t<counter>%6.6d</counter>\n", run_counter);
	fprintf(f,"\t<nr>%6.6d</nr>\n", Counter);
	fprintf(f,"\t<time>%6.6d</time>\n", Counter*log_frequency);
	fprintf(f,"\t<date>%s</date>\n", time);

	fprintf(f,"\t<genfreq>%d</genfreq>\n", speed);
        fprintf(f,"\t<row>%d</row>\n", row);
        fprintf(f,"\t<column>%d</column>\n", column);
        fprintf(f,"\t<size>%d</size>\n", size);
	fprintf(f,"\t<lower>%d</lower>\n", lower);
 	fprintf(f,"\t<upper>%d</upper>\n", upper);
 	fprintf(f,"\t<runtime>%d</runtime>\n", running_time);
        fprintf(f,"\t<logfreq>%d</logfreq>\n", log_frequency);
        fprintf(f,"\t<table>%d</table>\n", table_size);

	fprintf(f,"\t<model>%s</model>\n", model);
	fprintf(f,"\t<cache>%s</cache>\n", cache);
	fprintf(f,"\t<cpu>%s</cpu>\n", cpu);

	fprintf(f,"\t<gen>%8.8d</gen>\n", GeneratedCtrl);
	fprintf(f,"\t<drop>%8.8d</drop>\n", DroppedCtrl);
	fprintf(f,"\t<proc>%8.8d</proc>\n", ProcessedCtrl);
	fprintf(f,"\t<dup>%8.8d</dup>\n", DuplicateCtrl);

	//docker limits
	sprintf(command, "cat /sys/fs/cgroup/cpu/cpu.cfs_quota_us");
	char quota[100];
	p = popen(command, "r");
	if(fgets(quota, 100, p) != NULL);
	len=strlen(quota);
	quota[len-1]='\0';

	sprintf(command, "cat /sys/fs/cgroup/cpu/cpu.cfs_period_us");
        char period[100];
        p = popen(command, "r");
        if(fgets(period, 100, p) != NULL);
        len=strlen(period);
        period[len-1]='\0';

	sprintf(command, "cat /sys/fs/cgroup/cpu/cpuacct.usage");
        char usage[100];
        p = popen(command, "r");
        if(fgets(usage, 100, p) != NULL);
        len=strlen(usage);
        usage[len-1]='\0';

	sprintf(command, "cat /sys/fs/cgroup/memory/memory.usage_in_bytes");
        char usage2[100];
        p = popen(command, "r");
        if(fgets(usage2, 100, p) != NULL);
        len=strlen(usage2);
        usage2[len-1]='\0';

	sprintf(command, "cat /sys/fs/cgroup/memory/memory.limit_in_bytes");
        char limit[100];
        p = popen(command, "r");
        if(fgets(limit, 100, p) != NULL);
        len=strlen(limit);
        limit[len-1]='\0';

	int cpus =  atoll(quota)/atoll(period);
	fprintf(f,"\t<cpuquota>%s</cpuquota>\n", quota);
	fprintf(f,"\t<cpuperiod>%s</cpuperiod>\n", period);
	fprintf(f,"\t<cpus>%d</cpus>\n", cpus);
	fprintf(f,"\t<cpuusage>%s</cpu-sage>\n", usage);
	fprintf(f,"\t<memusage>%s</memusage>\n", usage2);
	 fprintf(f,"\t<memlimit>%s</memlimit>\n",limit);
	
	//threads
	for (int i = 0; i < thread_nr; i++){ 
		fprintf(f,"\t<thread%d>%3.3d</thread%d>\n", i, FifoLen[i], i);
		fprintf(f,"\t<cpucoret%d>%d</cpucoret%d>\n", i, i+2, i);
		
		//mhz
		char comm[150];
		sprintf(comm,"cat /proc/cpuinfo | grep -E 'processor|cpu MHz' | cut -d : -f 2 | paste - - | sed '%dq;d' | cut -d \" \" -f 3", i+2);
        	char mhz[100];
        	p = popen(comm, "r");
        	if(fgets(mhz, 100, p)!=NULL);
		len=strlen(mhz);
        	mhz[len-1]='\0';
		fprintf(f,"\t<cpufreqt%d>%s</cpumfreqt%d>\n", i, mhz, i);
	}

	fprintf(f,"</stat>\n\n");
}

void close_xml() {
	fclose(f);
	pclose(p);
}
