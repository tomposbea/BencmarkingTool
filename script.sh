#!/bin/bash

#limit = sudo docker stats --no-stream --format "table{{.CPUPerc}}\t{{.MemUsage}}\t{{.MemPerc}}\t{{.NetIO}}"| tail -1 | cut -d " " -f 1

for counter in {1..1}; do #nr of repetitions
  for row in {2..3}; do
    for column in {2..3}; do #matrix size
      for thread in {3..4}; do
        for (( lower=1, upper=30; lower<upper; lower+=20, upper-=20 )); do
          for (( table=16384; table>=64; table/=8 )); do
	#	limit = sudo docker stats --no-stream --format "table{{.CPUPerc}}\t{{.MemUsage}}\t{{.MemPerc}}\t{{.NetIO}}"| tail -1 | cut -d " " -f 1
	#	echo $limit
             ./Run -runtime 1 -log 1 -thread $thread -row $row -column $column -lower $lower -upper $upper -table $table
done; done; done; done; done; done
# no need to change filename, results are appended to existing MatrixResults files
