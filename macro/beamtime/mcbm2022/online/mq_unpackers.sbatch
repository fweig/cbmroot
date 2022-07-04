#!/bin/bash

#SBATCH -J McbmUnps
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
  echo 'mq_unpackers.sbatch <Run Id> <Nb // branches> <Trigger set> <Storage disk index> <hostname:port>'
  echo 'mq_unpackers.sbatch <Run Id> <Nb // branches> <Trigger set> <Storage disk index> <hostname:port> <hist serv host>'
  echo 'mq_unpackers.sbatch <Run Id> <Nb // branches> <Trigger set> <Storage disk index> <hostname:port> <hist serv host> <par. serv host>'

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

# Setup file and parameter files for parameter server
_setup_name=mcbm_beam_2022_03_22_iron
_parfileBmon=$VMCWORKDIR/macro/beamtime/mcbm2022/mBmonCriPar.par
_parfileSts=$VMCWORKDIR/macro/beamtime/mcbm2022/mStsPar.par
_parfileTrdAsic=$VMCWORKDIR/parameters/trd/trd_v22d_mcbm.asic.par
_parfileTrdDigi=$VMCWORKDIR/parameters/trd/trd_v22d_mcbm.digi.par
_parfileTrdGas=$VMCWORKDIR/parameters/trd/trd_v22d_mcbm.gas.par
_parfileTrdGain=$VMCWORKDIR/parameters/trd/trd_v22d_mcbm.gain.par
_parfileTof=$VMCWORKDIR/macro/beamtime/mcbm2022/mTofCriPar.par
_parfileRich=$VMCWORKDIR/macro/beamtime/mcbm2021/mRichPar_70.par

# Parameter files => Update depending on run ID!!!
if [ $_run_id -ge 2060 ]; then
  if [ $_run_id -le 2065 ]; then
    _setup_name=mcbm_beam_2022_03_09_carbon
    _parfileTof=$VMCWORKDIR/macro/beamtime/mcbm2022/mTofCriParCarbon.par
  elif [ $_run_id -le 2160 ]; then # Potentially wrong setup between 2065 and 2150 but not official runs
    _setup_name=mcbm_beam_2022_03_22_iron
    _parfileTof=$VMCWORKDIR/macro/beamtime/mcbm2022/mTofCriParIron.par
  elif [ $_run_id -le 2310 ]; then # Potentially wrong setup between 2160 and 2176 but not official runs
    _setup_name=mcbm_beam_2022_03_28_uranium
    _parfileTrdAsic=$VMCWORKDIR/parameters/trd/trd_v22g_mcbm.asic.par
    _parfileTrdDigi=$VMCWORKDIR/parameters/trd/trd_v22g_mcbm.digi.par
    _parfileTrdGas=$VMCWORKDIR/parameters/trd/trd_v22g_mcbm.gas.par
    _parfileTrdGain=$VMCWORKDIR/parameters/trd/trd_v22g_mcbm.gain.par
    _parfileTof=$VMCWORKDIR/macro/beamtime/mcbm2022/mTofCriParUranium.par
  elif [ $_run_id -ge 2350 ]; then
    _setup_name=mcbm_beam_2022_05_23_nickel
    _parfileTrdAsic=$VMCWORKDIR/parameters/trd/trd_v22h_mcbm.asic.par
    _parfileTrdDigi=$VMCWORKDIR/parameters/trd/trd_v22h_mcbm.digi.par
    _parfileTrdGas=$VMCWORKDIR/parameters/trd/trd_v22h_mcbm.gas.par
    _parfileTrdGain=$VMCWORKDIR/parameters/trd/trd_v22h_mcbm.gain.par
    _parfileTof=$VMCWORKDIR/macro/beamtime/mcbm2022/mTofCriParUranium.par
  fi
fi

########################################################################################################################

# Apply sets of settings for different triggers
_UnpTimeOffsBMon=0
_UnpTimeOffsSts=-970
_UnpTimeOffsTrd1d=1225
_UnpTimeOffsTrd2d=-525
_UnpTimeOffsTof=45
_UnpTimeOffsRich=95

########################################################################################################################

