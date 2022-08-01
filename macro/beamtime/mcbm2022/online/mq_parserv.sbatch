#!/bin/bash

#SBATCH -J McbmPars
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
# --> Setup
if [ $_run_id -ge 2060 ]; then
  if [ $_run_id -le 2065 ]; then
    _setup_name=mcbm_beam_2022_03_09_carbon
  elif [ $_run_id -le 2160 ]; then # Potentially wrong setup between 2065 and 2150 but not official runs
    _setup_name=mcbm_beam_2022_03_22_iron
  elif [ $_run_id -le 2310 ]; then # Potentially wrong setup between 2160 and 2176 but not official runs
    _setup_name=mcbm_beam_2022_03_28_uranium
    _parfileTrdAsic=$VMCWORKDIR/parameters/trd/trd_v22g_mcbm.asic.par
    _parfileTrdDigi=$VMCWORKDIR/parameters/trd/trd_v22g_mcbm.digi.par
    _parfileTrdGas=$VMCWORKDIR/parameters/trd/trd_v22g_mcbm.gas.par
    _parfileTrdGain=$VMCWORKDIR/parameters/trd/trd_v22g_mcbm.gain.par
  elif [ $_run_id -le 2397 ]; then # Potentially wrong setup between 2310 and 2350 but not official runs
    _setup_name=mcbm_beam_2022_05_23_nickel
    _parfileTrdAsic=$VMCWORKDIR/parameters/trd/trd_v22h_mcbm.asic.par
    _parfileTrdDigi=$VMCWORKDIR/parameters/trd/trd_v22h_mcbm.digi.par
    _parfileTrdGas=$VMCWORKDIR/parameters/trd/trd_v22h_mcbm.gas.par
    _parfileTrdGain=$VMCWORKDIR/parameters/trd/trd_v22h_mcbm.gain.par
  elif [ $_run_id -le 2497 ]; then # Potentially wrong setup between 2397 and 2454 but not official runs
    _setup_name=mcbm_beam_2022_06_16_gold
    _parfileTrdAsic=$VMCWORKDIR/parameters/trd/trd_v22h_mcbm.asic.par
    _parfileTrdDigi=$VMCWORKDIR/parameters/trd/trd_v22h_mcbm.digi.par
    _parfileTrdGas=$VMCWORKDIR/parameters/trd/trd_v22h_mcbm.gas.par
    _parfileTrdGain=$VMCWORKDIR/parameters/trd/trd_v22h_mcbm.gain.par
  fi
fi
# --> mMUCH
if [ $_run_id -ge 2060 ]; then
  if [ $_run_id -le 2162 ]; then
    _parfileMuch=$VMCWORKDIR/macro/beamtime/mcbm2022/mMuchParUpto26032022.par # Valid for runs 2060-2162
  elif [ $_run_id -le 2291 ]; then
    _parfileMuch=$VMCWORKDIR/macro/beamtime/mcbm2022/mMuchParUpto03042022.par # Valid for runs 2163-2291
  elif [ $_run_id -le 2315 ]; then
    _parfileMuch=$VMCWORKDIR/macro/beamtime/mcbm2022/mMuchParUpto10042022.par # Valid for runs 2292-2315
  elif [ $_run_id -le 2366 ]; then
    _parfileMuch=$VMCWORKDIR/macro/beamtime/mcbm2022/mMuchParUpto23052022.par # Valid for runs 2316-2366
  elif [ $_run_id -le 2397 ]; then
    _parfileMuch=$VMCWORKDIR/macro/beamtime/mcbm2022/mMuchParUpto26052022.par # Valid for runs 2060-2162
  else
    _parfileMuch=$VMCWORKDIR/macro/beamtime/mcbm2022/mMuchPar.par # Valid for runs 2398 and higher until further notice
  fi
fi
# --> mTOF
if [ $_run_id -ge 2060 ]; then
  if [ $_run_id -le 2065 ]; then
    _parfileTof=$VMCWORKDIR/macro/beamtime/mcbm2022/mTofCriParCarbon.par # Valid for runs 2060-2065
  elif [ $_run_id -le 2160 ]; then # Potentially wrong setup between 2065 and 2150 but not official runs
    _parfileTof=$VMCWORKDIR/macro/beamtime/mcbm2022/mTofCriParIron.par # Valid for runs 2150-2160
  elif [ $_run_id -le 2310 ]; then # Potentially wrong setup between 2160 and 2176 but not official runs
    _parfileTof=$VMCWORKDIR/macro/beamtime/mcbm2022/mTofCriParUranium.par # Valid for runs 2176-2310
  elif [ $_run_id -le 2497 ]; then # Potentially wrong setup between 2310 and 2335 but not official runs
     # Nickel runs: 2335 - 2397
     # Gold runs: 2400 - 2497
    _parfileTof=$VMCWORKDIR/macro/beamtime/mcbm2022/mTofCriParNickel.par # Valid for runs 2335-2497
  else
    _parfileTof=$VMCWORKDIR/macro/beamtime/mcbm2022/mTofCriPar.par # Up to TOF team to tune for testing
  fi
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
# Parameter server         #
############################
PARAMSRV_LOG="${_log_folder}parmq_${LOGFILETAG}"

PARAMETERSERVER="parmq-server"
PARAMETERSERVER+=" --control static"
PARAMETERSERVER+=" --id parmq-server"
PARAMETERSERVER+=" --severity info"
PARAMETERSERVER+=" --channel-name parameters"
PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
PARAMETERSERVER+=" --first-input-name $_parfileSts;$_parfileTrdAsic;$_parfileTrdDigi;$_parfileTrdGas;$_parfileTrdGain;$_parfileTof;$_parfileBmon;$_parfileRich"
PARAMETERSERVER+=" --first-input-type ASCII"
PARAMETERSERVER+=" --setup $_setup_name"

echo ${_BuildDir}/bin/MQ/parmq/$PARAMETERSERVER &> $PARAMSRV_LOG &
${_BuildDir}/bin/MQ/parmq/$PARAMETERSERVER &> $PARAMSRV_LOG &

STILL_RUNNING=`ps | wc -l`
STILL_RUNNING_OUT="${STILL_RUNNING}\n"
STILL_RUNNING_OUT+=`ps`
echo `date` "${STILL_RUNNING_OUT}" > ${_log_folder}/still_running_parserv.txt
while [ 6 -lt $STILL_RUNNING ]; do
 sleep 5
# ps
# echo `ps | wc -l`
 STILL_RUNNING=`ps | wc -l`
 STILL_RUNNING_OUT="${STILL_RUNNING}\n"
 STILL_RUNNING_OUT+=`ps`
 echo `date` "${STILL_RUNNING_OUT}" > ${_log_folder}/still_running_parserv.txt
done
