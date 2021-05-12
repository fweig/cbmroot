#!/bin/bash
# Copyright (C) 2019 PI-UHd,GSI
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Norbert Herrmann

# script to write cosmic data to file  
$FAIRROOTPATH/bin/shmmonitor --cleanup

if [ -z "$1" ]; then 
    _runname="r0088_20180905_1602"
else 
    _runname=$1
fi
  
_reqmod=0
_reqtint=50
_ntimeslices=-1 
#_ntimeslices=100000 
_iUnp=1   
_batch=1
_pulmode=0 
_pulmulmin=2
_pultotmin=50

_tshiftref=0.  
#_tshiftref=-35. 

CbmDir=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk
MQDir=$CbmDir/build/bin/MQ
MQSourceDir=$CbmDir/build/bin/MQ/source
WDir=$CbmDir/macro/beamtime/mcbm2018

#_dirname=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018/input/$_runname/
_outdir=$WDir/data/
#_datapath=~/KRONOS/CBM/cbmroot/trunk
#_dirname=$_datapath/macro/beamtime/mcbm2018/input/$_runname/
#_outdir=$_datapath/macro/beamtime/mcbm2018/data/
 
#_filename="./tsaData/${_runname}_pn05_*.tsa;./tsaData/${_runname}_pn06_*.tsa;./tsaData/${_runname}_pn07_*.tsa"
_filename="./tsaData/${_runname}_pn02_*.tsa;./tsaData/${_runname}_pn04_*.tsa;./tsaData/${_runname}_pn05_*.tsa;./tsaData/${_runname}_pn06_*.tsa;./tsaData/${_runname}_pn07_*.tsa;./tsaData/${_runname}_pn08_*.tsa;./tsaData/${_runname}_pn10_*.tsa"
_digifile=$_runname.$_reqtint.$_reqmod.$_pulmode.root

# ASCII files  
#_mapfile=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/MQ/hitbuilder/MapTofGbtx.par
_mapfile=$WDir/mTofParAll.par
#_mapfile=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018/etofPar.par
#_digibdffile=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/parameters/tof/v18j_cosmicHD.digibdf.par
#_digiparfile=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/parameters/tof/tof_v18j_cosmicHD.digi.par
_digibdffile=$CbmDir/parameters/tof/v18l_mCbm.digibdf.par
_digiparfile=$CbmDir/parameters/tof/tof_v18l_mCbm.digi.par

# ROOT files 
_geofile=$WDir/tof_v18l_mCbm.par.root

# MQ ports
_pPar=5102
_pSam=5662
_pCmd=5672
_pDig=5682

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
#SAMPLER+=" --max-timeslices 1000000"
SAMPLER+=" --flib-port 10"
#SAMPLER+=" --dirname $_dirname"
#SAMPLER+=" --filename $_filename"
#SAMPLER+=" --flib-host myHost"
SAMPLER+=" --channel-config name=tofcomponent,type=push,method=bind,transport=zeromq,address=tcp://*:$_pSam"
SAMPLER+=" --channel-config name=syscmd,type=pub,method=bind,transport=zeromq,address=tcp://*:$_pCmd"
#SAMPLER+=" --transport shmem"
#SAMPLER+=" --transport zeromq"
#SAMPLER+=" --transport nanomsg"
#SAMPLER+=" --severity WARN"
SAMPLER+=" --severity INFO"
#SAMPLER+=" --severity DEBUG"
if  [[ $_batch = 1 ]]; then 
SAMPLER+=" --log-to-file Sampl.out"
SAMPLER+=" --control static"
nohup  $MQDir/source/$SAMPLER & 
else 
xterm -geometry 80x23+0+0 -hold -e $MQDir/source/$SAMPLER &
fi
 
while (( _iUnp > 0 )); do

