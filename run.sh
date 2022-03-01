gcc -Wall -c lcm.c
gcc -Wall -c main.c
gcc -Wall -c file_io.c
gcc -Wall -c print.c

gcc -o test -pthread print.o lcm.o file_io.o  main.o
