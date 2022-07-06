#!/bin/bash

cd ../build

if [[ $# -eq 0 ]]
then
        echo Please input new run counter value, tip:MatrixReports.runcounter+1
        exit 1
fi

counter=$1
echo Run counter: $counter

for (( lower=10; lower<100; lower+=10 )); do
            counter=$((counter+1))
            ./Run -runtime 50 -lower $lower -upper 99 -counter $counter
done;

echo Last run counter: $counter
