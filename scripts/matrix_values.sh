#!/bin/bash

cd ../build

if [[ $# -eq 0 ]]
then
        echo Please input new run counter value, 0 or previous value + 1
        exit 1
fi

counter=$1
echo Run counter: $counter

for (( lower=10; lower<100; lower+=20 )); do
	    echo "LOWER: $lower"
            counter=$((counter+1))
	    ./Run -runtime 500 -lower $lower -upper 99 -counter $counter -csv MatrixValues_500 -xml MatrixValues_500
	    #./Run -runtime 250 -lower $lower -upper 99 -counter $counter -csv Docker_Matrix_Values_250 -xml Docker_Matrix_Values_250
done;

echo Last run counter: $counter
