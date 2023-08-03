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
SAMPLER+=" --no-split-ts 1"
SAMPLER+=" --channel-config name=fullts,type=push,method=bind,address=tcp://127.0.0.1:11555"
#SAMPLER+=" --transport shmem"
SAMPLER+=" --transport zeromq"
#SAMPLER+=" --transport nanomsg"
xterm -l -geometry 80x23+0+0 -hold -e /usr/local/bin/MQ/source/$SAMPLER &

_iMoni=0
while (( _iMoni < _nbmoni )); do
  (( _yOffset=200*_iMoni ))
  (( _iMoni += 1 ))
  (( _iPort = 11680 + _iMoni ))

  UNPACKER="McbmUnpack"
  UNPACKER+=" --id unp$_iMoni"
  UNPACKER+=" --severity info"
  UNPACKER+=" --IgnOverMs 1"
  UNPACKER+=" --TsNameOut unpts$_iMoni"
  UNPACKER+=" --channel-config name=fullts,type=pull,method=connect,address=tcp://127.0.0.1:11555"
  #UNPACKER+=" --transport shmem"
  UNPACKER+=" --transport zeromq"
  #UNPACKER+=" --transport nanomsg"
#  UNPACKER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11005"
  UNPACKER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
  UNPACKER+=" --channel-config name=unpts$_iMoni,type=pub,method=bind,transport=zeromq,address=tcp://127.0.0.1:$_iPort"
  #UNPACKER+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11666"
  #UNPACKER+=" --channel-config name=histo-conf,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11667,rateLogging=0"
  #UNPACKER+=" --channel-config name=canvas-conf,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11668,rateLogging=0"
  xterm -l -geometry 80x23+400+$_yOffset -hold -e /usr/local/bin/MQ/mcbm/$UNPACKER &

  MONITOR="McbmMonitorPulser"
  MONITOR+=" --id mon$_iMoni"
  MONITOR+=" --severity info"
  MONITOR+=" --PubFreqTs $_pubfreqts"
  MONITOR+=" --PubTimeMin $_pubminsec"
  MONITOR+=" --PubTimeMax $_pubmaxsec"
  MONITOR+=" --TsNameIn unpts$_iMoni"
  MONITOR+=" --channel-config name=unpts$_iMoni,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:$_iPort"
  #MONITOR+=" --transport shmem"
  MONITOR+=" --transport zeromq"
  #MONITOR+=" --transport nanomsg"
  MONITOR+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
  MONITOR+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11666"
  MONITOR+=" --channel-config name=histo-conf,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11667,rateLogging=0"
  MONITOR+=" --channel-config name=canvas-conf,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11668,rateLogging=0"
  xterm -l -geometry 80x23+800+$_yOffset -hold -e /usr/local/bin/MQ/mcbm/$MONITOR &
done

PARAMETERSERVER="parmq-server"
PARAMETERSERVER+=" --id parmq-server"
PARAMETERSERVER+=" --severity info"
PARAMETERSERVER+=" --channel-name parameters"
#PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:11005"
PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
PARAMETERSERVER+=" --first-input-name $_parfileSts;$_parfileMuch;$_parfileTrdAsic;$_parfileTrdDigi;$_parfileTrdGas;$_parfileTrdGain;$_parfileTof;$_parfileRich;$_parfilePsd"
PARAMETERSERVER+=" --first-input-type ASCII"
PARAMETERSERVER+=" --libs-to-load=libCbmFlibMcbm2018" # doesn't work due to runtime problem
xterm -geometry 80x23+1200+0 -hold -e /usr/local/bin/MQ/parmq/$PARAMETERSERVER &

HISTSERVER="MqHistoServer"
HISTSERVER+=" --id server1"
HISTSERVER+=" --histport 8081"
HISTSERVER+=" --channel-config name=histogram-in,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11666"
HISTSERVER+=" --channel-config name=histo-conf,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11667,rateLogging=0"
HISTSERVER+=" --channel-config name=canvas-conf,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11668,rateLogging=0"
xterm -geometry 80x23+1400+0 -hold -e /usr/local/bin/MQ/histogramServer/$HISTSERVER &
