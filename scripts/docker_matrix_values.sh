#!/bin/bash

## creates conatiners with cpu limit between 1-110
## image: benctool_matrix_values, entry point: scripts/matrix_values.sh
for (( cpu=1; cpu<110; cpu+=10)); do
	echo "CPU: $cpu"
	#docker run -it --memory="$mem GiB"  -v /home/etombea/BencmarkingTool/:/home/etombea/BencmarkingTool/  benctool_matrix_values
	docker run -it --cpus=$cpu  -v /home/etombea/BencmarkingTool/:/home/etombea/BencmarkingTool/ benctool_matrix_values
done

## creates containers with mem limit between 1-125 GiB
## image: benctool_matrix_values, entry point: scripts/matrix_values.sh
for (( mem=1; mem<125; mem+=10)); do
        echo "MEMORY: $mem"
        docker run -it --memory="$mem GiB"  -v /home/etombea/BencmarkingTool/:/home/etombea/BencmarkingTool/  benctool_matrix_values
done

