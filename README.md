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

## Dockerization

Create volume based on working directory
```
docker volume create --name [volume_name] --opt type=none --opt device=~[path_to_working_dir] --opt o=bind
docker volume ls
docker inspect [volume_name]
ls -l $(sudo docker volume inspect [volume_name] | jq -r '.[0].Mountpoint')
```

Run container
- mount volume
- cpu limits: memory, cpu
```
docker run -ti --memory=[memory_limit] --cpus=[cpu_limit] -v [source]:[destination] [img_name] /bin/bash
docker commit [id] [name]
```

## Script to run program with different parameters
- Location: in build folder
- need to give it access rights: chmod 755 <name>
- ./script
- iterates through matrix size, thread numbers, table size -> results in MatrixResults xml and csv files
- run in Docker container

```
#!/bin/bash

for row in {2..3}; do
  for column in {3..4}; do #matrix size
    for (( thread=3; thread<=9; thread+=3 )); do
      for (( lower=10, upper=30; lower<upper; lower+=20, upper-=20 )); do
        for (( table=16384; table>=64; table/=8 )); do
            counter=$((counter+1))
            ./Run -runtime 4 -log 1 -thread $thread -row $row -column $column -lower $lower -upper $upper -table $table -counter $counter
done; done; done; done; done;
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
- **init.c**: initializes parameters
- **system_data.c**: gets platform parameters, cgroup limitations

### headers
- **defines.h**: variables, constants, defined values
- **functions.h**: function declarations
- **def_linx/windows**: os specific macros

### inputs
- **matrix_conf.cfg**: row, column nunmber and generating speed

### results
- MatrixReports.xml
- MatrixReports.csv
- new file: -xml, -csv parameter

## Unit tests
- test.c: Unit test cases
- build/test/function_test_suite.xml: Info about the test case results
