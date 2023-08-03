#!/bin/bash

if [ -e /home/cbmdata/_deps/fairsoft_apr22/install//bin/fairmq-shmmonitor ]; then
  /home/cbmdata/_deps/fairsoft_apr22/install//bin/fairmq-shmmonitor --cleanup
fi

if [ $# -ge 1 ]; then
  _nbmoni=$1
  ((_pubfreqts = $_nbmoni*100 ))
  _pubminsec=1.0
  _pubmaxsec=10.0

  if [ $# -ge 4 ]; then
    _filename=""
    _dirname=""
    _hostname=$4

    if [ $# -ge 5 ]; then
      _pubfreqts=$5

      if [ $# -ge 6 ]; then
        _pubminsec=$6

        if [ $# -ge 7 ]; then
          _pubmaxsec=$7
        fi
      fi
    fi
  elif [ $# -ge 2 ]; then
    _filename=$2
    _hostname=""
    if [ $# -eq 3 ]; then
      _dirname=$3
    else
      _dirname=""
    fi
  else
    echo 'Starting connection to local stream'
    echo ' for other usages, please supply at least a filename.'
    echo 'Possible usages are:'
    echo 'startUnpack.sh'
    echo 'startUnpack.sh <Nb Unp & Moni processes>'
    echo 'startUnpack.sh <Nb Unp & Moni processes> <full filename pattern list>'
    echo 'startUnpack.sh <Nb Unp & Moni processes> <filename pattern> <folder_path>'
    echo 'startUnpack.sh <Nb Unp & Moni processes> "" "" <hostname(s) list>'
    echo 'startUnpack.sh <Nb Unp & Moni processes> "" "" <hostname(s) list> <Hist publish freq. in TS>'
    echo 'startUnpack.sh <Nb Unp & Moni processes> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s>'
    echo 'startUnpack.sh <Nb Unp & Moni processes> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s> <Max Hist pub. in s>'
    _filename=""
    _dirname=""
    _hostname="localhost"
  fi
else
  echo 'Starting connection to local stream with 1 monitor process'
  echo ' for other usages, please supply at least a filename.'
  echo 'Possible usages are:'
  echo 'startUnpack.sh'
  echo 'startUnpack.sh <Nb Unp & Moni processes>'
  echo 'startUnpack.sh <Nb Unp & Moni processes> <full filename pattern list>'
  echo 'startUnpack.sh <Nb Unp & Moni processes> <filename pattern> <folder_path>'
  echo 'startUnpack.sh <Nb Unp & Moni processes> "" "" <hostname(s) list>'
  echo 'startUnpack.sh <Nb Unp & Moni processes> "" "" <hostname(s) list> <Hist publish freq. in TS>'
  echo 'startUnpack.sh <Nb Unp & Moni processes> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s>'
  echo 'startUnpack.sh <Nb Unp & Moni processes> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s> <Max Hist pub. in s>'
  _filename=""
  _dirname=""
  _hostname="localhost"
  _nbmoni=1
  _pubfreqts=100
  _pubminsec=1.0
  _pubmaxsec=10.0
fi

_parfileSts=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2021/mStsPar.par
_parfileMuch=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2021/mMuchPar.par
_parfileTrdAsic=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v21b_mcbm.asic.par
_parfileTrdDigi=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v21b_mcbm.digi.par
_parfileTrdGas=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v21b_mcbm.gas.par
_parfileTrdGain=/home/heinemann/gpuunpacker/cbmroot/parameters/trd/trd_v21b_mcbm.gain.par
_parfileTof=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2021/mTofCriPar.par
_parfileRich=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2021/mRichPar_70.par
_parfilePsd=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2021/mPsdPar.par
_setup_name=mcbm_beam_2021_07_surveyed
_run_id=1588

LOGFILETAG=`hostname`
LOGFILETAG+="_"
LOGFILETAG+=`date +%Y_%m_%d_%H_%M_%S`
LOGFILETAG+=".log"

(( _paraBuffSz=100 ))
(( _singBuffSz=_paraBuffSz*_nbmoni ))

echo "Buffer size for parallel  devices $_paraBuffSz"
echo "Buffer size for singleton devices $_singBuffSz"

SAMPLER="RepReqTsSampler"
SAMPLER+=" --id sampler1"
SAMPLER+=" --max-timeslices 300"
SAMPLER+=" --severity info"
#SAMPLER+=" --flib-port 10"
if [ "$_hostname" != "" ]; then
  SAMPLER+=" --fles-host $_hostname"
elif [ "$_filename" != "" ]; then
  SAMPLER+=" --filename $_filename"
  if [ "$_dirname" != "" ]; then
    SAMPLER+=" --dirname $_dirname"
  fi
fi
SAMPLER+=" --high-water-mark 1000"
SAMPLER+=" --no-split-ts 1"
SAMPLER+=" --ChNameMissTs missedts"
SAMPLER+=" --ChNameCmds commands"
SAMPLER+=" --PubFreqTs $_pubfreqts"
SAMPLER+=" --PubTimeMin $_pubminsec"
SAMPLER+=" --PubTimeMax $_pubmaxsec"
SAMPLER+=" --channel-config name=ts-request,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:11555"
SAMPLER+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11666"
SAMPLER+=" --channel-config name=missedts,type=pub,method=bind,address=tcp://127.0.0.1:11006"
SAMPLER+=" --channel-config name=commands,type=pub,method=bind,address=tcp://127.0.0.1:11007"
SAMPLER+=" --transport zeromq"
# Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
# with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
SAMPLER_LOG="sampler1_$LOGFILETAG"
xterm -l -lf $SAMPLER_LOG -geometry 80x23+0+0 -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/source/$SAMPLER  &

echo $SAMPLER

_iMoni=0
while (( _iMoni < _nbmoni )); do
  (( _yOffset=200*_iMoni ))
  (( _iMoni += 1 ))
  (( _iPort = 11680 + _iMoni ))

  UNPACKER="MqDevUnpack"  
  UNPACKER+=" --id unp$_iMoni"
  UNPACKER+=" --severity info"
  #UNPACKER+=" --severity debug"
  UNPACKER+=" --RunId $_run_id"
  UNPACKER+=" --TsNameOut unpts$_iMoni"
  UNPACKER+=" --channel-config name=ts-request,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11555"
  UNPACKER+=" --channel-config name=unpts$_iMoni,type=push,method=bind,transport=zeromq,address=tcp://127.0.0.1:$_iPort"
  UNPACKER+=" --transport zeromq"
  # Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
  # with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
  UNPACKER_LOG="unp$_iMoni"
  UNPACKER_LOG+="_$LOGFILETAG"
  xterm -l -lf $UNPACKER_LOG -geometry 132x23+400+$_yOffset -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/mq/$UNPACKER &

  TRIGGER="MqDevTrigger"
  TRIGGER+=" --id trigger$_iMoni"
  TRIGGER+=" --severity info"
  #TRIGGER+=" --severity debug"
###  TRIGGER+=" --TriggerDet Tof"
  TRIGGER+=" --TriggerDet Sts"
  TRIGGER+=" --TriggerWin 50.0"
  TRIGGER+=" --TriggerMinDigis 20"
  TRIGGER+=" --TriggerDeadTime 10.0"
  TRIGGER+=" --TsNameIn unpts$_iMoni"
  TRIGGER+=" --TriggerNameOut trigger"
  TRIGGER+=" --channel-config name=unpts$_iMoni,type=pull,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_iPort"
  TRIGGER+=" --channel-config name=trigger,type=push,method=connect,transport=zeromq,address=tcp://127.0.0.1:11556"
  TRIGGER+=" --transport zeromq"
  # Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
  # with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
  TRIGGER_LOG="trigger$_iMoni"
  TRIGGER_LOG+="_$LOGFILETAG"
  xterm -l -lf $TRIGGER_LOG -geometry 80x23+800+$_yOffset -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/mq/$TRIGGER &

  EVTBUILDER="MqDevBuildEvents"
  EVTBUILDER+=" --id build$_iMoni"
  EVTBUILDER+=" --severity info"
  #EVTBUILDER+=" --severity debug"
  EVTBUILDER+=" --OutFileName events.root"
  EVTBUILDER+=" --SetEvbuildWin kSts,-100,100"
##  EVTBUILDER+=" --SetEvbuildWin kTrd,-250,250"
##  EVTBUILDER+=" --SetEvbuildWin kTof,-150,150" # To get T0 Digis (seed + close-by digis) in the event
##  EVTBUILDER+=" --SetEvbuildWin kRich,-100,100"
##  EVTBUILDER+=" --SetEvbuildWin kPsd,-100,100"
  EVTBUILDER+=" --TrigNameIn trigger"
  EVTBUILDER+=" --EvtNameOut events"
  EVTBUILDER+=" --channel-config name=trigger,type=pull,method=bind,transport=zeromq,address=tcp://127.0.0.1:11556"
  EVTBUILDER+=" --channel-config name=events,type=push,method=connect,transport=zeromq,address=tcp://127.0.0.1:11557"
  EVTBUILDER+=" --transport zeromq"
  # Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
  # with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
  EVTBUILDER_LOG="build$_iMoni"
  EVTBUILDER_LOG+="_$LOGFILETAG"
  xterm -l -lf $EVTBUILDER_LOG -geometry 80x23+800+$_yOffset -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/mq/$EVTBUILDER &

done

EVTSINK="MqDevEventSink"
EVTSINK+=" --id evtsink1"
EVTSINK+=" --severity info"
#EVTSINK+=" --severity debug"
EVTSINK+=" --OutFileName mcbm_digis_events.root"
EVTSINK+=" --ChannelNameDataInput events"
EVTSINK+=" --channel-config name=events,type=pull,method=bind,transport=zeromq,address=tcp://127.0.0.1:11557"
EVTSINK+=" --channel-config name=commands,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11007"
# Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
# with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
EVTSINK_LOG="evtsink1_$LOGFILETAG"
xterm -l -lf $EVTSINK_LOG -geometry 80x23+1200+0 -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/mq/$EVTSINK &

PARAMETERSERVER="parmq-server"
PARAMETERSERVER+=" --id parmq-server"
PARAMETERSERVER+=" --severity info"
PARAMETERSERVER+=" --channel-name parameters"
PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
PARAMETERSERVER+=" --first-input-name $_parfileSts;$_parfileMuch;$_parfileTrdAsic;$_parfileTrdDigi;$_parfileTrdGas;$_parfileTrdGain;$_parfileTof;$_parfileRich;$_parfilePsd"
PARAMETERSERVER+=" --first-input-type ASCII"
PARAMETERSERVER+=" --libs-to-load=libCbmFlibMcbm2018" # doesn't work due to runtime problem
PARAMETERSERVER+=" --setup $_setup_name"
# Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
# with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
PARAMSRV_LOG="parmq_$LOGFILETAG"
xterm -l -lf $PARAMSRV_LOG -geometry 80x23+1600+0 -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/parmq/$PARAMETERSERVER &

HISTSERVER="MqHistoServer"
HISTSERVER+=" --id server1"
HISTSERVER+=" --severity info"
HISTSERVER+=" --histport 8081"
HISTSERVER+=" --channel-config name=histogram-in,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11666"
HISTSERVER+=" --channel-config name=histo-conf,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11667,rateLogging=0"
HISTSERVER+=" --channel-config name=canvas-conf,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11668,rateLogging=0"
# Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
# with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
HISTSRV_LOG="server1_$LOGFILETAG"
xterm -l -lf $HISTSRV_LOG -geometry 80x23+2000+0 -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/histogramServer/$HISTSERVER &
