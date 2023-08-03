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
    echo 'startMQMcbmPulserMonitor2020.sh'
    echo 'startMQMcbmPulserMonitor2020.sh <Nb Unp & Moni processes>'
    echo 'startMQMcbmPulserMonitor2020.sh <Nb Unp & Moni processes> <full filename pattern list>'
    echo 'startMQMcbmPulserMonitor2020.sh <Nb Unp & Moni processes> <filename pattern> <folder_path>'
    echo 'startMQMcbmPulserMonitor2020.sh <Nb Unp & Moni processes> "" "" <hostname(s) list>'
    echo 'startMQMcbmPulserMonitor2020.sh <Nb Unp & Moni processes> "" "" <hostname(s) list> <Hist publish freq. in TS>'
    echo 'startMQMcbmPulserMonitor2020.sh <Nb Unp & Moni processes> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s>'
    echo 'startMQMcbmPulserMonitor2020.sh <Nb Unp & Moni processes> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s> <Max Hist pub. in s>'
    _filename=""
    _dirname=""
    _hostname="localhost"
  fi
else
  echo 'Starting connection to local stream with 1 monitor process'
  echo ' for other usages, please supply at least a filename.'
  echo 'Possible usages are:'
  echo 'startMQMcbmPulserMonitor2020.sh'
  echo 'startMQMcbmPulserMonitor2020.sh <Nb Unp & Moni processes>'
  echo 'startMQMcbmPulserMonitor2020.sh <Nb Unp & Moni processes> <full filename pattern list>'
  echo 'startMQMcbmPulserMonitor2020.sh <Nb Unp & Moni processes> <filename pattern> <folder_path>'
  echo 'startMQMcbmPulserMonitor2020.sh <Nb Unp & Moni processes> "" "" <hostname(s) list>'
  echo 'startMQMcbmPulserMonitor2020.sh <Nb Unp & Moni processes> "" "" <hostname(s) list> <Hist publish freq. in TS>'
  echo 'startMQMcbmPulserMonitor2020.sh <Nb Unp & Moni processes> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s>'
  echo 'startMQMcbmPulserMonitor2020.sh <Nb Unp & Moni processes> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s> <Max Hist pub. in s>'
  _filename=""
  _dirname=""
  _hostname="localhost"
  _nbmoni=1
  _pubfreqts=100
  _pubminsec=1.0
  _pubmaxsec=10.0
fi

_parfileSts=/usr/local/share/cbmroot/macro/beamtime/mcbm2020/mStsPar.par
_parfileMuch=/usr/local/share/cbmroot/macro/beamtime/mcbm2020/mMuchPar.par
_parfileTrdAsic=/usr/local/share/cbmroot/parameters/trd/trd_v18q_mcbm.asic.par
_parfileTrdDigi=/usr/local/share/cbmroot/parameters/trd/trd_v18q_mcbm.digi.par
_parfileTrdGas=/usr/local/share/cbmroot/parameters/trd/trd_v18q_mcbm.gas.par
_parfileTrdGain=/usr/local/share/cbmroot/parameters/trd/trd_v18q_mcbm.gain.par
_parfileTof=/usr/local/share/cbmroot/macro/beamtime/mcbm2020/mTofPar.par
_parfileRich=/usr/local/share/cbmroot/macro/beamtime/mcbm2020/mRichPar.par
_parfilePsd=/usr/local/share/cbmroot/macro/beamtime/mcbm2020/mPsdPar.par

LOGFILETAG=`hostname`
LOGFILETAG+="_"
LOGFILETAG+=`date +%Y_%m_%d_%H_%M_%S`
LOGFILETAG+=".log"

(( _paraBuffSz=100 ))
(( _singBuffSz=_paraBuffSz*_nbmoni ))

echo "Buffer size for parallel  devices $_paraBuffSz"
echo "Buffer size for singleton devices $_singBuffSz"

