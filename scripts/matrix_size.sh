#!/bin/bash
cd ../build

if [[ $# -eq 0 ]]
then
        echo Please input new run counter value, tip: 0 or previous value + 1
        exit 1
fi

counter=$1
echo Run counter: $counter

for row in {2..5}; do
  for column in {2..5}; do
	    echo "MATRIX SIZE: $row x $column"
            counter=$((counter+1))
            ./Run -runtime 250 -row $row -column $column -counter $counter -csv MatrixSize_250 -xml MatrixSize_250
done; done; 

echo Last run counter: $counter
