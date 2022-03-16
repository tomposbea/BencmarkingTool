#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include <poll.h>
#include <sys/stat.h>
#include <sched.h>
#include <sched.h>
#include <time.h>

#include "inc.h"

#define AtomicExchange(var, val) __atomic_exchange_n (var, val, __ATOMIC_RELEASE)
#define AtomicLoad(val) __atomic_load_n(val, __ATOMIC_ACQUIRE)
#define strncpy_s(dest,num1,src,num2) strncpy(dest, src, num2)

const char filename[] = "../input.txt";
const char config[] = "../matrix_conf.cfg";
char * myfifo = "/tmp/myfifo";

int size, row, column, speed;
int nr_matrixes_generated;
int nr_matrixes_lcm;
int nr_matrixes_deleted;
int running_time;

int enable=0, run=1;

int fifo_busy[THREADS];
int fifo_len[THREADS];
int write_ptr[THREADS];
int read_ptr[THREADS];
int table_ptr = 0;
int table_busy=0;
char table[MAXTABLE][STRINGLEN];

struct Transport {
	int valid;
	char string[STRINGLEN];
};
struct Transport Fifo[THREADS][FIFO];

void matrix_config(){
    read_matrix_config(config, &row, &column, &speed);
    printf("Matrix config: speed=%d, N(column)=%d, M(row)=%d\n", speed, column, row);
    size=row*column;
}

int write_to_fifo(int fifonum, char* temp){
	int oldbusy=1;
	while(oldbusy){
		oldbusy=AtomicExchange(&fifo_busy[fifonum],1);
		if(oldbusy){ //check if right fifo
			while(AtomicLoad(&(fifo_busy[fifonum]))) usleep(0); //fifo is busy
			oldbusy=1;
		}
	}

	if(((write_ptr[fifonum]+1)%FIFO) == read_ptr[fifonum]){ //fifo is full
		nr_matrixes_deleted++;
		AtomicExchange(&fifo_busy[fifonum],0);
		return -1;
	} 
	
	printf("\n %d fifo mess: %s\n", fifonum, temp);
	//fifo not full
	int oldwrite=write_ptr[fifonum];
	write_ptr[fifonum]=(write_ptr[fifonum]+1)%FIFO;

	strncpy_s(Fifo[fifonum][oldwrite].string, STRINGLEN, temp, STRINGLEN);
	Fifo[fifonum][oldwrite].valid=1;
	fifo_len[fifonum]++;

	AtomicExchange(&(fifo_busy[fifonum]),0);
	return 0;

}

int FindDuplicate(char* instring) {
	int i;
	for (i = 0; i < MAXTABLE; i++) if (!strcmp(table[i], instring)) return i;  // Found.
	return -1;  // Not Found
}

int process_string(char* instring) {
	if(FindDuplicate(instring)!=-1){ //duplicate found
		nr_matrixes_lcm++;
		return 1;
	}

	//critical section
	int oldbusy=1;
	while(oldbusy){
		oldbusy=AtomicExchange(&table_busy,1);
                if(oldbusy){ //check if ours
                        while(AtomicLoad(&(table_busy))) usleep(0); //fifo is busy
                        oldbusy=1;
                }
        }

	//write to table
	strncpy_s(table[table_ptr], STRINGLEN, instring, STRINGLEN);
	table_ptr=(table_ptr+1)%MAXTABLE;
	nr_matrixes_lcm++;
	AtomicExchange(&table_busy, 0);
	return 0;
}

int read_from_fifo(int fifonum){
	char intext[STRINGLEN];
	if(read_ptr[fifonum]==write_ptr[fifonum]) return -1; //fifo is empty
	
	int oldbusy=1;
	while(oldbusy){
		oldbusy=AtomicExchange(&fifo_busy[fifonum],1);
                if(oldbusy){ //check if right fifo
                        while(AtomicLoad(&(fifo_busy[fifonum]))) usleep(0); //fifo is busy
                        oldbusy=1;
                }
	}
	
	struct Transport* income=Fifo[fifonum]+read_ptr[fifonum];//push data to fifo
	if(income->valid) strncpy_s(intext, STRINGLEN, income->string, STRINGLEN); //save data to stack
	income->valid=0;//release fifo
	read_ptr[fifonum]=(read_ptr[fifonum]+1)%FIFO;
	fifo_len[fifonum]--;

	 AtomicExchange(&(fifo_busy[fifonum]),0);
	 printf("\nRead: %s\n", intext);
	 return process_string(intext);
}