SAMPLER="MultiTsaSampler"
SAMPLER+=" --id sampler1"
#SAMPLER+=" --max-timeslices 0"
#SAMPLER+=" --max-timeslices 10"
#SAMPLER+=" --max-timeslices 100"
SAMPLER+=" --max-timeslices 300"
#SAMPLER+=" --max-timeslices 1000"
SAMPLER+=" --severity info"
#SAMPLER+=" --flib-port 10"
if [ "$_hostname" != "" ]; then
  SAMPLER+=" --flib-host $_hostname"
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
SAMPLER+=" --channel-config name=fullts,type=push,method=bind,address=tcp://127.0.0.1:11555"
#SAMPLER+=" --channel-config name=fullts,type=push,method=bind,address=tcp://127.0.0.1:11555,sndBufSize=$_singBuffSz,rcvBuffSize=$_paraBuffSz"
#SAMPLER+=" --transport shmem"
SAMPLER+=" --transport zeromq"
#SAMPLER+=" --transport nanomsg"
SAMPLER+=" --channel-config name=missedts,type=pub,method=bind,address=tcp://127.0.0.1:11006"
SAMPLER+=" --channel-config name=commands,type=pub,method=bind,address=tcp://127.0.0.1:11007"
# Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
# with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
SAMPLER_LOG="sampler1_$LOGFILETAG"
xterm -l -lf $SAMPLER_LOG -geometry 80x23+0+0 -hold -e /usr/local/bin/MQ/source/$SAMPLER  &

echo $SAMPLER

_iMoni=0
while (( _iMoni < _nbmoni )); do
  (( _yOffset=200*_iMoni ))
  (( _iMoni += 1 ))
  (( _iPort = 11680 + _iMoni ))

  UNPACKER="McbmUnpack"
  UNPACKER+=" --id unp$_iMoni"
  UNPACKER+=" --severity info"
  UNPACKER+=" --IgnOverMs 1"
  UNPACKER+=" --SetTimeOffs kSTS,-985"
  UNPACKER+=" --SetTimeOffs kMUCH,-885"
  UNPACKER+=" --SetTimeOffs kTRD,-25"
  UNPACKER+=" --SetTimeOffs kTOF,25"
  UNPACKER+=" --SetTimeOffs kRICH,-310"
  UNPACKER+=" --SetTimeOffs kPSD,-240"
  UNPACKER+=" --TsNameOut unpts$_iMoni"
  UNPACKER+=" --channel-config name=fullts,type=pull,method=connect,address=tcp://127.0.0.1:11555"
  #UNPACKER+=" --transport shmem"
  UNPACKER+=" --transport zeromq"
  #UNPACKER+=" --transport nanomsg"
#  UNPACKER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11005"
  UNPACKER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
  UNPACKER+=" --channel-config name=unpts$_iMoni,type=push,method=bind,transport=zeromq,address=tcp://127.0.0.1:$_iPort"
#  UNPACKER+=" --channel-config name=unpts$_iMoni,type=push,method=bind,transport=zeromq,address=tcp://127.0.0.1:$_iPort,sndBufSize=$_paraBuffSz,rcvBuffSize=$_paraBuffSz"
#  UNPACKER+=" --channel-config name=commands,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11007"
  #UNPACKER+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11666"
  #UNPACKER+=" --channel-config name=histo-conf,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11667,rateLogging=0"
  #UNPACKER+=" --channel-config name=canvas-conf,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11668,rateLogging=0"
  # Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
  # with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
  UNPACKER_LOG="unp$_iMoni"
  UNPACKER_LOG+="_$LOGFILETAG"
  VALGRIND_UNP="valgrind -v --error-limit=no --suppressions=$ROOTSYS/share/root/etc/valgrind-root.supp --leak-check=full --show-reachable=yes --log-file=valgrind_unp_log.txt"
  VALGHEAP_UNP="valgrind -v --tool=massif --massif-out-file=valgrind_unp_massif.out"
  xterm -l -lf $UNPACKER_LOG -geometry 80x23+400+$_yOffset -hold -e /usr/local/bin/MQ/mcbm/$UNPACKER &
