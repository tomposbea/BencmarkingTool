#!/bin/bash
cd ../build

if [[ $# -eq 0 ]]
then
        echo Please input new run counter value, tip:MatrixReports.runcounter+1
        exit 1
fi

counter=$1
echo Run counter: $counter

for row in {1..6}; do
  for column in {1..6}; do
            counter=$((counter+1))
            ./Run -runtime 50 -row $row -column $column -counter $counter
done; done; 

echo Last run counter: $counter
