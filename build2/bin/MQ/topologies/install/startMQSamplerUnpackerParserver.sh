#!/bin/bash
if [ -e /home/cbmdata/_deps/fairsoft_apr22/install//bin/fairmq-shmmonitor ]; then
  /home/cbmdata/_deps/fairsoft_apr22/install//bin/fairmq-shmmonitor --cleanup
fi

if [ -z "$1" ]; then
    _filename=/usr/local/share/cbmroot/input/tofget4_hd2018.tsa
else
    _filename=$1
fi

_paramfile=/usr/local/share/cbmroot/input/MapTofHdCosmics_v18d.par

SAMPLER="TsaSampler"
SAMPLER+=" --id sampler1"
SAMPLER+=" --max-timeslices 10"
#SAMPLER+=" --flib-port 10"
SAMPLER+=" --filename $_filename"
#SAMPLER+=" --flib-host myHost"
SAMPLER+=" --transport shmem"
SAMPLER+=" --channel-config name=tofcomponent,type=push,method=bind,transport=shmem,address=tcp://127.0.0.1:5555"
#SAMPLER+=" --transport zeromq"
#SAMPLER+=" --transport nanomsg"
xterm -geometry 80x23+0+0 -hold -e /usr/local/bin/MQ/source/$SAMPLER &

UNPACKER="UnpackTofStar2018"
UNPACKER+=" --id sink1"
UNPACKER+=" --channel-config name=tofcomponent,type=pull,method=connect,transport=shmem,address=tcp://127.0.0.1:5555"
UNPACKER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:5005"
UNPACKER+=" --transport shmem"
xterm -geometry 80x23+500+0 -hold -e /usr/local/bin/MQ/unpacker/$UNPACKER &

PARAMETERSERVER="parmq-server"
PARAMETERSERVER+=" --id parmq-server"
PARAMETERSERVER+=" --channel-name parameters"
PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:5005"
PARAMETERSERVER+=" --first-input-name $_paramfile"
PARAMETERSERVER+=" --first-input-type ASCII"
xterm -geometry 80x23+500+0 -hold -e /usr/local/bin/MQ/parmq/$PARAMETERSERVER &
#xterm -geometry 80x23+500+0 -hold -e /home/cbmdata/_deps/fairroot_v18.6.9/install//bin/$PARAMETERSERVER &

