#!/bin/bash
$FAIRROOTPATH/bin/shmmonitor --cleanup

if [ -z "$1" ]; then 
    _runname="359"
else 
    _runname=$1
fi
 
if [ -z "$2" ]; then 
_reqmod=-193
else 
_reqmod=$2
fi

if [ -z "$3" ]; then 
_pulmode=1
else 
_pulmode=$3
fi

if [ -z "$4" ]; then 
    _reqtint=100
else
    _reqtint=$4 
fi

_Opt=$5

_ntimeslices=-1
#_ntimeslices=10000
_iUnp=1
_batch=1
_pulmulmin=5
_pultotmin=50
_pultotmax=500
#_puldetref=12 # TSR=022
_puldetref=16 # TSR=032

#_tofftof=0.  
_tofftof=-30. 

if [[ $_reqmod -eq 1 ]]; then
  _iUnp=1
fi

#_dirname=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018/input/$_runname/
_outdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021/data/
#_datapath=~/KRONOS/CBM/cbmroot/trunk
#_dirname=$_datapath/macro/beamtime/mcbm2018/input/$_runname/
#_outdir=$_datapath/macro/beamtime/mcbm2018/data/
 
#_filename="./tsaData/${_runname}_pn05_*.tsa;./tsaData/${_runname}_pn06_*.tsa;./tsaData/${_runname}_pn07_*.tsa"
#_filename="./tsaData2020/${_runname}_pn02_*.tsa;./tsaData2020/${_runname}_pn04_*.tsa;./tsaData2020/${_runname}_pn05_*.tsa;./tsaData2020/${_runname}_pn06_*.tsa;./tsaData2020/${_runname}_pn08_*.tsa;./tsaData2020/${_runname}_pn10_*.tsa;./tsaData2020/${_runname}_pn11_*.tsa;./tsaData2020/${_runname}_pn12_*.tsa;./tsaData2020/${_runname}_pn13_*.tsa;./tsaData2020/${_runname}_pn15_*.tsa"
_filename="./tsaData2021/${_runname}_node8_*.tsa;./tsaData2021/${_runname}_node9_*.tsa"
_digifile=$_runname.$_reqtint.$_reqmod.${_pulmode}${_Opt}.root

# ASCII files  
#_mapfile=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/MQ/hitbuilder/MapTofGbtx.par
_mapfile=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021/mTofPar.par
if [ $_runname -ge 707 ] && [ $_runname -lt 754 ]; then 
  _mapfile=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2020/mTofPar_2Stack.par
else
  if [ $_runname -ge 754 ] && [ $_runname -lt 900 ]; then 
    _mapfile=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2020/mTofPar_3Stack.par
    _puldetref=12 # TSR=022
  else 
	if [ $_runname -ge 1079 ] && [ $_runname -lt 10000 ]; then 
	  _mapfile=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021/mTofPar2Stack.par
    fi
  fi
fi 
#_mapfile=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018/etofPar.par
#_digibdffile=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/parameters/tof/v18j_cosmicHD.digibdf.par
#_digiparfile=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/parameters/tof/tof_v18j_cosmicHD.digi.par
_digibdffile=/lustre/cbm/users/nh/CBM/cbmroot/trunk/parameters/tof/tof_v21a_mcbm.digibdf.par
#_digiparfile=/lustre/cbm/users/nh/CBM/cbmroot/trunk/parameters/tof/tof_v19b_mcbm.digi.par

# ROOT files 
#_geofile=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018/tof_v18l_mCbm.par.root
_geofile=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021/tof_mcbm_beam_2021_01.par.root

# MQ ports
_pPar=5603
_pSam=5613
_pCmd=5623
_pDig=5633

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
PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,rateLogging=0,transport=zeromq,address=tcp://127.0.0.1:$_pPar"
#PARAMETERSERVER+=" --libs-to-load libCbmTof;libCbmFlibMcbm2018"
#PARAMETERSERVER+=" --first-input-name $_mapfile;$_digiparfile;$_digibdffile"
PARAMETERSERVER+=" --first-input-name $_mapfile;$_digibdffile"
PARAMETERSERVER+=" --first-input-type ASCII"
PARAMETERSERVER+=" --second-input-name $_geofile"
PARAMETERSERVER+=" --second-input-type ROOT"
PARAMETERSERVER+=" --severity INFO"
if  [[ $_batch = 1 ]]; then 
PARAMETERSERVER+=" --control static"
PARAMETERSERVER+=" --log-to-file ParServ.out"
nohup /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/build/bin/MQ/parmq/$PARAMETERSERVER &
else
xterm -geometry 80x23+0+340 -hold -e /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/build/bin/MQ/parmq/$PARAMETERSERVER &
#xterm -geometry 80x23+500+0 -hold -e /cvmfs/fairroot.gsi.de/fairroot/v18.0.6_fairsoft-may18/bin/$PARAMETERSERVER &
fi

