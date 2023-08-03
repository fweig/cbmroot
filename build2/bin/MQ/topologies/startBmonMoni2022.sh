#!/bin/bash

if [ -e /home/cbmdata/_deps/fairsoft_apr22/install//bin/fairmq-shmmonitor ]; then
  /home/cbmdata/_deps/fairsoft_apr22/install//bin/fairmq-shmmonitor --cleanup
fi

if [ $# -ge 2 ]; then
  _nbbranch=$1
  _run_id=$2
  ((_pubfreqts = $_nbbranch*100 ))
  _pubminsec=1.0
  _pubmaxsec=10.0

  if [ $# -ge 5 ]; then
    _filename=""
    _dirname=""
    _hostname=$5

    if [ $# -ge 6 ]; then
      _pubfreqts=$6

      if [ $# -ge 7 ]; then
        _pubminsec=$7

        if [ $# -ge 8 ]; then
          _pubmaxsec=$8
        fi
      fi
    fi
  elif [ $# -ge 3 ]; then
    _filename=$3
    _hostname=""
    if [ $# -eq 4 ]; then
      _dirname=$4
    else
      _dirname=""
    fi
  else
    echo 'Missing parameters or wrong number of parameters.'
    echo 'Possible usages are:'
    echo 'startBmonMoni2022.sh <Nb // branches> <Run Id> <full filename pattern list>'
    echo 'startBmonMoni2022.sh <Nb // branches> <Run Id> <filename pattern> <folder_path>'
    echo 'startBmonMoni2022.sh <Nb // branches> <Run Id> "" "" <hostname(s) list>'
    echo 'startBmonMoni2022.sh <Nb // branches> <Run Id> "" "" <hostname(s) list> <Hist publish freq. in TS>'
    echo 'startBmonMoni2022.sh <Nb // branches> <Run Id> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s>'
    echo 'startBmonMoni2022.sh <Nb // branches> <Run Id> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s> <Max Hist pub. in s>'

    return -1
  fi
else
  echo 'Missing parameters. At least the number of branches and the trigger set are required'
  echo 'Possible usages are:'
  echo 'startBmonMoni2022.sh <Nb // branches> <Run Id> <full filename pattern list>'
  echo 'startBmonMoni2022.sh <Nb // branches> <Run Id> <filename pattern> <folder_path>'
  echo 'startBmonMoni2022.sh <Nb // branches> <Run Id> "" "" <hostname(s) list>'
  echo 'startBmonMoni2022.sh <Nb // branches> <Run Id> "" "" <hostname(s) list> <Hist publish freq. in TS>'
  echo 'startBmonMoni2022.sh <Nb // branches> <Run Id> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s>'
  echo 'startBmonMoni2022.sh <Nb // branches> <Run Id> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s> <Max Hist pub. in s>'

  return -1
fi

_parfileSts=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mStsPar.par
_parfileMuch=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mMuchPar.par  # Valid from 2163
_parfileTrdAsic=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22d_mcbm.asic.par
_parfileTrdDigi=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22d_mcbm.digi.par
_parfileTrdGas=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22d_mcbm.gas.par
_parfileTrdGain=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v22d_mcbm.gain.par
_parfileTof=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mTofCriPar.par
_parfileBmon=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mBmonCriPar.par
_parfileRich=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2021/mRichPar_70.par
_parfilePsd=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2021/mPsdPar.par
_setup_name=mcbm_beam_2022_03_22_iron

if [ $_run_id -ge 2060 ]; then
  if [ $_run_id -le 2065 ]; then
    _setup_name=mcbm_beam_2022_03_09_carbon
    _parfileMuch=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mMuchParUpto26032022.par # Valid for runs 2060-2162
  elif [ $_run_id -le 2160 ]; then # Potentially wrong setup between 2065 and 2150 but not official runs
    _setup_name=mcbm_beam_2022_03_22_iron
    _parfileMuch=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2022/mMuchParUpto26032022.par # Valid for runs 2060-2162
  elif [ $_run_id -le 2310 ]; then # Potentially wrong setup between 2160 and 2176 but not official runs
    _setup_name=mcbm_beam_2022_03_28_uranium
  fi
fi


_ratelog=0 # hides ZMQ messages rates and bandwidth
#_ratelog=1 # display ZMQ messages rates and bandwidth

LOGFILETAG=`hostname`
LOGFILETAG+="_"
LOGFILETAG+=`date +%Y_%m_%d_%H_%M_%S`
LOGFILETAG+=".log"

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
HISTSERVER+=" --histport 8091"
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

  UNPACKER="BmonMonitor"
  UNPACKER+=" --control static"
  UNPACKER+=" --id unp$_iBranch"
  #UNPACKER+=" --severity error"
  UNPACKER+=" --severity info"
  #UNPACKER+=" --severity debug"
  UNPACKER+=" --Setup $_setup_name"
  UNPACKER+=" --RunId $_run_id"
  UNPACKER+=" --IgnOverMs 1"
  UNPACKER+=" --UnpBmon true"
  UNPACKER+=" --SetTimeOffs kT0,0"
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
done

LIST_OF_PIDS+=$!
LIST_OF_PIDS+=" "

PID_LOG="pids_$LOGFILETAG"
echo $LIST_OF_PIDS &> $PID_LOG
