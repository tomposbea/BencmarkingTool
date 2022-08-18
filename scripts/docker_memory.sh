## creates containers with mem limit between 1-125 GiB
## image: benctool_matrix_values, entry point: scripts/matrix_values.sh
for (( mem=10; mem<125; mem+=20)); do
        echo "MEMORY: $mem"
        docker run -it --memory="$mem GiB"  -v /home/etombea/BencmarkingTool/:/home/etombea/BencmarkingTool/  benctool_all_parameters
done
