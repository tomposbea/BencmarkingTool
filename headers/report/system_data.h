char command[100], comm[150], model[100], cache[100], cpu[100], quota[100], period[100], usage[100], usage2[100], limit[100], mhz[100];

char old_usage[100], old_usage2[100];

//system_data.c
void do_popen(char command[100], char *res);
void get_system_data();
