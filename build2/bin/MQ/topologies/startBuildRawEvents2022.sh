#!/bin/bash

if [ -e /home/cbmdata/_deps/fairsoft_apr22/install//bin/fairmq-shmmonitor ]; then
  /home/cbmdata/_deps/fairsoft_apr22/install//bin/fairmq-shmmonitor --cleanup
fi

if [ $# -ge 3 ]; then
  _nbbranch=$1
  _TriggSet=$2
  _run_id=$3
  ((_pubfreqts = $_nbbranch*100 ))
  _pubminsec=1.0
  _pubmaxsec=10.0

  if [ $# -ge 6 ]; then
    _filename=""
    _dirname=""
    _hostname=$6

    if [ $# -ge 7 ]; then
      _pubfreqts=$7

      if [ $# -ge 8 ]; then
        _pubminsec=$8

        if [ $# -ge 9 ]; then
          _pubmaxsec=$9
        fi
      fi
    fi
  elif [ $# -ge 4 ]; then
    _filename=$4
    _hostname=""
    if [ $# -eq 5 ]; then
      _dirname=$5
    else
      _dirname=""
    fi
  else
    echo 'Missing parameters or wrong number of parameters.'
    echo 'Possible usages are:'
    echo 'startBuildRawEvents2022.sh <Nb // branches> <Trigger set> <Run Id> <full filename pattern list>'
    echo 'startBuildRawEvents2022.sh <Nb // branches> <Trigger set> <Run Id> <filename pattern> <folder_path>'
    echo 'startBuildRawEvents2022.sh <Nb // branches> <Trigger set> <Run Id> "" "" <hostname(s) list>'
    echo 'startBuildRawEvents2022.sh <Nb // branches> <Trigger set> <Run Id> "" "" <hostname(s) list> <Hist publish freq. in TS>'
    echo 'startBuildRawEvents2022.sh <Nb // branches> <Trigger set> <Run Id> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s>'
    echo 'startBuildRawEvents2022.sh <Nb // branches> <Trigger set> <Run Id> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s> <Max Hist pub. in s>'

    return -1
  fi
else
  echo 'Missing parameters. At least the number of branches and the trigger set are required'
  echo 'Possible usages are:'
  echo 'startBuildRawEvents2022.sh <Nb // branches> <Trigger set> <Run Id> <full filename pattern list>'
  echo 'startBuildRawEvents2022.sh <Nb // branches> <Trigger set> <Run Id> <filename pattern> <folder_path>'
  echo 'startBuildRawEvents2022.sh <Nb // branches> <Trigger set> <Run Id> "" "" <hostname(s) list>'
  echo 'startBuildRawEvents2022.sh <Nb // branches> <Trigger set> <Run Id> "" "" <hostname(s) list> <Hist publish freq. in TS>'
  echo 'startBuildRawEvents2022.sh <Nb // branches> <Trigger set> <Run Id> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s>'
  echo 'startBuildRawEvents2022.sh <Nb // branches> <Trigger set> <Run Id> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s> <Max Hist pub. in s>'

  return -1
fi

########################################################################################################################

# Setup file and parameter files for parameter server
_parfileSts=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mStsPar.par
_parfileMuch=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mMuchPar.par
_parfileTrdAsic=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22d_mcbm.asic.par
_parfileTrdDigi=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22d_mcbm.digi.par
_parfileTrdGas=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22d_mcbm.gas.par
_parfileTrdGain=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22d_mcbm.gain.par
_parfileTof=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mTofCriPar.par
_parfileBmon=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mBmonCriPar.par
_parfileRich=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2021/mRichPar_70.par
_parfilePsd=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2021/mPsdPar.par
_setup_name=mcbm_beam_2022_03_22_iron

# Parameter files => Update depending on run ID!!!
# --> Setup
if [ $_run_id -ge 2060 ]; then
  if [ $_run_id -le 2065 ]; then
    _setup_name=mcbm_beam_2022_03_09_carbon
  elif [ $_run_id -le 2160 ]; then # Potentially wrong setup between 2065 and 2150 but not official runs
    _setup_name=mcbm_beam_2022_03_22_iron
  elif [ $_run_id -le 2310 ]; then # Potentially wrong setup between 2160 and 2176 but not official runs
    _setup_name=mcbm_beam_2022_03_28_uranium
    _parfileTrdAsic=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22g_mcbm.asic.par
    _parfileTrdDigi=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22g_mcbm.digi.par
    _parfileTrdGas=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22g_mcbm.gas.par
    _parfileTrdGain=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22g_mcbm.gain.par
  elif [ $_run_id -le 2397 ]; then # Potentially wrong setup between 2310 and 2350 but not official runs
    _setup_name=mcbm_beam_2022_05_23_nickel
    _parfileTrdAsic=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22h_mcbm.asic.par
    _parfileTrdDigi=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22h_mcbm.digi.par
    _parfileTrdGas=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22h_mcbm.gas.par
    _parfileTrdGain=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22h_mcbm.gain.par
  elif [ $_run_id -le 2497 ]; then # Potentially wrong setup between 2397 and 2454 but not official runs
    _setup_name=mcbm_beam_2022_06_16_gold
    _parfileTrdAsic=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22h_mcbm.asic.par
    _parfileTrdDigi=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22h_mcbm.digi.par
    _parfileTrdGas=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22h_mcbm.gas.par
    _parfileTrdGain=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22h_mcbm.gain.par
  fi
fi
# --> mMUCH
if [ $_run_id -ge 2060 ]; then
  if [ $_run_id -le 2162 ]; then
    _parfileMuch=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mMuchParUpto26032022.par # Valid for runs 2060-2162
  elif [ $_run_id -le 2291 ]; then
    _parfileMuch=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mMuchParUpto03042022.par # Valid for runs 2163-2291
  elif [ $_run_id -le 2315 ]; then
    _parfileMuch=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mMuchParUpto10042022.par # Valid for runs 2292-2315
  elif [ $_run_id -le 2366 ]; then
    _parfileMuch=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mMuchParUpto23052022.par # Valid for runs 2316-2366
  elif [ $_run_id -le 2397 ]; then
    _parfileMuch=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mMuchParUpto26052022.par # Valid for runs 2060-2162
  else
    _parfileMuch=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mMuchPar.par # Valid for runs 2398 and higher until further notice
  fi
fi
# --> mTOF
if [ $_run_id -ge 2060 ]; then
  if [ $_run_id -le 2065 ]; then
    _parfileTof=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mTofCriParCarbon.par # Valid for runs 2060-2065
  elif [ $_run_id -le 2160 ]; then # Potentially wrong setup between 2065 and 2150 but not official runs
    _parfileTof=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mTofCriParIron.par # Valid for runs 2150-2160
  elif [ $_run_id -le 2310 ]; then # Potentially wrong setup between 2160 and 2176 but not official runs
    _parfileTof=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mTofCriParUranium.par # Valid for runs 2176-2310
  elif [ $_run_id -le 2497 ]; then # Potentially wrong setup between 2310 and 2335 but not official runs
     # Nickel runs: 2335 - 2397
     # Gold runs: 2400 - 2497
    _parfileTof=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mTofCriParNickel.par # Valid for runs 2335-2497
  else
    _parfileTof=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mTofCriPar.par # Up to TOF team to tune for testing
  fi
fi
########################################################################################################################


_ratelog=0 # hides ZMQ messages rates and bandwidth
#_ratelog=1 # display ZMQ messages rates and bandwidth

LOGFILETAG=`hostname`
LOGFILETAG+="_"
LOGFILETAG+=`date +%Y_%m_%d_%H_%M_%S`
LOGFILETAG+=".log"

# Apply sets of settings for different triggers
case $_TriggSet in
  0) # default, any Tof hit
  _TofMean=20
  _TofWin=100
  _TofMin=1
  _T0Min=0
  _StsMin=0
  _StsMinLay=0
  _MuchMin=0
  _Trd1dMin=0
  _Trd2dMin=0
  _RichMin=0
  ;;

  1) # default,  Tof - T0 concidences (pulser)
  _TofMean=20
  _TofWin=200
  _TofMin=2
  _TofMinLay=1
  _T0Min=1
  _StsMin=0
  _StsMinLay=0
  _MuchMin=0
  _Trd1dMin=0
  _Trd2dMin=0
  _RichMin=0
  ;;

  2) # Tof standalone track trigger (cosmic)
  _TofMean=20
  _TofWin=50
  _TofMin=8
  _TofMinLay=4
  _T0Min=0
  _StsMin=0
  _StsMinLay=0
  _MuchMin=0
  _Trd1dMin=0
  _Trd2dMin=0
  _RichMin=0
  ;;

  3) # Tof track trigger with T0
  _TofMean=20
  _TofWin=50
  _TofMin=8
  _TofMinLay=4
  _T0Min=1
  _StsMin=0
  _StsMinLay=0
  _MuchMin=0
  _Trd1dMin=0
  _Trd2dMin=0
  _RichMin=0
  ;;

  4) # mCbm track trigger Tof, T0 & STS
  _TofMean=20
  _TofWin=50
  _TofMin=8
  _TofMinLay=4
  _T0Min=1
  _StsMin=2
  _StsMinLay=0
  _MuchMin=0
  _Trd1dMin=0
  _Trd2dMin=0
  _RichMin=0
  ;;

  5) # mCbm lambda trigger
  _TofMean=20
  _TofWin=40
  _TofMin=16
  _TofMinLay=8  # PAL 07/04/2022: Not sure here if it should be 4 or 8 (2 tracks in same det. stack excluded?)
  _T0Min=1
  _StsMin=8
  _StsMinLay=0
  _MuchMin=0
  _Trd1dMin=0
  _Trd2dMin=0
  _RichMin=0
  ;;

  6) # One hit per detector system w/ big acceptance = mCbm full track trigger
  _TofMean=20
  _TofWin=50
  _TofMin=8
  _TofMinLay=4
  _T0Min=1
  _StsMin=4
  _StsMinLay=0
  _MuchMin=2
  _Trd1dMin=2
  _Trd2dMin=0
  _RichMin=0
esac
echo Using MQ trigger par set: $_TriggSet

# Compute limits of TOF selection/trigger window
_TofL=$_TofMean
_TofH=$_TofMean
(( _TofL -= _TofWin))
(( _TofH += _TofWin))
echo Tof window $_TofL - $_TofH

LIST_OF_PIDS=""

(( _paraBuffSz=100 ))
(( _singBuffSz=_paraBuffSz*_nbbranch ))

echo "Buffer size for parallel  devices $_paraBuffSz"
echo "Buffer size for singleton devices $_singBuffSz"


SAMPLER="RepReqTsSampler"
SAMPLER+=" --control static"
SAMPLER+=" --id sampler1"
#SAMPLER+=" --max-timeslices 0"
#SAMPLER+=" --max-timeslices 10"
#SAMPLER+=" --max-timeslices 30"
#SAMPLER+=" --max-timeslices 100"
#SAMPLER+=" --max-timeslices 300"
#SAMPLER+=" --max-timeslices 1000"
SAMPLER+=" --max-timeslices -1"
#SAMPLER+=" --severity info"
#SAMPLER+=" --flib-port 10"
if [ "$_hostname" != "" ]; then
  SAMPLER+=" --fles-host $_hostname"
elif [ "$_filename" != "" ]; then
  SAMPLER+=" --filename $_filename"
  if [ "$_dirname" != "" ]; then
    SAMPLER+=" --dirname $_dirname"
  fi
fi
SAMPLER+=" --high-water-mark 10"
SAMPLER+=" --no-split-ts 1"
SAMPLER+=" --ChNameMissTs missedts"
SAMPLER+=" --ChNameCmds commands"
SAMPLER+=" --PubFreqTs $_pubfreqts"
SAMPLER+=" --PubTimeMin $_pubminsec"
SAMPLER+=" --PubTimeMax $_pubmaxsec"
SAMPLER+=" --channel-config name=ts-request,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:11555,rateLogging=$_ratelog"
SAMPLER+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11666,rateLogging=$_ratelog"
SAMPLER+=" --channel-config name=missedts,type=pub,method=bind,address=tcp://127.0.0.1:11006,rateLogging=$_ratelog"
SAMPLER+=" --channel-config name=commands,type=pub,method=bind,address=tcp://127.0.0.1:11007,rateLogging=$_ratelog"
SAMPLER+=" --transport zeromq"
# Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
# with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
SAMPLER_LOG="sampler1_$LOGFILETAG"
# xterm -l -lf $SAMPLER_LOG -geometry 80x23+0+0 -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/source/$SAMPLER  &
nohup /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/source/$SAMPLER  &> $SAMPLER_LOG &

echo $SAMPLER

LIST_OF_PIDS+=$!
LIST_OF_PIDS+=" "

PARAMETERSERVER="parmq-server"
PARAMETERSERVER+=" --control static"
PARAMETERSERVER+=" --id parmq-server"
PARAMETERSERVER+=" --severity info"
PARAMETERSERVER+=" --channel-name parameters"
PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
PARAMETERSERVER+=" --first-input-name $_parfileSts;$_parfileMuch;$_parfileTrdAsic;$_parfileTrdDigi;$_parfileTrdGas;$_parfileTrdGain;$_parfileTof;$_parfileBmon;$_parfileRich;$_parfilePsd"
PARAMETERSERVER+=" --first-input-type ASCII"
#PARAMETERSERVER+=" --libs-to-load=CbmStsBase;CbmMuchBase;CbmTrdBase;CbmTofBase;CbmRichBase" # doesn't work due to runtime problem
PARAMETERSERVER+=" --setup $_setup_name"
# Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
# with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
PARAMSRV_LOG="parmq_$LOGFILETAG"
# xterm -l -lf $PARAMSRV_LOG -geometry 80x23+1600+0 -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/parmq/$PARAMETERSERVER &
nohup /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/parmq/$PARAMETERSERVER &> $PARAMSRV_LOG &

LIST_OF_PIDS+=$!
LIST_OF_PIDS+=" "

HISTSERVER="MqHistoServer"
HISTSERVER+=" --control static"
HISTSERVER+=" --id server1"
HISTSERVER+=" --severity info"
HISTSERVER+=" --histport 8081"
HISTSERVER+=" --channel-config name=histogram-in,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11666,rateLogging=$_ratelog"
HISTSERVER+=" --channel-config name=histo-conf,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11667,rateLogging=0"
HISTSERVER+=" --channel-config name=canvas-conf,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11668,rateLogging=0"
# Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
# with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
HISTSRV_LOG="server1_$LOGFILETAG"
# xterm -l -lf $HISTSRV_LOG -geometry 80x23+2000+0 -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/histogramServer/$HISTSERVER &
nohup /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/histogramServer/$HISTSERVER &> $HISTSRV_LOG &

LIST_OF_PIDS+=$!
LIST_OF_PIDS+=" "

sleep 1

_iBranch=0
while (( _iBranch < _nbbranch )); do
  (( _yOffset=200*_iBranch ))
  (( _iBranch += 1 ))
  (( _iPort = 11680 + _iBranch ))

  UNPACKER="MqUnpack"
  UNPACKER+=" --control static"
  UNPACKER+=" --id unp$_iBranch"
  #UNPACKER+=" --severity error"
  UNPACKER+=" --severity info"
  #UNPACKER+=" --severity debug"
  UNPACKER+=" --Setup $_setup_name"
  UNPACKER+=" --RunId $_run_id"
  UNPACKER+=" --IgnOverMs 1"
  UNPACKER+=" --UnpBmon true"
  UNPACKER+=" --UnpMuch true"
  UNPACKER+=" --UnpPsd false"
  UNPACKER+=" --SetTimeOffs kT0,0"
  UNPACKER+=" --SetTimeOffs kSTS,-980"
  UNPACKER+=" --SetTimeOffs kMUCH,-980"
  UNPACKER+=" --SetTimeOffs kTRD,1300"
  UNPACKER+=" --SetTimeOffs kTRD2D,-510"
  UNPACKER+=" --SetTimeOffs kTOF,40"
  UNPACKER+=" --SetTimeOffs kRICH,130"
  UNPACKER+=" --PubFreqTs $_pubfreqts"
  UNPACKER+=" --PubTimeMin $_pubminsec"
  UNPACKER+=" --PubTimeMax $_pubmaxsec"
  UNPACKER+=" --TsNameOut unpts$_iBranch"
  UNPACKER+=" --channel-config name=ts-request,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11555,rateLogging=$_ratelog"
  UNPACKER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
  UNPACKER+=" --channel-config name=unpts$_iBranch,type=push,method=bind,transport=zeromq,sndBufSize=2,address=tcp://127.0.0.1:$_iPort,rateLogging=$_ratelog"
#  UNPACKER+=" --channel-config name=commands,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11007"
  UNPACKER+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11666,rateLogging=$_ratelog"
  UNPACKER+=" --transport zeromq"
  # Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
  # with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
  UNPACKER_LOG="unp$_iBranch"
  UNPACKER_LOG+="_$LOGFILETAG"
  # xterm -l -lf $UNPACKER_LOG -geometry 132x23+400+$_yOffset -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/mcbm/$UNPACKER &
  nohup /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/mcbm/$UNPACKER &> $UNPACKER_LOG &

  LIST_OF_PIDS+=$!
  LIST_OF_PIDS+=" "

  EVTBUILDER="BuildDigiEvents"
  EVTBUILDER+=" --control static"
  EVTBUILDER+=" --id build$_iBranch"
  EVTBUILDER+=" --severity info"
  #EVTBUILDER+=" --severity debug"
  EVTBUILDER+=" --PubFreqTs $_pubfreqts"
  EVTBUILDER+=" --PubTimeMin $_pubminsec"
  EVTBUILDER+=" --PubTimeMax $_pubmaxsec"
  EVTBUILDER+=" --FillHistos true"
#  EVTBUILDER+=" --FillHistos false"
  EVTBUILDER+=" --IgnTsOver false"
  EVTBUILDER+=" --EvtOverMode AllowOverlap"
  EVTBUILDER+=" --RefDet kTof"
  EVTBUILDER+=" --AddDet kT0"
  EVTBUILDER+=" --AddDet kMuch"
  EVTBUILDER+=" --DelDet kPsd"
  EVTBUILDER+=" --SetTrigWin kT0,-100,100"
  EVTBUILDER+=" --SetTrigWin kSts,-60,60"
  EVTBUILDER+=" --SetTrigWin kMuch,-400,400"
  EVTBUILDER+=" --SetTrigWin kTrd,-300,300"
  EVTBUILDER+=" --SetTrigWin kTrd2D,-300,300"
  EVTBUILDER+=" --SetTrigWin kTof,$_TofL,$_TofH"
  EVTBUILDER+=" --SetTrigWin kRich,-60,60"
  EVTBUILDER+=" --SetTrigMinNb kT0,$_T0Min"
  EVTBUILDER+=" --SetTrigMinNb kSts,$_StsMin"
  EVTBUILDER+=" --SetTrigMinNb kMuch,0"
  EVTBUILDER+=" --SetTrigMinNb kTrd,0"
  EVTBUILDER+=" --SetTrigMinNb kTrd2D,0"
  EVTBUILDER+=" --SetTrigMinNb kTof,$_TofMin"
  EVTBUILDER+=" --SetTrigMinNb kRich,0"
  EVTBUILDER+=" --SetTrigMinLayersNb kTof,$_TofMinLay"
  EVTBUILDER+=" --SetTrigMinLayersNb kSts,$_StsMinLay"
  EVTBUILDER+=" --TsNameIn unpts$_iBranch"
  EVTBUILDER+=" --EvtNameOut events"
  EVTBUILDER+=" --channel-config name=unpts$_iBranch,type=pull,method=connect,transport=zeromq,rcvBufSize=2,address=tcp://127.0.0.1:$_iPort,rateLogging=$_ratelog"
  EVTBUILDER+=" --channel-config name=events,type=push,method=connect,transport=zeromq,sndBufSize=2,address=tcp://127.0.0.1:11556,rateLogging=$_ratelog"
#  EVTBUILDER+=" --channel-config name=commands,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11007"
  EVTBUILDER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
  EVTBUILDER+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11666,rateLogging=$_ratelog"
  EVTBUILDER+=" --transport zeromq"
  # Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
  # with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
  EVTBUILDER_LOG="build$_iBranch"
  EVTBUILDER_LOG+="_$LOGFILETAG"
  # xterm -l -lf $EVTBUILDER_LOG -geometry 80x23+800+$_yOffset -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/mcbm/$EVTBUILDER &
  nohup /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/mcbm/$EVTBUILDER &> $EVTBUILDER_LOG &
  VALGRIND_EVT="valgrind -v --error-limit=no --suppressions=$ROOTSYS/etc/root/valgrind-root.supp --leak-check=full --show-reachable=yes --log-file=valgrind_evt_log.txt"
  VALGHEAP_EVT="valgrind --tool=massif --massif-out-file=valgrind_evt_massif.out"
#  nohup $VALGRIND_EVT /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/mcbm/$EVTBUILDER &> $EVTBUILDER_LOG &
#  nohup $VALGHEAP_EVT /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/mcbm/$EVTBUILDER &> $EVTBUILDER_LOG &

  LIST_OF_PIDS+=$!
  LIST_OF_PIDS+=" "
done

EVTSINK="DigiEventSink"
EVTSINK+=" --control static"
EVTSINK+=" --id evtsink1"
EVTSINK+=" --severity info"
#EVTSINK+=" --severity debug"
#EVTSINK+=" --StoreFullTs 1"
#EVTSINK+=" --BypassConsecutiveTs 1"
EVTSINK+=" --OutFileName mcbm_digis_events.root"
EVTSINK+=" --FillHistos true"
#EVTSINK+=" --BypassConsecutiveTs true"
EVTSINK+=" --PubFreqTs $_pubfreqts"
EVTSINK+=" --PubTimeMin $_pubminsec"
EVTSINK+=" --PubTimeMax $_pubmaxsec"
EVTSINK+=" --EvtNameIn events"
EVTSINK+=" --channel-config name=events,type=pull,method=bind,transport=zeromq,rcvBufSize=$_nbbranch,address=tcp://127.0.0.1:11556,rateLogging=$_ratelog"
EVTSINK+=" --channel-config name=missedts,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11006,rateLogging=$_ratelog"
EVTSINK+=" --channel-config name=commands,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11007,rateLogging=$_ratelog"
EVTSINK+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11666,rateLogging=$_ratelog"
# Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
# with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
EVTSINK_LOG="evtsink1_$LOGFILETAG"
# xterm -l -lf $EVTSINK_LOG -geometry 80x23+1200+0 -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/mcbm/$EVTSINK &
nohup /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/mcbm/$EVTSINK &> $EVTSINK_LOG &

LIST_OF_PIDS+=$!
LIST_OF_PIDS+=" "

PID_LOG="pids_$LOGFILETAG"
echo $LIST_OF_PIDS &> $PID_LOG
