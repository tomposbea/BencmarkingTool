# BencmarkingTool

## Compilation with gcc

```bash
chmod 755 run.sh
./run.sh
./test
```

## Compilation with CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
./Run
```

## Files
#### main.c 
- Reads matrix configuration(size, how often it should be generated) from matrix_conf.cfg
- Generates random matrixes every few seconds, until CTRL+C
- Creates row number threads, running on different CPUs
- Threads calculate results for matrix rows
- TODO: thread creation inside of loop(floating point exception after first iteration)

#### lcm.c 
- Calculate LCM, GCD for 2 values
- Calculate LCM, GCD final LCM, GCD for an array

#### file_io.c 
- Reading NxM matrix from file
- Writing results to file

#### print.c 
- Printing matrix to terminal
- Printing results to terminal

#### generate.c
- Generate random number between 2 values
- Generate NxM matrix

#### read_conf.c
- Reads configuration file

#### Headers
- includes.h

#### Input files
- input.txt
- matrix_conf.cfg