// generates int matrix, converts into char array, writes in pipe
void *thread_generate(void *argt){
    int cpu=sched_getcpu();
    printf("matrix generator cpu core: %i\n", cpu );

    int array[size], matrix[row][column];
    char char_array[CHAR_BUF];
    int roundrobin=0;    

    int fd;
    mkfifo(myfifo, 0666);
    fd=open(myfifo, O_WRONLY);
	
    while(run) {
	if(enable) {
		generate_matrix(row, column, matrix);
		matrix_to_array(row, column, matrix,size,array);	
      		convert_array_to_char(size,array,char_array);

		nr_matrixes_generated++;
                roundrobin++;

		printf("\nWrite: %.*s\n", (int)sizeof(char_array), char_array);	
		write_to_fifo(roundrobin, char_array);
		//write(fd, char_array, sizeof(char_array));
	
		if(roundrobin>=THREADS) roundrobin=0;

		long long int sleep=speed*10000;
		usleep(sleep); //microseconds (1sec = 6zeros)
	} else usleep(0);
    }

    close(fd);
    unlink(myfifo);
}


// reads char array from pipe, converts into int matrix
void *thread_process(void *argt){
    int cpu=sched_getcpu();
    int fifonum=*((int *)argt);
    printf("Process %d cpu core: %i\n",fifonum, cpu );
	
    char buf[STRINGLEN];
    int mess[size];
    int matrix[row][column];

    int fd;
    fd = open(myfifo, O_RDONLY);

    while(run){
	if(enable) {
        	read(fd, buf, MAX_BUF);
        	//read_from_fifo(fifonum);
		printf("\nMess: %s\n", buf); 

		convert_array_to_int(size, buf, mess);
		array_to_matrix(row, column, size, mess,matrix);
	
		lcm(row, column, matrix);
		nr_matrixes_lcm++;
	} else usleep(0);
    }

    close(fd);
}

void *thread_report(void *argt){
    int cpu=sched_getcpu();
    printf("Report cpu core: %i\n", cpu );
   
    printf("\n"); 
    while(run){
	if(enable){
		running_time++;
		nr_matrixes_deleted=nr_matrixes_generated-nr_matrixes_lcm;
        	printf("%dsec, generated: %d, calculated on: %d, lost: %d\n", 
		running_time, nr_matrixes_generated, nr_matrixes_lcm, nr_matrixes_deleted);
	}
	usleep(1000000);
    }
}

void stop_process(){
    printf("\nRunning\n");
    enable=1;
    
    getchar(); //any key
    enable=0;
    run=0;
    printf("\nStopped\n");
}

// create writes and reader thread
void create_threads() {
        pthread_t thread_id;

        pthread_create(&thread_id, NULL, thread_generate, 0);
	pthread_create(&thread_id, NULL, thread_report, 0);

	for(int i=0;i<THREADS;i++)	
		pthread_create(&thread_id, NULL, thread_process, (void *)&i);

	stop_process();

	int *ptr;
        pthread_join(thread_id, (void**)&ptr);
}

void init(){
    nr_matrixes_generated=0;
    nr_matrixes_lcm=0;
    nr_matrixes_deleted=0;
    running_time=0;

    // Init Fifo System.
    for (int i = 0; i < THREADS; i++) {
	for (int j = 0; j < FIFO; j++) Fifo[i][j].valid = 0;
		write_ptr[i] = 0;
		read_ptr[i] = 0;
		fifo_busy[i] = 0;
		fifo_len[i] = 0;
    }

    // Init table
    for (int i = 0; i < MAXTABLE; i++) table[i][0] = 0;
}

int main()
{
    init();
    matrix_config();
    create_threads();

    return 0;
}
