# BencmarkingTool

## Eric-Test Framework
```bash
git submodule add ssh://gerrit.ericsson.se:29418/pc/eric-test
```

## Compilation with CMake

```bash
mkdir build
cd build
cmake ..
cmake --build .
./Run
```

## Running Unit Tests
```bash
cd build
ctest
```

## Implementing Codemodel-V2
```bash
cd build
mkdir -p .cmake/api/v1/query
touch .cmake/api/v1/query/codemodel-v2
cmake ..
```

## Files
#### main.c 
- Reads matrix configuration(size, how often it should be generated) from matrix_conf.cfg
- Thread 0: generates random matrixes every few seconds, write them in pipe
- Thread 1: calculate LCM, GCD for each matrix line
- Thread 2: log how many matrixes were generated, calculated on and lost
- TODO: poll

#### fifo.c
- Writing to FIFO, number of fifos defined in defines.h
- Reading from FIFO
- check if message is duplicate, do calculations

#### lcm.c 
- Calculate LCM, GCD for 2 values, an array, a matrix

#### file_io.c 
- Reading NxM matrix from file
- Writing results to file

#### print.c 
- Printing array, matrix to terminal
- Printing results to terminal

#### generate.c
- Generate random number between 2 values
- Generate NxM matrix

#### read_conf.c
- Reads configuration file

#### conversions.c
- Necessary matrix conversions to write matrixes as char arrays
- int matrix <-> int array
- int array <-> char array

#### test/test.c
- Unit test cases

#### Headers
- defines.h: variables, constants, defined values
- functions.h: function declarations
- def_linx/windows: os specific macros

#### Input files
- input.txt: a matrix
- matrix_conf.cfg: row, column nunmber and generating speed
