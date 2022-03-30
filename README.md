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
./Run [parameters]
```

- ./Run -h
- ./Run --help

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

## Dockerfile
- Location: outside of BenchmarkingTool folder
- TODO: unit tests

```bash
FROM gcc:9.2
ENV DEBIAN_FRONTEND noninteractive

# copy from host to working directory
COPY . /BencmarkingTool

WORKDIR /BencmarkingTool/

# install cmake
RUN     apt-get update && \
        apt-get install -y --no-install-recommends apt-utils && \
        apt-get -y install cmake

# build the application with cmake
RUN     mkdir BencmarkingTool/build && \
        cd BencmarkingTool/build && \
        cmake .. && \
        cmake --build .

RUN ["chmod", "+x", "BencmarkingTool/build"]

# run the application
CMD ["BencmarkingTool/build/Run"]
```

## STRUCTURE
### main.c 
- Reads matrix configuration(size, how often it should be generated) from matrix_conf.cfg
- Thread 0: generates random matrixes every few seconds write them in fifo's(roundrobin)
- Thread 1: check if the new matrix is a duplicate, if not calculate LCM, GCD for each matrix line
- Thread 2: log how many matrixes were generated, calculated, lost and how many were duplicates

### src
- **fifo.c**: read/write to fifos, check if message is duplicate, do calculations
- **lcm.c**: calculate LCM, GCD for 2 values, an array, a matrix
- **file_io.c**: reading NxM matrix from file, writing results to txt file, NOT USED
- **print.c**: printing array, matrix, results, logs to terminal
- **generate.c**: generate random number between 2 values, generate NxM matrix, check if matrix is unique
- **read_conf.c**: reads configuration file with matrix size and generating speed info
- **conversions.c**: necessary matrix conversions to write matrixes as char arrays
- **write_xml.c**: writes Thread 2 output to an xml file
- **write_csv.c**: writes Thread 2 output to an csv file
- **error.c**: error messages for input parameters
- **usage.c**: read+check input parameters, --help option

### headers
- **defines.h**: variables, constants, defined values
- **functions.h**: function declarations
- **def_linx/windows**: os specific macros

### inputs
- **matrix_conf.cfg**: row, column nunmber and generating speed

### results
- MatrixReports.xml
- MatrixReports.csv

## Unit tests
- test.c: Unit test cases
- build/test/function_test_suite.xml: Info about the test case results
