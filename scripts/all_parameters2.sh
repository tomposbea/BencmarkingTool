#!/bin/bash
cd ../build

if [[ $# -eq 0 ]]
then
        echo Please input new run counter value, tip: 0 or previous value + 1
        exit 1
fi

counter=$1
echo Run counter: $counter

for row in {3..5}; do
  for (( lower=10; lower<=70; lower+=20 )); do
    for (( table=16384; table>=256; table/=8 )); do
      for (( thread=4; thread<=20; thread+=4 )); do
            ./Run -row $row \
                -column 4 \
                -lower $lower \
                -upper 99 \
                -table $table \
                -thread $thread \
                -counter $counter \
                -csv Result_docker_cpu2 -xml Result_docker_cpu2
        counter=$((counter+2))
      done;
    done;
  done;
done;

echo Last run counter: $counter

