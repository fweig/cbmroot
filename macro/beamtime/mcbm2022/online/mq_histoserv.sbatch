#!/bin/bash

#SBATCH -J McbmHist
#SBATCH --oversubscribe

# Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# author: Pierre-Alain Loizeau [committer]

if [ $# -eq 5 ]; then
  _run_id=$1
  _nbbranch=$2
  _TriggSet=$3
  _Disk=$4
  _hostname=$5
else
  echo 'Missing parameters. Only following pattern allowed:'
  echo 'mq_processing_node.sbatch <Run Id> <Nb // branches> <Trigger set> <Storage disk index> <hostname:port>'

  return -1
fi

# Prepare log folder variable
_log_folder="/local/mcbm2022/online_logs/${_run_id}/"

echo $SLURM_ARRAY_TASK_ID ${_run_id} ${_nbbranch} ${_TriggSet} ${_hostname}

# CBMROOT + FAIRMQ initialisation
_BuildDir=/scratch/loizeau/cbmroot_mcbm/build
source ${_BuildDir}/config.sh
# source /local/mcbm2022/install/config.sh
if [ -e $SIMPATH/bin/fairmq-shmmonitor ]; then
  $SIMPATH/bin/fairmq-shmmonitor --cleanup
fi
########################################################################################################################

_ratelog=0 # hides ZMQ messages rates and bandwidth
#_ratelog=1 # display ZMQ messages rates and bandwidth

# ProcessName_runid_trigset_hostname_yyyy_mm_dd_hh_mm_ss.log
LOGFILETAG="${_run_id}_${_TriggSet}_"
LOGFILETAG+=`hostname`
LOGFILETAG+="_"
LOGFILETAG+=`date +%Y_%m_%d_%H_%M_%S`
LOGFILETAG+=".log"

########################################################################################################################

############################
# Histogram server         #
############################
HISTSRV_LOG="${_log_folder}server1_${LOGFILETAG}"

HISTSERVER="MqHistoServer"
HISTSERVER+=" --control static"
HISTSERVER+=" --id server1"
HISTSERVER+=" --severity info"
HISTSERVER+=" --histport 8080"
HISTSERVER+=" --channel-config name=histogram-in,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11666,rateLogging=$_ratelog"
HISTSERVER+=" --channel-config name=histo-conf,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11667,rateLogging=0"
HISTSERVER+=" --channel-config name=canvas-conf,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11668,rateLogging=0"

echo ${_BuildDir}/bin/MQ/histogramServer/$HISTSERVER &> $HISTSRV_LOG &
${_BuildDir}/bin/MQ/histogramServer/$HISTSERVER &> $HISTSRV_LOG &

STILL_RUNNING=`ps | wc -l`
STILL_RUNNING_OUT="${STILL_RUNNING}\n"
STILL_RUNNING_OUT+=`ps`
echo `date` "${STILL_RUNNING_OUT}" > ${_log_folder}/still_running_histoserv.txt
while [ 6 -lt $STILL_RUNNING ]; do
 sleep 5
# ps
# echo `ps | wc -l`
 STILL_RUNNING=`ps | wc -l`
 STILL_RUNNING_OUT="${STILL_RUNNING}\n"
 STILL_RUNNING_OUT+=`ps`
 echo `date` "${STILL_RUNNING_OUT}" > ${_log_folder}/still_running_histoserv.txt
done
