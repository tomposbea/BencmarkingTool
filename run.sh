#gcc -Wall -c lcm.c
#gcc -Wall -c main.c
#gcc -Wall -c file_io.c
#gcc -Wall -c print.c

#gcc -o test -pthread print.o lcm.o file_io.o  main.o

gcc -Wall -c fifo.c
gcc -Wall -c conversions.c
gcc -Wall -c generate.c
gcc -Wall -c print.c
gcc -Wall -c read_conf.c
gcc -o test -pthread fifo.c conversions.o generate.o print.o read_conf.o
