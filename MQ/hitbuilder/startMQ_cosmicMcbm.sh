#!/bin/bash
# Copyright (C) 2020 PI-UHd,GSI
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Norbert Herrmann

$FAIRROOTPATH/bin/shmmonitor --cleanup

if [ -z "$1" ]; then 
    _runname="MCBM_cosmic_`date +%F`_`date +%T`"
else 
    _runname=$1
fi

if [ -z "$2" ]; then 
	_version=0 
else
	_version=$2
fi

if [ -z "$3" ]; then 
	_node="node8" 
else
	_node=$3
fi

_reqmod=0
_reqtint=50
_ntimeslices=-1
#_ntimeslices=100
_iUnp=1
_batch=1
_pulmode=0
_pulmulmin=18
_pultotmin=80
_pultotmax=500
_puldetref=12 # TSR=022
#_puldetref=16 # TSR=032
#_puldetref=17 # TSR=032
 
_tofftof=0.  
#_tofftof=-30. 

if [[ $_reqmod -eq 1 ]]; then
  _iUnp=1
fi

_dirname=${VMCWORKDIR}/macro/beamtime/mcbm2021/input/$_runname/
#_outdir=${VMCWORKDIR}/macro/beamtime/hd2020/data/
_outdir=${VMCWORKDIR}/macro/beamtime/mcbm2021/data/
#_datapath=~/KRONOS/CBM/cbmroot/git/cbmroot
_filename=$_runname*.tsa
#_filename=${VMCWORKDIR}/macro/beamtime/hd2020/input/$_runname/*.tsa
#_outdir=$_datapath/macro/beamtime/mcbm2018/data/
 
#_filename="./tsaData/_pn05_*.tsa;./tsaData/_pn06_*.tsa;./tsaData/_pn07_*.tsa"
_digifile=$_runname.$_reqtint.$_reqmod.$_pulmode.$_version.$_node.root

# ASCII files  
_mapfile=${VMCWORKDIR}/macro/beamtime/mcbm2021/mTofCriPar.par	
_digibdffile=${VMCWORKDIR}/parameters/tof/tof_v21f_mcbm.digibdf.par
#_digiparfile=${VMCWORKDIR}/parameters/tof/tof_v21c_mcbm.digi.par

# ROOT files 
#_geofile=${VMCWORKDIR}/macro/beamtime/mcbm2021/tof_mcbm_beam_2021_07.par.root
#_geofile=${VMCWORKDIR}/macro/beamtime/mcbm2021/tof_mcbm_beam_2021_07_surveyed.par.root
_geofile=${VMCWORKDIR}/macro/beamtime/mcbm2021/tof_mcbm_beam_2021_08.par.root

rm -v nohup.out 
rm -v *log
rm all_*

PARAMETERSERVER="parmq-server"
echo pkill $PARAMETERSERVER
pkill -9 $PARAMETERSERVER
sleep 1
PARAMETERSERVER+=" --id parmq-server"
PARAMETERSERVER+=" --channel-name parameters"
PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,rateLogging=0,transport=zeromq,address=tcp://127.0.0.1:5005"
#PARAMETERSERVER+=" --libs-to-load libCbmTof;libCbmFlibMcbm2018"
#PARAMETERSERVER+=" --first-input-name $_mapfile;$_digiparfile;$_digibdffile"
PARAMETERSERVER+=" --first-input-name $_mapfile;$_digibdffile"
PARAMETERSERVER+=" --first-input-type ASCII"
PARAMETERSERVER+=" --second-input-name $_geofile"
PARAMETERSERVER+=" --second-input-type ROOT"
#PARAMETERSERVER+=" --severity INFO"
PARAMETERSERVER+=" --severity DEBUG"
#PARAMETERSERVER+=" --file-severity INFO"
if  [[ $_batch = 1 ]]; then 
PARAMETERSERVER+=" --control static"
PARAMETERSERVER+=" --log-to-file ParServ.out"
nohup ${VMCWORKDIR}/build/bin/MQ/parmq/$PARAMETERSERVER &
else
xterm -geometry 80x23+0+340 -hold -e ${VMCWORKDIR}/build/bin/MQ/parmq/$PARAMETERSERVER &
#xterm -geometry 80x23+500+0 -hold -e /home/cbm/starsoft/fairroot_v18.0.6-fairsoft_may18p1_root6/bin/$PARAMETERSERVER &
fi

SAMPLER="TsaMultiSamplerTof"
SAMPLER+=" --id sampler1"
SAMPLER+=" --max-timeslices $_ntimeslices"
#SAMPLER+=" --max-timeslices 1000000"
#SAMPLER+=" --flib-port 5556"
#SAMPLER+=" --flib-host $_node"
SAMPLER+=" --dirname $_dirname"
SAMPLER+=" --filename $_filename"
SAMPLER+=" --channel-config name=tofcomponent,type=push,method=bind,rateLogging=0,transport=zeromq,address=tcp://*:5655"
SAMPLER+=" --channel-config name=syscmd,type=pub,method=bind,rateLogging=0,transport=zeromq,address=tcp://*:5666"
#SAMPLER+=" --transport shmem"
#SAMPLER+=" --transport zeromq"
#SAMPLER+=" --transport nanomsg"
#SAMPLER+=" --severity WARN"
SAMPLER+=" --severity INFO"
SAMPLER+=" --file-severity INFO"
#SAMPLER+=" --severity DEBUG"
SAMPLER+=" --SelectComponents 0"
if  [[ $_batch = 1 ]]; then 
SAMPLER+=" --log-to-file Sampl.out"
SAMPLER+=" --control static"
nohup  ${VMCWORKDIR}/build/bin/MQ/source/$SAMPLER & 
else 
xterm -geometry 80x23+0+0 -hold -e ${VMCWORKDIR}/build/bin/MQ/source/$SAMPLER &
fi
 