#  xterm -l -lf $UNPACKER_LOG -geometry 80x23+400+$_yOffset -hold -e $VALGRIND_UNP /usr/local/bin/MQ/mcbm/$UNPACKER &
#  xterm -l -lf $UNPACKER_LOG -geometry 80x23+400+$_yOffset -hold -e $VALGHEAP_UNP /usr/local/bin/MQ/mcbm/$UNPACKER &

  EVTBUILDER="BuildRawEvents"
  EVTBUILDER+=" --id build$_iMoni"
  EVTBUILDER+=" --severity info"
  EVTBUILDER+=" --PubFreqTs $_pubfreqts"
  EVTBUILDER+=" --PubTimeMin $_pubminsec"
  EVTBUILDER+=" --PubTimeMax $_pubmaxsec"
  EVTBUILDER+=" --FillHistos true"
  EVTBUILDER+=" --IgnTsOver false"
  EVTBUILDER+=" --EvtOverMode NoOverlap"
  EVTBUILDER+=" --SetTrigWin kT0,-1,10" # To get T0 Digis (seed + close-by digis) in the event
  EVTBUILDER+=" --SetTrigWin kSts,-50,100"
  EVTBUILDER+=" --SetTrigWin kMuch,-150,50"
  EVTBUILDER+=" --SetTrigWin kTrd,-250,100"
  EVTBUILDER+=" --SetTrigWin kTof,-150,10"
  EVTBUILDER+=" --SetTrigWin kRich,-150,20"
  EVTBUILDER+=" --SetTrigWin kPsd,-50,10"
  EVTBUILDER+=" --SetTrigMinNb kT0,1"
  EVTBUILDER+=" --SetTrigMinNb kSts,0"
  EVTBUILDER+=" --SetTrigMinNb kMuch,0"
  EVTBUILDER+=" --SetTrigMinNb kTrd,0"
  EVTBUILDER+=" --SetTrigMinNb kTof,10"
  EVTBUILDER+=" --SetTrigMinNb kRich,0"
  EVTBUILDER+=" --SetTrigMinNb kPsd,0"
  EVTBUILDER+=" --TsNameIn unpts$_iMoni"
  EVTBUILDER+=" --EvtNameOut events"
  EVTBUILDER+=" --channel-config name=unpts$_iMoni,type=pull,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_iPort"
  #EVTBUILDER+=" --transport shmem"
  EVTBUILDER+=" --transport zeromq"
  #EVTBUILDER+=" --transport nanomsg"
  EVTBUILDER+=" --channel-config name=events,type=push,method=connect,transport=zeromq,address=tcp://127.0.0.1:11556"
#  EVTBUILDER+=" --channel-config name=commands,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11007"
  EVTBUILDER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
  EVTBUILDER+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11666"
  EVTBUILDER+=" --channel-config name=histo-conf,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11667,rateLogging=0"
  EVTBUILDER+=" --channel-config name=canvas-conf,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11668,rateLogging=0"
  # Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
  # with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
  EVTBUILDER_LOG="build$_iMoni"
  EVTBUILDER_LOG+="_$LOGFILETAG"
  VALGRIND_EVT="valgrind -v --error-limit=no --suppressions=$ROOTSYS/share/root/etc/valgrind-root.supp --leak-check=full --show-reachable=yes --log-file=valgrind_evt_log.txt"
  VALGHEAP_EVT="valgrind --tool=massif --massif-out-file=valgrind_evt_massif.out"
  xterm -l -lf $EVTBUILDER_LOG -geometry 80x23+800+$_yOffset -hold -e /usr/local/bin/MQ/mcbm/$EVTBUILDER &