UNPACKER="UnpackTofMcbm2018"
UNPACKER+=" --id unpack$_iUnp"
UNPACKER+=" --channel-config name=tofcomponent,type=pull,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_pSam"
UNPACKER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_pPar"
UNPACKER+=" --channel-config name=tofdigis,type=push,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_pDig"
UNPACKER+=" --channel-config name=syscmd,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_pCmd"
#UNPACKER+=" --transport shmem"
#UNPACKER+=" --severity DEBUG"
UNPACKER+=" --severity  INFO"
if  [[ $_reqmod -lt 1 ]]; then
    UNPACKER+=" --ReqMode 0"
    case $_reqmod in
	0)
	    ;;
       -1)
	    UNPACKER+=" --ReqDet0       20486" # diamond -> 0x00005006
	    UNPACKER+=" --ReqDet1       65590" # RPC 031 -> 0x00010036
	    UNPACKER+=" --ReqDet2       65606" # RPC 041
	    ;;
       -2) 
	    UNPACKER+=" --ReqDet0       20486" # diamond
	    UNPACKER+=" --ReqDet1      131126" # RPC 032
	    UNPACKER+=" --ReqDet2      131142" # RPC 042
	    ;; # for ceramics

       -3)
	    UNPACKER+=" --ReqDet0       20486" # diamond
	    UNPACKER+=" --ReqDet1      196662" # RPC 033
	    UNPACKER+=" --ReqDet2      196678" # RPC 043
	    ;; # for BUC

       *)
	    echo ReqMode $_reqmod not yet defined, exiting
	    exit 1
	    ;;
    esac;
else
    UNPACKER+=" --ReqMode $_reqmod"
fi
UNPACKER+=" --ReqTint $_reqtint"
UNPACKER+=" --PulserMode $_pulmode"
UNPACKER+=" --PulMulMin $_pulmulmin"
UNPACKER+=" --PulTotMin $_pultotmin"
UNPACKER+=" --TShiftRef $_tshiftref"
if  [[ $_batch = 1 ]]; then 
UNPACKER+=" --control static"
UNPACKER+=" --log-to-file Unp$_iUnp.out"
#echo nohup $UNPACKER 
nohup $MQDir/unpacker/$UNPACKER &
else 
xterm -geometry 110x23+520+0 -hold -e $MQDir/unpacker/$UNPACKER & 
fi

(( _iUnp -= 1 ))
done 

HITBUILDER="HitBuilderTof"
HITBUILDER+=" --id hitbuilder1"
HITBUILDER+=" --channel-config name=tofdigis,type=pull,method=bind,transport=zeromq,address=tcp://*:$_pDig"
HITBUILDER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_pPar"
HITBUILDER+=" --channel-config name=syscmd,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_pCmd"
#HITBUILDER+=" --channel-config name=tofhits,type=push,method=bind,transport=shmem,address=tcp://127.0.0.1:5557"
#HITBUILDER+=" --channel-config name=tofcalib,type=push,method=bind,transport=shmem,address=tcp://127.0.0.1:5558"
#HITBUILDER+=" --transport shmem"
#HITBUILDER+=" --severity DEBUG"
HITBUILDER+=" --severity INFO"
#HITBUILDER+=" --severity WARN"
HITBUILDER+=" --OutRootFile $_outdir$_digifile"
#HITBUILDER+=" --MaxEvent 10000000"
HITBUILDER+=" --RunId 1554616781"
HITBUILDER+=" --PulserMode $_pulmode"
HITBUILDER+=" --PulMulMin $_pulmulmin"
HITBUILDER+=" --PulTotMin $_pultotmin"
HITBUILDER+=" --BRefType  5"
HITBUILDER+=" --BRefSm    0"
HITBUILDER+=" --BRefDet   0"
HITBUILDER+=" --DutType   0"
HITBUILDER+=" --DutSm     3"
HITBUILDER+=" --DutRpc    1"
HITBUILDER+=" --SelType   0"
HITBUILDER+=" --SelSm     4"
HITBUILDER+=" --SelRpc    1"
HITBUILDER+=" --Sel2Type  5"
HITBUILDER+=" --Sel2Sm    0"
HITBUILDER+=" --Sel2Rpc   0"

if  [[ $_batch = 1 ]]; then 
HITBUILDER+=" --control static"
HITBUILDER+=" --log-to-file HitBuild.out"
nohup  $MQDir/hitbuilder/$HITBUILDER &
else
xterm -geometry 120x23+1400+0 -hold -e $MQDir/hitbuilder/$HITBUILDER &
fi