SAMPLER="TsaMultiSamplerTof"
SAMPLER+=" --id sampler1"
SAMPLER+=" --max-timeslices $_ntimeslices"
#SAMPLER+=" --max-timeslices 1000000"
#SAMPLER+=" --flib-port 10"
#SAMPLER+=" --dirname $_dirname"
SAMPLER+=" --filename $_filename"
#SAMPLER+=" --flib-host myHost"
SAMPLER+=" --channel-config name=tofcomponent,type=push,method=bind,rateLogging=0,transport=zeromq,address=tcp://*:$_pSam"
SAMPLER+=" --channel-config name=syscmd,type=pub,method=bind,rateLogging=0,transport=zeromq,address=tcp://*:$_pCmd"
#SAMPLER+=" --transport shmem"
#SAMPLER+=" --transport zeromq"
#SAMPLER+=" --transport nanomsg"
#SAMPLER+=" --severity WARN"
SAMPLER+=" --severity INFO"
#SAMPLER+=" --severity DEBUG"
SAMPLER+=" --SelectComponents 1"
if  [[ $_batch = 1 ]]; then 
SAMPLER+=" --log-to-file Sampl.out"
SAMPLER+=" --control static"
nohup  /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/build/bin/MQ/source/$SAMPLER & 
else 
xterm -geometry 80x23+0+0 -hold -e /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/build/bin/MQ/source/$SAMPLER &
fi
 
while (( _iUnp > 0 )); do

UNPACKER="UnpackTofMcbm2018"
UNPACKER+=" --id unpack$_iUnp"
UNPACKER+=" --channel-config name=tofcomponent,type=pull,method=connect,rateLogging=0,transport=zeromq,address=tcp://127.0.0.1:$_pSam"
UNPACKER+=" --channel-config name=parameters,type=req,method=connect,rateLogging=0,transport=zeromq,address=tcp://127.0.0.1:$_pPar"
UNPACKER+=" --channel-config name=tofdigis,type=push,method=connect,rateLogging=0,transport=zeromq,address=tcp://127.0.0.1:$_pDig"
UNPACKER+=" --channel-config name=syscmd,type=sub,method=connect,rateLogging=0,transport=zeromq,address=tcp://127.0.0.1:$_pCmd"
#UNPACKER+=" --transport shmem"
#UNPACKER+=" --severity DEBUG"
UNPACKER+=" --severity  INFO" 
UNPACKER+=" --SelectComponents 1"
#UNPACKER+=" --ReqBeam      20486" # diamond -> 0x00005006 v14a
#UNPACKER+=" --ReqBeam      10246" # diamond -> 0x00002806 v21a 
if  [[ $_reqmod -lt 1 ]]; then
    UNPACKER+=" --ReqMode 0"
    case $_reqmod in
	0)
	    ;;
       -1)
	    UNPACKER+=" --ReqDet0       10246" # diamond -> 0x00002806 v21a
	    UNPACKER+=" --ReqDet1       32822" # RPC 031 -> 0x00008036 v21a
	    UNPACKER+=" --ReqDet2       32838" # RPC 041 -> 0x00008046 v21a
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
	    
       -4) # v21a address mode
	    UNPACKER+=" --ReqDet0       10246" # diamond
	    UNPACKER+=" --ReqDet1       65542" # RPC 002       
	    UNPACKER+=" --ReqDet2       65574" # RPC 022       
	    ;;
       
       -190) 
	    UNPACKER+=" --ReqDet0       20486"  # diamond
	    UNPACKER+=" --ReqDet1       65606"  # RPC 041
	    UNPACKER+=" --ReqDet2       36870"  # RPC 900
	    UNPACKER+=" --ReqDet3      102406"  # RPC 901
	    ;; # for  double stack calibration
       
       -191) 
	    UNPACKER+=" --ReqDet0       20486" # diamond
	    UNPACKER+=" --ReqDet1       65606"  # RPC 041
	    UNPACKER+=" --ReqDet2       24582"  # RPC 600
	    UNPACKER+=" --ReqDet3       90118"   # RPC 601
	    ;; # for USTC counter analysis

       -192) 
	    UNPACKER+=" --ReqDet0       65606"  # RPC 041
	    UNPACKER+=" --ReqDet1       36870"  # RPC 900
	    UNPACKER+=" --ReqDet2     102406"   # RPC 901
	    ;; # for  double stack calibration

       -193) BeamBeam
	    UNPACKER+=" --ReqDet0       65606"  # RPC 041
	    UNPACKER+=" --ReqDet1       24582"  # RPC 600
	    UNPACKER+=" --ReqDet2       90118"  # RPC 601
	    ;; # for USTC counter analysis

       -194) 
	    UNPACKER+=" --ReqDet0       20486"  # diamond
	    UNPACKER+=" --ReqDet1       24582"  # RPC 600
	    UNPACKER+=" --ReqDet2       90118"  # RPC 601
	    ;; # for USTC counter analysis
	    
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
UNPACKER+=" --PulTotMax $_pultotmax"
UNPACKER+=" --ToffTof $_tofftof"
UNPACKER+=" --RefModType    5"
UNPACKER+=" --RefModId      0"
UNPACKER+=" --RefCtrType    0"
UNPACKER+=" --RefCtrId      0"
if  [[ $_batch = 1 ]]; then 
UNPACKER+=" --control static"
UNPACKER+=" --log-to-file Unp$_iUnp.out"
#echo nohup $UNPACKER 
nohup /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/build/bin/MQ/unpacker/$UNPACKER &
else 
xterm -geometry 110x23+520+0 -hold -e /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/build/bin/MQ/unpacker/$UNPACKER & 
fi

