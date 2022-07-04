#!/bin/bash

#SBATCH -J McbmSink
#SBATCH --oversubscribe

# Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# author: Pierre-Alain Loizeau [committer]

_histServHost="127.0.0.1"
_parServHost="127.0.0.1"
if [ $# -ge 5 ]; then
  _run_id=$1
  _nbbranch=$2
  _TriggSet=$3
  _Disk=$4
  _hostname=$5
  if [ $# -ge 6 ]; then
    _histServHost=$6
    if [ $# -eq 7 ]; then
      _parServHost=$7
    fi
  fi
else
  echo 'Missing parameters. Only following pattern allowed:'
  echo 'mq_sink.sbatch <Run Id> <Nb // branches> <Trigger set> <Storage disk index> <hostname:port>'
  echo 'mq_sink.sbatch <Run Id> <Nb // branches> <Trigger set> <Storage disk index> <hostname:port> <hist serv host>'
  echo 'mq_sink.sbatch <Run Id> <Nb // branches> <Trigger set> <Storage disk index> <hostname:port> <hist serv host> <par. serv host>'

  return -1
fi

# Prepare log folder variables
_log_folder="/local/mcbm2022/online_logs/${_run_id}/"
_localhost=`hostname`

echo $SLURM_ARRAY_TASK_ID ${_localhost} ${_run_id} ${_nbbranch} ${_TriggSet} ${_hostname}

# CBMROOT + FAIRMQ initialisation
_BuildDir=/scratch/loizeau/cbmroot_mcbm/build
source ${_BuildDir}/config.sh
# source /local/mcbm2022/install/config.sh
if [ -e $SIMPATH/bin/fairmq-shmmonitor ]; then
  $SIMPATH/bin/fairmq-shmmonitor --cleanup
fi

# Only one Processing branch is monitoring, and the full topology gets 2.5 TS/s, so with 10 branches pub may be ~10s
_pubfreqts=3
_pubminsec=1.0
_pubmaxsec=10.0

########################################################################################################################

_ratelog=0 # hides ZMQ messages rates and bandwidth
#_ratelog=1 # display ZMQ messages rates and bandwidth

# ProcessName_runid_trigset_hostname_yyyy_mm_dd_hh_mm_ss.log
LOGFILETAG="${_run_id}_${_TriggSet}_${_localhost}_"
LOGFILETAG+=`date +%Y_%m_%d_%H_%M_%S`
LOGFILETAG+=".log"

########################################################################################################################

############################
# Event Sink               #
############################
EVTSINK_LOG="${_log_folder}evtsink1_${LOGFILETAG}"

EVTSINK="DigiEventSink"
EVTSINK+=" --control static"
EVTSINK+=" --id evtsink1"
EVTSINK+=" --severity info"
# EVTSINK+=" --severity debug"
EVTSINK+=" --StoreFullTs 0"
# EVTSINK+=" --BypassConsecutiveTs true"
EVTSINK+=" --WriteMissingTs false"
EVTSINK+=" --DisableCompression true"
EVTSINK+=" --TreeFileMaxSize 4000000000"
if [ ${_Disk} -eq 0 ]; then
  EVTSINK+=" --OutFileName /local/mcbm2022/data/${_run_id}_${_TriggSet}_${_localhost}.digi_events.root"
else
  EVTSINK+=" --OutFileName /storage/${_Disk}/data/${_run_id}_${_TriggSet}_${_localhost}.digi_events.root"
fi
EVTSINK+=" --FillHistos true"
EVTSINK+=" --PubFreqTs $_pubfreqts"
EVTSINK+=" --PubTimeMin $_pubminsec"
EVTSINK+=" --PubTimeMax $_pubmaxsec"
EVTSINK+=" --EvtNameIn events"
EVTSINK+=" --channel-config name=events,type=pull,method=bind,transport=zeromq,rcvBufSize=$_nbbranch,address=tcp://127.0.0.1:11556,rateLogging=$_ratelog"
EVTSINK+=" --channel-config name=missedts,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11006,rateLogging=$_ratelog"
EVTSINK+=" --channel-config name=commands,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11007,rateLogging=$_ratelog"
EVTSINK+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://${_histServHost}:11666,rateLogging=$_ratelog"

echo ${_BuildDir}/bin/MQ/mcbm/$EVTSINK &> $EVTSINK_LOG &
${_BuildDir}/bin/MQ/mcbm/$EVTSINK &> $EVTSINK_LOG &

STILL_RUNNING=`ps | wc -l`
STILL_RUNNING_OUT="${STILL_RUNNING}\n"
STILL_RUNNING_OUT+=`ps`
echo `date` "${STILL_RUNNING_OUT}" > ${_log_folder}/still_running_sink.txt
while [ 6 -lt $STILL_RUNNING ]; do
 sleep 5
# ps
# echo `ps | wc -l`
 STILL_RUNNING=`ps | wc -l`
 STILL_RUNNING_OUT="${STILL_RUNNING}\n"
 STILL_RUNNING_OUT+=`ps`
 echo `date` "${STILL_RUNNING_OUT}" > ${_log_folder}/still_running_sink.txt
done
