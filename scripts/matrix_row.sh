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
            echo "MATRIX SIZE: $row x 3"
            counter=$((counter+1))
            ./Run -row $row -column 3 -counter $counter -csv MatrixRow_500 -xml MatrixRow_500
done;

echo Last run counter: $counter
