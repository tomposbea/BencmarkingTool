#include <stdio.h>
#include<errno.h>
#include "../../headers/defines.h"
#include "../../headers/init/read_conf.h"
#define CONFIG_SIZE (256)
#define SPEED_SET (1)
#define n_SET (2)
#define m_SET (3)

typedef struct config {
	unsigned set;
  	int n;
  	int m;
  	int speed;
} CONFIG;

int parse_config(char *buf, CONFIG *config) {
	char dummy[CONFIG_SIZE];
  	
	if (sscanf(buf, " %s", dummy) == EOF) 
		return 0; // blank line
  	
	if (sscanf(buf, " %[#]", dummy) == 1) 
		return 0; // comment
  	
	if(sscanf(buf, "speed = %d", &config->speed) == 1) {
    		if(config->set & SPEED_SET) 
			return SPEED_SET;
    		config->set |= SPEED_SET;
    		return 0;
  	}
  	
	if(sscanf(buf, "n = %d", &config->n) == 1) {
    		if(config->set & n_SET) 
			return n_SET;
    		config->set |= n_SET;
    		return 0;
  	}
	
	if(sscanf(buf, "m = %d", &config->m) == 1) {
    		if(config->set & m_SET) 
			return m_SET;
    		config->set |= m_SET;
    	return 0;
  	}
  	
	return 3; // syntax error
}

void init_config(CONFIG *config) {
	config->set = 0u;
}

void read_matrix_config(const char *filename, int *n, int *m, int *fr){
	FILE *f = fopen(filename, "r");
	
	char buf[CONFIG_SIZE];
	CONFIG config[1];
	init_config(config);
  	int line_number = 0;
  	while (fgets(buf, sizeof buf, f)) {
    		++line_number;
    		int err = parse_config(buf, config);
  	}
  	*n=config->n;
  	*m=config->m;
  	*fr=config->speed;
}