(( _iUnp -= 1 ))
done 

HITBUILDER="HitBuilderTof"
HITBUILDER+=" --id hitbuilder1"
HITBUILDER+=" --channel-config name=tofdigis,type=pull,method=bind,rateLogging=0,transport=zeromq,address=tcp://*:$_pDig"
HITBUILDER+=" --channel-config name=parameters,type=req,method=connect,rateLogging=0,transport=zeromq,address=tcp://127.0.0.1:$_pPar"
HITBUILDER+=" --channel-config name=syscmd,type=sub,method=connect,rateLogging=0,transport=zeromq,address=tcp://127.0.0.1:$_pCmd"
#HITBUILDER+=" --channel-config name=tofhits,type=push,method=bind,transport=shmem,address=tcp://127.0.0.1:5557"
#HITBUILDER+=" --channel-config name=tofcalib,type=push,method=bind,transport=shmem,address=tcp://127.0.0.1:5558"
#HITBUILDER+=" --transport shmem"
#HITBUILDER+=" --severity DEBUG"
HITBUILDER+=" --severity INFO"
#HITBUILDER+=" --severity WARN"
HITBUILDER+=" --OutRootFile $_outdir$_digifile"
#HITBUILDER+=" --MaxEvent 10000000"
#HITBUILDER+=" --RunId 1552883952"
HITBUILDER+=" --RunId 1614415472"
HITBUILDER+=" --PulserMode $_pulmode"
HITBUILDER+=" --PulMulMin $_pulmulmin"
HITBUILDER+=" --PulTotMin $_pultotmin"
HITBUILDER+=" --PulTotMax $_pultotmax"
HITBUILDER+=" --PulDetRef $_puldetref"
HITBUILDER+=" --ReqTint $_reqtint"
#HITBUILDER+=" --ReqBeam      20486" # diamond -> 0x00005006
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
if [[ $_reqmod -eq 1 ]]; then
HITBUILDER+=" --Mode   1"
fi

if  [[ $_batch = 1 ]]; then 
HITBUILDER+=" --control static"
HITBUILDER+=" --log-to-file HitBuild.out"
nohup  /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/build/bin/MQ/hitbuilder/$HITBUILDER &
else
xterm -geometry 120x23+1400+0 -hold -e /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/build/bin/MQ/hitbuilder/$HITBUILDER &
fi
