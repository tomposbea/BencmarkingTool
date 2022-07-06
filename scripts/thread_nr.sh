#!/bin/bash

cd ../build

if [[ $# -eq 0 ]]
then
        echo Please input new run counter value, tip:MatrixReports.runcounter+1
        exit 1
fi

counter=$1
echo Run counter: $counter

for (( thread=4; thread<=24; thread+=1 )); do
            counter=$((counter+1))
            ./Run -runtime 50 -thread $thread -counter $counter
done;

echo Last run counter: $counter

