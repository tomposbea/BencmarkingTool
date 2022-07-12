#!/bin/bash

cd ../build

if [[ $# -eq 0 ]]
then
        echo Please input new run counter value, tip: 0 or previous value + 1
        exit 1
fi

counter=$1
echo Run counter: $counter

for (( thread=4; thread<=20; thread+=1 )); do
	    echo "THREADS: $thread"
            counter=$((counter+1))
            ./Run -runtime 250 -thread $thread -counter $counter -csv ThreadNr_250 -xml ThreadNr_250
done;

echo Last run counter: $counter

