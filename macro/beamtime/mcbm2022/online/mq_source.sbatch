#!/bin/bash

#SBATCH -J McbmSource
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
  echo 'mq_source.sbatch <Run Id> <Nb // branches> <Trigger set> <Storage disk index> <hostname:port>'
  echo 'mq_source.sbatch <Run Id> <Nb // branches> <Trigger set> <Storage disk index> <hostname:port> <hist serv host>'
  echo 'mq_source.sbatch <Run Id> <Nb // branches> <Trigger set> <Storage disk index> <hostname:port> <hist serv host> <par. serv host>'

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
# Sampler                  #
############################
SAMPLER_LOG="${_log_folder}sampler1_${LOGFILETAG}"

SAMPLER="RepReqTsSampler"
SAMPLER+=" --control static"
SAMPLER+=" --id sampler1"
SAMPLER+=" --max-timeslices -1"
SAMPLER+=" --severity info"
SAMPLER+=" --fles-host $_hostname"
SAMPLER+=" --high-water-mark 10"
SAMPLER+=" --no-split-ts 1"
SAMPLER+=" --ChNameMissTs missedts"
SAMPLER+=" --ChNameCmds commands"
SAMPLER+=" --PubFreqTs $_pubfreqts"
SAMPLER+=" --PubTimeMin $_pubminsec"
SAMPLER+=" --PubTimeMax $_pubmaxsec"
SAMPLER+=" --channel-config name=ts-request,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:11555,rateLogging=$_ratelog"
SAMPLER+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://${_histServHost}:11666,rateLogging=$_ratelog"
SAMPLER+=" --channel-config name=missedts,type=pub,method=bind,address=tcp://127.0.0.1:11006,rateLogging=$_ratelog"
SAMPLER+=" --channel-config name=commands,type=pub,method=bind,address=tcp://127.0.0.1:11007,rateLogging=$_ratelog"
SAMPLER+=" --transport zeromq"

echo ${_BuildDir}/bin/MQ/source/$SAMPLER &> $SAMPLER_LOG &
${_BuildDir}/bin/MQ/source/$SAMPLER &> $SAMPLER_LOG &

STILL_RUNNING=`ps | wc -l`
STILL_RUNNING_OUT="${STILL_RUNNING}\n"
STILL_RUNNING_OUT+=`ps`
echo `date` "${STILL_RUNNING_OUT}" > ${_log_folder}/still_running_source.txt
while [ 6 -lt $STILL_RUNNING ]; do
 sleep 5
# ps
# echo `ps | wc -l`
 STILL_RUNNING=`ps | wc -l`
 STILL_RUNNING_OUT="${STILL_RUNNING}\n"
 STILL_RUNNING_OUT+=`ps`
 echo `date` "${STILL_RUNNING_OUT}" > ${_log_folder}/still_running_source.txt
done
