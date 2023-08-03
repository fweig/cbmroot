#!/bin/bash
if [ -e /home/cbmdata/_deps/fairsoft_apr22/install//bin/fairmq-shmmonitor ]; then
  /home/cbmdata/_deps/fairsoft_apr22/install//bin/fairmq-shmmonitor --cleanup
fi

if [ -z "$1" ]; then
#    _filename=/home/heinemann/gpuunpacker/cbmroot/input/stsxyter_cosy2018.tsa
    _filename=/home/heinemann/gpuunpacker/cbmroot/input/tofget4_hd2018.tsa
else
    _filename=$1
fi

SAMPLER="TsaInfo"
SAMPLER+=" --id sampler1"
SAMPLER+=" --max-timeslices 10"
#SAMPLER+=" --flib-port 10"
SAMPLER+=" --filename $_filename"
#SAMPLER+=" --flib-host myHost"
SAMPLER+=" --transport shmem"
#SAMPLER+=" --transport zeromq"
#SAMPLER+=" --transport nanomsg"
xterm -geometry 80x23+0+0 -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/source/$SAMPLER &

