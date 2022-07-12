FROM gcc:9.4
WORKDIR /home/etombea/BencmarkingTool/build
ENTRYPOINT ./Run -runtime 2
