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

SAMPLER="TsaSampler"
SAMPLER+=" --id sampler1"
SAMPLER+=" --max-timeslices 10"
#SAMPLER+=" --flib-port 10"
SAMPLER+=" --filename $_filename"
#SAMPLER+=" --flib-host myHost"
SAMPLER+=" --transport shmem"
SAMPLER+=" --channel-config name=tofcomponent,type=push,method=bind,address=tcp://127.0.0.1:5555"
SAMPLER+=" --channel-config name=stscomponent,type=push,method=bind,address=tcp://127.0.0.1:5556"
#SAMPLER+=" --transport zeromq"
#SAMPLER+=" --transport nanomsg"
xterm -geometry 80x23+0+0 -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/source/$SAMPLER &

SINK="DevNullSink"
SINK+=" --id sink1"
SINK+=" --channel-config name=tofcomponent,type=pull,method=connect,address=tcp://127.0.0.1:5555"
SINK+=" --channel-config name=stscomponent,type=pull,method=connect,address=tcp://127.0.0.1:5556"
SINK+=" --transport shmem"
xterm -geometry 80x23+500+0 -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/sink/$SINK &
