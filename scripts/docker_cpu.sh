#!/bin/bash

## creates conatiners with cpu limit between 1-110
## image: benctool_matrix_values, entry point: scripts/all_parameetrs2.sh
for (( cpu=10; cpu<110; cpu+=10)); do
	echo "CPU: $cpu"
	docker run -it --cpus=$cpu  -v /home/etombea/BencmarkingTool/:/home/etombea/BencmarkingTool/ benctool_all_parameters
done
