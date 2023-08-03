#!/bin/bash
if [ -e /home/cbmdata/_deps/fairsoft_apr22/install//bin/fairmq-shmmonitor ]; then
  /home/cbmdata/_deps/fairsoft_apr22/install//bin/fairmq-shmmonitor --cleanup
fi

if [ -z "$1" ]; then
    _runname="r0088_20180905_1602"
else
    _runname=$1
fi

_reqmod=3
_reqtint=50
_ntimeslices=-1
_iUnp=4
_batch=1
_pulmode=1
_pulmulmin=9
_pultotmin=50
#_tshiftref=0.
_tshiftref=-35.

#_dirname=/usr/local/share/cbmroot/macro/beamtime/mcbm2018/input/$_runname/
_outdir=/usr/local/share/cbmroot/macro/beamtime/mcbm2018/data/
#_datapath=~/KRONOS/CBM/cbmroot/trunk
#_dirname=$_datapath/macro/beamtime/mcbm2018/input/$_runname/
#_outdir=$_datapath/macro/beamtime/mcbm2018/data/

_filename="./tsaData/_pn05_*.tsa;./tsaData/_pn06_*.tsa;./tsaData/_pn07_*.tsa"
_digifile=$_runname.$_reqtint.$_reqmod.$_pulmode.root

# ASCII files
#_mapfile=/usr/local/share/cbmroot/input/MapTofGbtx.par
_mapfile=/usr/local/share/cbmroot/macro/beamtime/mcbm2018/mTofPar.par
#_mapfile=/usr/local/share/cbmroot/macro/beamtime/mcbm2018/etofPar.par
#_digibdffile=/usr/local/share/cbmroot/parameters/tof/v18j_cosmicHD.digibdf.par
#_digiparfile=/usr/local/share/cbmroot/parameters/tof/tof_v18j_cosmicHD.digi.par
_digibdffile=/usr/local/share/cbmroot/parameters/tof/v18l_mCbm.digibdf.par
_digiparfile=/usr/local/share/cbmroot/parameters/tof/tof_v18l_mCbm.digi.par

# ROOT files
_geofile=/usr/local/share/cbmroot/macro/beamtime/mcbm2018/tof_v18l_mCbm.par.root

rm -v nohup.out
rm -v *log
rm all_*

PARAMETERSERVER="parmq-server"
echo pkill $PARAMETERSERVER
pkill $PARAMETERSERVER
pkill $PARAMETERSERVER
sleep 1
PARAMETERSERVER+=" --id parmq-server"
PARAMETERSERVER+=" --channel-name parameters"
PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:5005"
#PARAMETERSERVER+=" --libs-to-load libCbmTof;libCbmFlibMcbm2018"
PARAMETERSERVER+=" --first-input-name $_mapfile;$_digiparfile;$_digibdffile"
PARAMETERSERVER+=" --first-input-type ASCII"
PARAMETERSERVER+=" --second-input-name $_geofile"
PARAMETERSERVER+=" --second-input-type ROOT"
PARAMETERSERVER+=" --severity INFO"
if  [[ $_batch = 1 ]]; then
PARAMETERSERVER+=" --control static"
PARAMETERSERVER+=" --log-to-file ParServ.out"
nohup /usr/local/bin/MQ/parmq/$PARAMETERSERVER &
else
xterm -geometry 80x23+0+340 -hold -e /usr/local/bin/MQ/parmq/$PARAMETERSERVER &
#xterm -geometry 80x23+500+0 -hold -e /home/cbmdata/_deps/fairroot_v18.6.9/install//bin/$PARAMETERSERVER &
fi

SAMPLER="TsaMultiSamplerTof"
SAMPLER+=" --id sampler1"
SAMPLER+=" --max-timeslices $_ntimeslices"
#SAMPLER+=" --max-timeslices 1000000"
#SAMPLER+=" --flib-port 10"
#SAMPLER+=" --dirname $_dirname"
SAMPLER+=" --filename $_filename"
#SAMPLER+=" --flib-host myHost"
SAMPLER+=" --channel-config name=tofcomponent,type=push,method=bind,transport=zeromq,address=tcp://*:5655"
SAMPLER+=" --channel-config name=syscmd,type=pub,method=bind,transport=zeromq,address=tcp://*:5666"
#SAMPLER+=" --transport shmem"
#SAMPLER+=" --transport zeromq"
#SAMPLER+=" --transport nanomsg"
#SAMPLER+=" --severity WARN"
SAMPLER+=" --severity INFO"
#SAMPLER+=" --severity DEBUG"
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
UNPACKER+=" --channel-config name=tofdigis,type=push,method=connect,transport=zeromq,address=tcp://127.0.0.1:5656"
UNPACKER+=" --channel-config name=syscmd,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:5666"
#UNPACKER+=" --transport shmem"
#UNPACKER+=" --severity DEBUG"
UNPACKER+=" --severity  INFO"
UNPACKER+=" --ReqMode $_reqmod"
UNPACKER+=" --ReqTint $_reqtint"
UNPACKER+=" --PulserMode $_pulmode"
UNPACKER+=" --PulMulMin $_pulmulmin"
UNPACKER+=" --PulTotMin $_pultotmin"
UNPACKER+=" --TShiftRef $_tshiftref"
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
HITBUILDER+=" --channel-config name=tofdigis,type=pull,method=bind,transport=zeromq,address=tcp://*:5656"
HITBUILDER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:5005"
HITBUILDER+=" --channel-config name=syscmd,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:5666"
#HITBUILDER+=" --channel-config name=tofhits,type=push,method=bind,transport=shmem,address=tcp://127.0.0.1:5557"
#HITBUILDER+=" --channel-config name=tofcalib,type=push,method=bind,transport=shmem,address=tcp://127.0.0.1:5558"
#HITBUILDER+=" --transport shmem"
#HITBUILDER+=" --severity DEBUG"
HITBUILDER+=" --severity INFO"
#HITBUILDER+=" --severity WARN"
HITBUILDER+=" --OutRootFile $_outdir$_digifile"
#HITBUILDER+=" --MaxEvent 10000000"
HITBUILDER+=" --RunId 1552992251"
HITBUILDER+=" --PulserMode $_pulmode"
HITBUILDER+=" --PulMulMin $_pulmulmin"
HITBUILDER+=" --PulTotMin $_pultotmin"
HITBUILDER+=" --BRefType  5"
HITBUILDER+=" --BRefSm    0"
HITBUILDER+=" --BRefDet   0"
HITBUILDER+=" --DutType   0"
HITBUILDER+=" --DutSm     0"
HITBUILDER+=" --DutRpc    2"
HITBUILDER+=" --SelType   0"
HITBUILDER+=" --SelSm     1"
HITBUILDER+=" --SelRpc    2"
HITBUILDER+=" --Sel2Type  0"
HITBUILDER+=" --Sel2Sm    2"
HITBUILDER+=" --Sel2Rpc   2"

if  [[ $_batch = 1 ]]; then
HITBUILDER+=" --control static"
HITBUILDER+=" --log-to-file HitBuild.out"
nohup  /usr/local/bin/MQ/hitbuilder/$HITBUILDER &
else
xterm -geometry 120x23+1400+0 -hold -e /usr/local/bin/MQ/hitbuilder/$HITBUILDER &
fi