_ratelog=0 # hides ZMQ messages rates and bandwidth
#_ratelog=1 # display ZMQ messages rates and bandwidth

# ProcessName_runid_trigset_hostname_yyyy_mm_dd_hh_mm_ss.log
LOGFILETAG="${_run_id}_${_TriggSet}_${_localhost}_"
LOGFILETAG+=`date +%Y_%m_%d_%H_%M_%S`
LOGFILETAG+=".log"

########################################################################################################################

echo ${_BuildDir}/bin/MQ/mcbm/$EVTSINK &> $EVTSINK_LOG &
${_BuildDir}/bin/MQ/mcbm/$EVTSINK &> $EVTSINK_LOG &

############################
# Processing branches      #
############################
_iBranch=0
while (( _iBranch < _nbbranch )); do
  (( _iPort = 11680 + _iBranch ))

  ##########################
  # Unpacker               #
  ##########################
  UNPACKER_LOG="${_log_folder}unp${_iBranch}_${LOGFILETAG}"

  UNPACKER="MqUnpack"
  UNPACKER+=" --control static"
  UNPACKER+=" --id unp$_iBranch"
#  UNPACKER+=" --severity error"
  UNPACKER+=" --severity info"
#  UNPACKER+=" --severity debug"
  UNPACKER+=" --Setup $_setup_name"
  UNPACKER+=" --RunId $_run_id"
  UNPACKER+=" --IgnOverMs false"
  UNPACKER+=" --UnpBmon true"
  UNPACKER+=" --UnpMuch false"
  UNPACKER+=" --UnpPsd false"
  UNPACKER+=" --SetTimeOffs kT0,${_UnpTimeOffsBMon}"
  UNPACKER+=" --SetTimeOffs kSTS,${_UnpTimeOffsSts}"
  UNPACKER+=" --SetTimeOffs kTRD,${_UnpTimeOffsTrd1d}"
  UNPACKER+=" --SetTimeOffs kTRD2D,${_UnpTimeOffsTrd2d}"
  UNPACKER+=" --SetTimeOffs kTOF,${_UnpTimeOffsTof}"
  UNPACKER+=" --SetTimeOffs kRICH,${_UnpTimeOffsRich}"
  UNPACKER+=" --PubFreqTs $_pubfreqts"
  UNPACKER+=" --PubTimeMin $_pubminsec"
  UNPACKER+=" --PubTimeMax $_pubmaxsec"
#  if [ ${_iBranch} -eq 0 ]; then
#    UNPACKER+=" --FillHistos true"
#  else
#    UNPACKER+=" --FillHistos false"
#  fi
  UNPACKER+=" --TsNameOut unpts$_iBranch"
  UNPACKER+=" --channel-config name=ts-request,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11555,rateLogging=$_ratelog"
  UNPACKER+=" --channel-config name=unpts$_iBranch,type=push,method=bind,transport=zeromq,sndBufSize=2,address=tcp://127.0.0.1:$_iPort,rateLogging=$_ratelog"
#  UNPACKER+=" --channel-config name=commands,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11007"
  UNPACKER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://${_parServHost}:11005,rateLogging=0"
  UNPACKER+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://${_histServHost}:11666,rateLogging=$_ratelog"
  UNPACKER+=" --transport zeromq"

  echo ${_BuildDir}/bin/MQ/mcbm/$UNPACKER &> $UNPACKER_LOG &
  ${_BuildDir}/bin/MQ/mcbm/$UNPACKER &> $UNPACKER_LOG &
  (( _iBranch += 1 ))
done

STILL_RUNNING=`ps | wc -l`
STILL_RUNNING_OUT="${STILL_RUNNING}\n"
STILL_RUNNING_OUT+=`ps`
echo `date` "${STILL_RUNNING_OUT}" > ${_log_folder}/still_running_unpackers.txt
while [ 6 -lt $STILL_RUNNING ]; do
 sleep 5
# ps
# echo `ps | wc -l`
 STILL_RUNNING=`ps | wc -l`
 STILL_RUNNING_OUT="${STILL_RUNNING}\n"
 STILL_RUNNING_OUT+=`ps`
 echo `date` "${STILL_RUNNING_OUT}" > ${_log_folder}/still_running_unpackers.txt
done
