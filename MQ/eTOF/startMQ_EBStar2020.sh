#!/bin/bash
# Copyright (C) 2019 PI-UHd,GSI
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Norbert Herrmann

# script to write cosmic data to file  
$FAIRROOTPATH/bin/shmmonitor --cleanup

if [ -z "$1" ]; then 
    _runname="r0010_20191027_1534"
else 
    _runname=$1
fi

_ntimeslices=-1
#_ntimeslices=1000000 
_iEB=1   
_batch=1

PortN=2

CbmDir=/home/cbm/software2020/cbmroot_trunk
MQDir=$CbmDir/build/bin/MQ
MQSourceDir=$CbmDir/build/bin/MQ/source
WDir=$CbmDir/macro/beamtime/star2019

_dirname=/data/2020/wheel/$_runname/
_filename=$_runname*.tsa
_outdir=$WDir/data/
#_datapath=~/KRONOS/CBM/cbmroot/trunk
#_dirname=$_datapath/macro/beamtime/mcbm2018/input/$_runname/
#_outdir=$_datapath/macro/beamtime/mcbm2018/data/
#_filename="./tsaData/${_runname}_pn05_*.tsa;./tsaData/${_runname}_pn06_*.tsa;./tsaData/${_runname}_pn07_*.tsa"
#_filename="./tsaData/${_runname}_pn02_*.tsa;./tsaData/${_runname}_pn04_*.tsa;./tsaData/${_runname}_pn05_*.tsa;./tsaData/${_runname}_pn06_*.tsa;./tsaData/${_runname}_pn07_*.tsa;./tsaData/${_runname}_pn08_*.tsa;./tsaData/${_runname}_pn10_*.tsa"

# ASCII files  
_mapfile=$WDir/etofEvtBuildPar.par
_digibdffile=$CbmDir/parameters/tof/Star_2.digibdf.par
_digiparfile=$CbmDir/parameters/tof/tof_Star_2.digi.par

# ROOT files 
_geofile=$WDir/tof_Star_2.par.root

# MQ ports
_pPar=510$PortN
_pSam=566$PortN
_pCmd=567$PortN
_pDig=568$PortN

rm -v nohup.out 
rm -v *log
rm all_*

PARAMETERSERVER="parmq-server"
echo pkill $PARAMETERSERVER
pkill -9 $PARAMETERSERVER
pkill -9 $PARAMETERSERVER
sleep 1
PARAMETERSERVER+=" --id parmq-server"
PARAMETERSERVER+=" --channel-name parameters"
PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:$_pPar"
#PARAMETERSERVER+=" --libs-to-load libCbmTof;libCbmFlibMcbm2018"
PARAMETERSERVER+=" --first-input-name $_mapfile;$_digiparfile;$_digibdffile"
PARAMETERSERVER+=" --first-input-type ASCII"
PARAMETERSERVER+=" --second-input-name $_geofile"
PARAMETERSERVER+=" --second-input-type ROOT"
PARAMETERSERVER+=" --severity INFO"
if  [[ $_batch = 1 ]]; then 
PARAMETERSERVER+=" --control static"
PARAMETERSERVER+=" --log-to-file ParServ.out"
nohup $MQDir/parmq/$PARAMETERSERVER &
else
xterm -geometry 80x23+0+340 -hold -e $MQDir/parmq/$PARAMETERSERVER &
#xterm -geometry 80x23+500+0 -hold -e /cvmfs/fairroot.gsi.de/fairroot/v18.0.6_fairsoft-may18/bin/$PARAMETERSERVER &
fi

SAMPLER="TsaMultiSamplerTof"
SAMPLER+=" --id sampler1"
SAMPLER+=" --max-timeslices $_ntimeslices"
#SAMPLER+=" --flib-port 10"
SAMPLER+=" --dirname $_dirname"
SAMPLER+=" --filename $_filename"
#SAMPLER+=" --flib-host myHost"
SAMPLER+=" --channel-config name=tofcomponent,type=push,method=bind,transport=zeromq,address=tcp://*:$_pSam"
SAMPLER+=" --channel-config name=syscmd,type=pub,method=bind,transport=zeromq,address=tcp://*:$_pCmd"
#SAMPLER+=" --transport shmem"
#SAMPLER+=" --transport zeromq"
#SAMPLER+=" --transport nanomsg"
#SAMPLER+=" --severity WARN"
SAMPLER+=" --severity INFO"
#SAMPLER+=" --severity DEBUG"
SAMPLER+=" --SelectComponents 0"
if  [[ $_batch = 1 ]]; then 
SAMPLER+=" --log-to-file Sampl.out"
SAMPLER+=" --control static"
echo $MQDir/source/$SAMPLER 
nohup  $MQDir/source/$SAMPLER &> Tsa.out & 
else 
xterm -geometry 80x23+0+0 -hold -e $MQDir/source/$SAMPLER &
fi
 
while (( _iEB > 0 )); do

EVENTBUILDER="EventBuilderEtofStar2019"
EVENTBUILDER+=" --id eventbuilder$_iEB"
EVENTBUILDER+=" --channel-config name=tofcomponent,type=pull,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_pSam"
EVENTBUILDER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_pPar"
EVENTBUILDER+=" --channel-config name=etofevts,type=push,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_pDig"
EVENTBUILDER+=" --channel-config name=syscmd,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_pCmd"
#EVENTBUILDER+=" --transport shmem"
#EVENTBUILDER+=" --severity DEBUG"
EVENTBUILDER+=" --severity  INFO"

if  [[ $_batch = 1 ]]; then 
EVENTBUILDER+=" --control static"
EVENTBUILDER+=" --log-to-file EB$_iEB.out"
#echo nohup $EVENTBUILDER 
nohup $MQDir/eTOF/$EVENTBUILDER &> EventBuilder.out &
else 
xterm -geometry 110x23+520+0 -hold -e $MQDir/eTOF/$EVENTBUILDER & 
fi

(( _iEB -= 1 ))
done 

TRIGGERHANDLER="TriggerHandlerEtof"
TRIGGERHANDLER+=" --id triggerhandler1"
TRIGGERHANDLER+=" --channel-config name=etofevts,type=pull,method=bind,transport=zeromq,address=tcp://*:$_pDig"
TRIGGERHANDLER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_pPar"
TRIGGERHANDLER+=" --channel-config name=syscmd,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_pCmd"
#TRIGGERHANDLER+=" --channel-config name=tofhits,type=push,method=bind,transport=shmem,address=tcp://127.0.0.1:5557"
#TRIGGERHANDLER+=" --channel-config name=tofcalib,type=push,method=bind,transport=shmem,address=tcp://127.0.0.1:5558"
#TRIGGERHANDLER+=" --transport shmem"
#TRIGGERHANDLER+=" --severity DEBUG"
TRIGGERHANDLER+=" --severity INFO"
#TRIGGERHANDLER+=" --severity WARN"

if  [[ $_batch = 1 ]]; then 
TRIGGERHANDLER+=" --control static"
TRIGGERHANDLER+=" --log-to-file TH"
nohup  $MQDir/eTOF/$TRIGGERHANDLER &> TrigHandler.out &
else
xterm -geometry 120x23+1400+0 -hold -e $MQDir/eTOF/$TRIGGERHANDLER &
fi
