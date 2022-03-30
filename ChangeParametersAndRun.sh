#!/bin/bash

# parameters
time=1
log=1
table=16384
lower=1
upper=99
file="Result"

counter=0
while [[ $counter -lt 1 ]]; do
  for row in {5..6}; do
    for column in {5..6}; do
      filename="${file}${counter}"
      for thread in {2..3}; do
        ./Run -runtime $time -thread $thread -row $row -column $column -xml $filename -csv $filename
        counter=$((counter+1))
      done
    done
  done
done

#./Run -runtime -log -thread - table -row -column -lower -upper -xml -csv
