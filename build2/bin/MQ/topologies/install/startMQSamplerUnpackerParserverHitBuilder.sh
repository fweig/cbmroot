#!/bin/bash

Running=`ps uaxww | grep HitB`

if [[ $Running == *"hitbuilder1"* ]]; then
    echo Hitbuilder is still running, wait or cleanup!
    exit 1
fi

if [ -e /home/cbmdata/_deps/fairsoft_apr22/install//bin/fairmq-shmmonitor ]; then
  /home/cbmdata/_deps/fairsoft_apr22/install//bin/fairmq-shmmonitor --cleanup
fi

if [ -z "$1" ]; then
    _runname="r0088_20180905_1602"
else
    _runname=$1
fi

_reqmod=3
_reqtint=100
_batch=1
_iUnp=6

_dirname=/usr/local/share/cbmroot/macro/beamtime/star2019/input/$_runname/
_filename=$_runname_*.tsa
_digifile=$_runname.$_reqtint.$_reqmod.root

# ASCII files
_mapfile=/usr/local/share/cbmroot/input/MapTofGbtx.par
#_digibdffile=/usr/local/share/cbmroot/input/v18j_cosmicHD.digibdf.par
#_digiparfile=/usr/local/share/cbmroot/input/tof_v18j_cosmicHD.digi.par
_digibdffile=/usr/local/share/cbmroot/parameters/tof/v18_star_cosmics.digibdf.par
_digiparfile=/usr/local/share/cbmroot/parameters/tof/tof_v18_star_cosmics.digi.par

# ROOT files
#_geofile=/usr/local/share/cbmroot/input/tof_v18j_cosmicHD.par.root
_geofile=/usr/local/share/cbmroot/input/tof_v18_star_cosmics.par.root

rm -v nohup.out
rm -v *log
rm all_*

PARAMETERSERVER="parmq-server"
echo pkill $PARAMETERSERVER
pkill $PARAMETERSERVER
sleep 1
PARAMETERSERVER+=" --id parmq-server"
PARAMETERSERVER+=" --channel-name parameters"
PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:5005"
#PARAMETERSERVER+=" --first-input-name $_paramfile"
PARAMETERSERVER+=" --first-input-name $_mapfile;$_digiparfile;$_digibdffile"
PARAMETERSERVER+=" --first-input-type ASCII"
PARAMETERSERVER+=" --second-input-name $_geofile"
PARAMETERSERVER+=" --second-input-type ROOT"
PARAMETERSERVER+=" --verbosity INFO"
if  [[ $_batch = 1 ]]; then
PARAMETERSERVER+=" --control static"
PARAMETERSERVER+=" --log-to-file ParServ.out"
nohup /usr/local/bin/MQ/parmq/$PARAMETERSERVER &
else
xterm -geometry 80x23+0+340 -hold -e /usr/local/bin/MQ/parmq/$PARAMETERSERVER &
#xterm -geometry 80x23+500+0 -hold -e /home/cbmdata/_deps/fairroot_v18.6.9/install//bin/$PARAMETERSERVER &
fi

SAMPLER="TsaSamplerTof"
SAMPLER+=" --id sampler1"
#SAMPLER+=" --max-timeslices -1"
SAMPLER+=" --max-timeslices 1000"
#SAMPLER+=" --flib-port 10"
SAMPLER+=" --dirname $_dirname"
SAMPLER+=" --filename $_filename"
#SAMPLER+=" --flib-host myHost"
SAMPLER+=" --channel-config name=tofcomponent,type=push,method=bind,transport=zeromq,address=tcp://*:5655"
SAMPLER+=" --channel-config name=syscmd,type=pub,method=bind,transport=zeromq,address=tcp://*:5666"
#SAMPLER+=" --transport shmem"
#SAMPLER+=" --transport zeromq"
#SAMPLER+=" --transport nanomsg"
#SAMPLER+=" --verbosity WARN"
SAMPLER+=" --verbosity INFO"
#SAMPLER+=" --verbosity DEBUG"
if  [[ $_batch = 1 ]]; then
SAMPLER+=" --log-to-file Sampl.out"
SAMPLER+=" --control static"
nohup  /usr/local/bin/MQ/source/$SAMPLER &
else
xterm -geometry 80x23+0+0 -hold -e /usr/local/bin/MQ/source/$SAMPLER &
fi

while (( _iUnp > 0 )); do

UNPACKER="UnpackTofMcbm2018"
UNPACKER+=" --id unpack$_iUnp"
UNPACKER+=" --channel-config name=tofcomponent,type=pull,method=connect,transport=zeromq,address=tcp://127.0.0.1:5655"
UNPACKER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:5005"
UNPACKER+=" --channel-config name=tofdigis,type=push,method=connect,transport=shmem,address=tcp://127.0.0.1:5656"
UNPACKER+=" --channel-config name=syscmd,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:5666"
#UNPACKER+=" --transport shmem"
#UNPACKER+=" --verbosity DEBUG"
UNPACKER+=" --verbosity  INFO"
UNPACKER+=" --ReqMode $_reqmod"
UNPACKER+=" --ReqTint $_reqtint"
UNPACKER+=" --ReqDet0       6"
UNPACKER+=" --ReqDet1      22"
UNPACKER+=" --ReqDet2      38"
UNPACKER+=" --ReqDet3      54"
UNPACKER+=" --ReqDet4      70"
UNPACKER+=" --ReqDet5      86"
UNPACKER+=" --ReqDet6   65542"
UNPACKER+=" --ReqDet7   65558"
UNPACKER+=" --ReqDet8   65574"
UNPACKER+=" --ReqDet9   65590"
UNPACKER+=" --ReqDet10  65606"
UNPACKER+=" --ReqDet11  65622"
UNPACKER+=" --ReqDet12 131078"
UNPACKER+=" --ReqDet13 131094"
UNPACKER+=" --ReqDet14 131110"
UNPACKER+=" --ReqDet15 131126"
UNPACKER+=" --ReqDet16 131142"
UNPACKER+=" --ReqDet17 131158"
if  [[ $_batch = 1 ]]; then
UNPACKER+=" --control static"
UNPACKER+=" --log-to-file Unp.out"
nohup /usr/local/bin/MQ/unpacker/$UNPACKER &
else
xterm -geometry 110x23+520+0 -hold -e /usr/local/bin/MQ/unpacker/$UNPACKER &
fi

(( _iUnp -= 1 ))
done

HITBUILDER="HitBuilderTof"
HITBUILDER+=" --id hitbuilder1"
HITBUILDER+=" --channel-config name=tofdigis,type=pull,method=bind,transport=shmem,address=tcp://*:5656"
HITBUILDER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:5005"
HITBUILDER+=" --channel-config name=syscmd,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:5666"
#HITBUILDER+=" --channel-config name=tofhits,type=push,method=bind,transport=shmem,address=tcp://127.0.0.1:5557"
#HITBUILDER+=" --channel-config name=tofcalib,type=push,method=bind,transport=shmem,address=tcp://127.0.0.1:5558"
#HITBUILDER+=" --transport shmem"
#HITBUILDER+=" --verbosity DEBUG"
HITBUILDER+=" --verbosity INFO"
#HITBUILDER+=" --verbosity WARN"
HITBUILDER+=" --OutRootFile $_digifile"
#HITBUILDER+=" --MaxEvent 10000"
HITBUILDER+=" --RunId 1538835959"
#HITBUILDER+=" --RunId 1536669562"
#HITBUILDER+=" --RunId 1537898701"
if  [[ $_batch = 1 ]]; then
HITBUILDER+=" --control static"
HITBUILDER+=" --log-to-file HitBuild.out"
nohup  /usr/local/bin/MQ/hitbuilder/$HITBUILDER &
else
xterm -geometry 120x23+1400+0 -hold -e /usr/local/bin/MQ/hitbuilder/$HITBUILDER &
fi
