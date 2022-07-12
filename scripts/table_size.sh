#!/bin/bash

cd ../build

if [[ $# -eq 0 ]]
then
        echo Please input new run counter value, tip: 0 or previous counter + 1
        exit 1
fi

counter=$1
echo Run counter: $counter

for (( table=16384; table>=256; table/=2 )); do
            counter=$((counter+1))
            ./Run -runtime 250 -table $table -counter $counter -xml TableSize_250 -csv TableSize_250
done;

echo Last run counter: $counter