while (( _iUnp > 0 )); do
UNPACKER="UnpackTofCri"
UNPACKER+=" --id unpack$_iUnp"
UNPACKER+=" --channel-config name=tofcomponent,type=pull,method=connect,rateLogging=0,transport=zeromq,address=tcp://127.0.0.1:5655"
UNPACKER+=" --channel-config name=parameters,type=req,method=connect,rateLogging=0,transport=zeromq,address=tcp://127.0.0.1:5005"
UNPACKER+=" --channel-config name=tofdigis,type=push,method=connect,rateLogging=0,transport=zeromq,address=tcp://127.0.0.1:5656"
UNPACKER+=" --channel-config name=syscmd,type=sub,method=connect,rateLogging=0,transport=zeromq,address=tcp://127.0.0.1:5666"
#UNPACKER+=" --transport shmem"
#UNPACKER+=" --severity DEBUG"
#UNPACKER+=" --severity  INFO"
UNPACKER+=" --file-severity info"
UNPACKER+=" --SelectComponents 0"
UNPACKER+=" --ReqMode $_reqmod"
UNPACKER+=" --ReqTint $_reqtint"
UNPACKER+=" --PulserMode $_pulmode"
UNPACKER+=" --PulMulMin $_pulmulmin"
UNPACKER+=" --PulTotMin $_pultotmin"
UNPACKER+=" --PulTotMax $_pultotmax"
UNPACKER+=" --ToffTof $_tofftof"
UNPACKER+=" --RefModType -1"
UNPACKER+=" --RefModId    0"
UNPACKER+=" --RefCtrType  0"
UNPACKER+=" --RefCtrId    0"
if  [[ $_batch = 1 ]]; then 
UNPACKER+=" --control static"
UNPACKER+=" --log-to-file Unp$_iUnp.out"
nohup ${VMCWORKDIR}/build/bin/MQ/unpacker/$UNPACKER &
else 
xterm -geometry 110x23+520+0 -hold -e ${VMCWORKDIR}/build/bin/MQ/unpacker/$UNPACKER & 
fi

(( _iUnp -= 1 ))
done 

HITBUILDER="HitBuilderTof"
HITBUILDER+=" --id hitbuilder1"
HITBUILDER+=" --channel-config name=tofdigis,type=pull,method=bind,rateLogging=0,transport=zeromq,address=tcp://*:5656"
HITBUILDER+=" --channel-config name=parameters,type=req,method=connect,rateLogging=0,transport=zeromq,address=tcp://127.0.0.1:5005"
HITBUILDER+=" --channel-config name=syscmd,type=sub,method=connect,rateLogging=0,transport=zeromq,address=tcp://127.0.0.1:5666"
#HITBUILDER+=" --channel-config name=tofhits,type=push,method=bind,transport=shmem,address=tcp://127.0.0.1:5557"
#HITBUILDER+=" --channel-config name=tofcalib,type=push,method=bind,transport=shmem,address=tcp://127.0.0.1:5558"
#HITBUILDER+=" --transport shmem"
#HITBUILDER+=" --severity DEBUG"
HITBUILDER+=" --severity INFO"
HITBUILDER+=" --file-severity INFO"
#HITBUILDER+=" --severity WARN"
HITBUILDER+=" --OutRootFile $_outdir$_digifile"
#HITBUILDER+=" --MaxEvent 10000000"
#HITBUILDER+=" --RunId 1581312162"   # v20a
#HITBUILDER+=" --RunId 1597162455"   # v20b
#HITBUILDER+=" --RunId 1621902311"  # tof_mcbm_beam_2021_02.par.root
#HITBUILDER+=" --RunId 1629444189"  # tof_mcbm_beam_2021_07.par.root
#HITBUILDER+=" --RunId 1639163969"  # tof_mcbm_beam_2021_07_surveyed.par.root
HITBUILDER+=" --RunId 1647348218"  # tof_mcbm_beam_2021_08.par.root
HITBUILDER+=" --PulserMode $_pulmode"
HITBUILDER+=" --PulMulMin $_pulmulmin"
HITBUILDER+=" --PulTotMin $_pultotmin"
HITBUILDER+=" --PulTotMax $_pultotmax"
HITBUILDER+=" --PulDetRef $_puldetref"
HITBUILDER+=" --ReqTint   $_reqtint"
#HITBUILDER+=" --ReqBeam      20486" # diamond -> 0x00005006
HITBUILDER+=" --BRefType -1"  # -1 = no beam counter
HITBUILDER+=" --BRefSm    0"
HITBUILDER+=" --BRefDet  -1"  # -1 = use ModMask
HITBUILDER+=" --DutType   0"
HITBUILDER+=" --DutSm     1"
HITBUILDER+=" --DutRpc    2"
HITBUILDER+=" --SelType   0"
HITBUILDER+=" --SelSm     2"
HITBUILDER+=" --SelRpc    2"
HITBUILDER+=" --Sel2Type  2"
HITBUILDER+=" --Sel2Sm    0"
HITBUILDER+=" --Sel2Rpc   2"

if  [[ $_batch = 1 ]]; then 
HITBUILDER+=" --control static"
HITBUILDER+=" --log-to-file HitBuild.out"
nohup  ${VMCWORKDIR}/build/bin/MQ/hitbuilder/$HITBUILDER &
else
xterm -geometry 120x23+1400+0 -hold -e ${VMCWORKDIR}/build/bin/MQ/hitbuilder/$HITBUILDER &
fi
