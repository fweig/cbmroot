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
    echo 'startMQSamplerT0Monitor2020.sh'
    echo 'startMQSamplerT0Monitor2020.sh <Nb Monitor processes>'
    echo 'startMQSamplerT0Monitor2020.sh <Nb Monitor processes> <full filename pattern list>'
    echo 'startMQSamplerT0Monitor2020.sh <Nb Monitor processes> <filename pattern> <folder_path>'
    echo 'startMQSamplerT0Monitor2020.sh <Nb Monitor processes> "" "" <hostname(s) list>'
    echo 'startMQSamplerT0Monitor2020.sh <Nb Monitor processes> "" "" <hostname(s) list> <Hist publish freq. in TS>'
    echo 'startMQSamplerT0Monitor2020.sh <Nb Monitor processes> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s>'
    echo 'startMQSamplerT0Monitor2020.sh <Nb Monitor processes> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s> <Max Hist pub. in s>'
    _filename=""
    _dirname=""
    _hostname="localhost"
  fi
else
  echo 'Starting connection to local stream with 1 monitor process'
  echo ' for other usages, please supply at least a filename.'
  echo 'Possible usages are:'
  echo 'startMQSamplerT0Monitor2020.sh'
  echo 'startMQSamplerT0Monitor2020.sh <Nb Monitor processes>'
  echo 'startMQSamplerT0Monitor2020.sh <Nb Monitor processes> <full filename pattern list>'
  echo 'startMQSamplerT0Monitor2020.sh <Nb Monitor processes> <filename pattern> <folder_path>'
  echo 'startMQSamplerT0Monitor2020.sh <Nb Monitor processes> "" "" <hostname(s) list>'
  echo 'startMQSamplerT0Monitor2020.sh <Nb Monitor processes> "" "" <hostname(s) list> <Hist publish freq. in TS>'
  echo 'startMQSamplerT0Monitor2020.sh <Nb Monitor processes> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s>'
  echo 'startMQSamplerT0Monitor2020.sh <Nb Monitor processes> "" "" <hostname(s) list> <Hist publish freq. in TS> <Min Hist pub. in s> <Max Hist pub. in s>'
  _filename=""
  _dirname=""
  _hostname="localhost"
  _nbmoni=1
  _pubfreqts=100
  _pubminsec=1.0
  _pubmaxsec=10.0
fi

_paramfile=/home/heinemann/gpuunpacker/cbmroot/macro/beamtime/mcbm2021/mT0Par.par

LOGFILETAG=`hostname`
LOGFILETAG+="_"
LOGFILETAG+=`date +%Y_%m_%d_%H_%M_%S`
LOGFILETAG+=".log"

SAMPLER="MultiTsaSampler"
SAMPLER+=" --id sampler1"
SAMPLER+=" --max-timeslices 0"
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
SAMPLER+=" --send-ts-per-sysid 1"
SAMPLER+=" --channel-config name=t0component,type=push,method=bind,address=tcp://127.0.0.1:11555"
#SAMPLER+=" --transport shmem"
SAMPLER+=" --transport zeromq"
#SAMPLER+=" --transport nanomsg"
# Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
# with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
SAMPLER_LOG="sampler1_$LOGFILETAG"
xterm -l -lf $SAMPLER_LOG -geometry 80x23+0+0 -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/source/$SAMPLER &

_iMoni=0
while (( _iMoni < _nbmoni )); do
  (( _yOffset=100*_iMoni ))
  (( _iMoni += 1 ))
  MONITOR="T0MonitorMcbm2018"

  MONITOR+=" --id mon$_iMoni"
  MONITOR+=" --severity info"
  MONITOR+=" --PubFreqTs $_pubfreqts"
  MONITOR+=" --PubTimeMin $_pubminsec"
  MONITOR+=" --PubTimeMax $_pubmaxsec"
  MONITOR+=" --PulsTotMin 185"
  MONITOR+=" --PulsTotMax 190"
  MONITOR+=" --SpillThr 200"
  MONITOR+=" --ChanMap 4,5,6,7,0,1,2,3"
  MONITOR+=" --channel-config name=t0component,type=pull,method=connect,address=tcp://127.0.0.1:11555"
  #MONITOR+=" --transport shmem"
  MONITOR+=" --transport zeromq"
  #MONITOR+=" --transport nanomsg"
  MONITOR+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
  MONITOR+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11666"
  MONITOR+=" --channel-config name=histo-conf,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11667"
  MONITOR+=" --channel-config name=canvas-conf,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11668"
  # Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
  # with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
  MONITOR_LOG="monit0_$_iMoni"
  MONITOR_LOG+="_$LOGFILETAG"
  xterm -l -lf $MONITOR_LOG -geometry 80x23+500+$_yOffset -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/monitor/$MONITOR &
done

PARAMETERSERVER="parmq-server"
PARAMETERSERVER+=" --id parmq-server"
PARAMETERSERVER+=" --severity info"
PARAMETERSERVER+=" --channel-name parameters"
PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
PARAMETERSERVER+=" --first-input-name $_paramfile"
PARAMETERSERVER+=" --first-input-type ASCII"
PARAMETERSERVER+=" --libs-to-load=libCbmFlibMcbm2018" # doesn't work due to runtime problem
# Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
# with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
PARAMSRV_LOG="parmq_$LOGFILETAG"
xterm -l -lf $PARAMSRV_LOG -geometry 80x23+1000+0 -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/parmq/$PARAMETERSERVER &

HISTSERVER="MqHistoServer"
HISTSERVER+=" --id server1"
HISTSERVER+=" --severity info"
HISTSERVER+=" --histport 8082"
HISTSERVER+=" --channel-config name=histogram-in,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11666"
HISTSERVER+=" --channel-config name=histo-conf,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11667"
HISTSERVER+=" --channel-config name=canvas-conf,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11668"
# Replaces log filename Xterm.log.hostname.yyyy.mm.dd.hh.mm.ss.XXXXXX
# with ProcessName_hostname_yyyy_mm_dd_hh_mm_ss.log
HISTSRV_LOG="server1_$LOGFILETAG"
xterm -l -lf $HISTSRV_LOG -geometry 80x23+1500+0 -hold -e /home/heinemann/gpuunpacker/cbmroot/build2/bin/MQ/histogramServer/$HISTSERVER &