#  xterm -l -lf $EVTBUILDER_LOG -geometry 80x23+800+$_yOffset -hold -e $VALGRIND_EVT /usr/local/bin/MQ/mcbm/$EVTBUILDER &
#  xterm -l -lf $EVTBUILDER_LOG -geometry 80x23+800+$_yOffset -hold -e $VALGHEAP_EVT /usr/local/bin/MQ/mcbm/$EVTBUILDER &

done

EVTSINK="McbmEventSink"
EVTSINK+=" --id evtsink1"
EVTSINK+=" --severity info"
EVTSINK+=" --OutFileName mcbm_digis_events.root"
EVTSINK+=" --FillHistos false"
EVTSINK+=" --PubFreqTs $_pubfreqts"
EVTSINK+=" --PubTimeMin $_pubminsec"
EVTSINK+=" --PubTimeMax $_pubmaxsec"
EVTSINK+=" --EvtNameIn events"
EVTSINK+=" --channel-config name=events,type=pull,method=bind,transport=zeromq,address=tcp://127.0.0.1:11556"
#EVTSINK+=" --channel-config name=events,type=pull,method=bind,transport=zeromq,address=tcp://127.0.0.1:11556,sndBufSize=$_paraBuffSz,rcvBuffSize=$_unpBufSz"
EVTSINK+=" --channel-config name=missedts,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11006"
EVTSINK+=" --channel-config name=commands,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11007"
EVTSINK+=" --channel-config name=histogram-in,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11666"
EVTSINK+=" --channel-config name=histo-conf,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11667,rateLogging=0"
EVTSINK+=" --channel-config name=canvas-conf,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11668,rateLogging=0"
# Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
# with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
EVTSINK_LOG="evtsink1_$LOGFILETAG"
VALGRIND_SINK="valgrind -v --error-limit=no --suppressions=$ROOTSYS/share/root/etc/valgrind-root.supp --leak-check=full --show-reachable=yes --log-file=valgrind_sink_log.txt"
VALGHEAP_SINK="valgrind -v --tool=massif --massif-out-file=valgrind_sink_massif.out"
VALGCPU_SINK="valgrind -v --tool=callgrind"
xterm -l -lf $EVTSINK_LOG -geometry 80x23+1200+0 -hold -e /usr/local/bin/MQ/mcbm/$EVTSINK &
#xterm -l -lf $EVTSINK_LOG -geometry 80x23+1200+0 -hold -e $VALGRIND_SINK /usr/local/bin/MQ/mcbm/$EVTSINK &
#xterm -l -lf $EVTSINK_LOG -geometry 80x23+1200+0 -hold -e $VALGHEAP_SINK /usr/local/bin/MQ/mcbm/$EVTSINK &
#xterm -l -lf $EVTSINK_LOG -geometry 80x23+1200+0 -hold -e $VALGCPU_SINK /usr/local/bin/MQ/mcbm/$EVTSINK &

PARAMETERSERVER="parmq-server"
PARAMETERSERVER+=" --id parmq-server"
PARAMETERSERVER+=" --severity info"
PARAMETERSERVER+=" --channel-name parameters"
#PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:11005"
PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
PARAMETERSERVER+=" --first-input-name $_parfileSts;$_parfileMuch;$_parfileTrdAsic;$_parfileTrdDigi;$_parfileTrdGas;$_parfileTrdGain;$_parfileTof;$_parfileRich;$_parfilePsd"
PARAMETERSERVER+=" --first-input-type ASCII"
PARAMETERSERVER+=" --libs-to-load=libCbmFlibMcbm2018" # doesn't work due to runtime problem
# Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
# with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
PARAMSRV_LOG="parmq_$LOGFILETAG"
xterm -l -lf $PARAMSRV_LOG -geometry 80x23+1600+0 -hold -e /usr/local/bin/MQ/parmq/$PARAMETERSERVER &

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
xterm -l -lf $HISTSRV_LOG -geometry 80x23+2000+0 -hold -e /usr/local/bin/MQ/histogramServer/$HISTSERVER &
