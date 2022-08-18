#!/bin/bash
cd ../build

if [[ $# -eq 0 ]]
then
        echo Please input new run counter value, tip: 0 or previous value + 1
        exit 1
fi

counter=$1
echo Run counter: $counter

# row=3 in Results_dut37
for row in {4..5}; do
  for (( lower=10; lower<=70; lower+=20 )); do    
    for (( table=16384; table>=256; table/=2 )); do
      for (( thread=4; thread<=20; thread+=1 )); do
	    ./Run -row $row \
		-column 4 \
		-lower $lower \
		-upper 99 \
		-table $table \
		-thread $thread \
		-counter $counter \
		-csv Result_dut37 -xml Result_dut37
        counter=$((counter+2))
      done;
    done;
  done;
done;

echo Last run counter: $counter
