#!/bin/bash

#sudo docker run -it --memory 50m --memory-swap 50m --cpus=".5"  benchtool /bin/bash

#container = `sudo docker ps | grep "benchtool" | cut -d " " -f 1`

for (( i=0;i<5;i++ ));do
	sudo docker stats --no-stream --format "table{{.CPUPerc}}\t{{.MemUsage}}\t{{.MemPerc}}\t{{.NetIO}}"| tail -1 >> stat.txt
	sleep 1
done
