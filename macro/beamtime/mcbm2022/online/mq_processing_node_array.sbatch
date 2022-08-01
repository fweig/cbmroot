#!/bin/bash

#SBATCH -J McbmOnline
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

# Prepare log folder variables
_log_folder="/local/mcbm2022/online_logs/${_run_id}/"
_localhost=`hostname`

echo $SLURM_ARRAY_TASK_ID ${_run_id} ${_nbbranch} ${_TriggSet} ${_hostname}

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

# Apply sets of settings for different triggers
_UnpTimeOffsBMon=0
_UnpTimeOffsSts=-970
_UnpTimeOffsTrd1d=1225
_UnpTimeOffsTrd2d=-525
_UnpTimeOffsTof=45
_UnpTimeOffsRich=95

########################################################################################################################

# Apply sets of settings for different triggers
_TriggerMinNumberBmon=0
_TriggerMinNumberSts=0
_TriggerMinNumberTrd1d=0
_TriggerMinNumberTrd2d=0
_TriggerMinNumberTof=4
_TriggerMinNumberRich=0

_TriggerMaxNumberBMon=-1
_TriggerMaxNumberSts=-1
_TriggerMaxNumberTrd1d=-1
_TriggerMaxNumberTrd2d=-1
_TriggerMaxNumberTof=-1
_TriggerMaxNumberRich=-1

_TriggerMinLayersNumberTof=0
_TriggerMinLayersNumberSts=0

_TrigWinMinBMon=-10
_TrigWinMaxBMon=10
_TrigWinMinSts=-40
_TrigWinMaxSts=40
_TrigWinMinTrd1d=-50
_TrigWinMaxTrd1d=400
_TrigWinMinTrd2d=-60
_TrigWinMaxTrd2d=350
_TrigWinMinTof=-10
_TrigWinMaxTof=70
_TrigWinMinRich=-10
_TrigWinMaxRich=40

bTrigSet=true;
case ${_TriggSet} in
  0)
    # NH: default, any Tof hit
    _TriggerMaxNumberBMon=1000

    _TriggerMinNumberTof=1

    _TrigWinMinBMon=-50
    _TrigWinMaxBMon=50
    _TrigWinMinSts=-60
    _TrigWinMaxSts=60
    _TrigWinMinTrd1d=-300
    _TrigWinMaxTrd1d=300
    _TrigWinMinTrd2d=-200
    _TrigWinMaxTrd2d=200

    _TrigWinMinTof=-80
    _TrigWinMaxTof=120

    _TrigWinMinRich=-60
    _TrigWinMaxRich=60
    ;;

  1)
    # NH: default,  Tof - T0 concidences (pulser)
    _TriggerMinNumberBmon=1
    _TriggerMaxNumberBMon=1000

    _TriggerMinNumberTof=2
    _TriggerMinLayersNumberTof=1

    _TrigWinMinBMon=-50
    _TrigWinMaxBMon=50
    _TrigWinMinSts=-60
    _TrigWinMaxSts=60
    _TrigWinMinTrd1d=-300
    _TrigWinMaxTrd1d=300
    _TrigWinMinTrd2d=-200
    _TrigWinMaxTrd2d=200

    _TrigWinMinTof=-180
    _TrigWinMaxTof=220

    _TrigWinMinRich=-60
    _TrigWinMaxRich=60
    ;;

  2)
    # NH: Tof standalone track trigger (cosmic)
    _TriggerMaxNumberBMon=1000

    _TriggerMinNumberTof=8
    _TriggerMinLayersNumberTof=4

    _TrigWinMinBMon=-50
    _TrigWinMaxBMon=50
    _TrigWinMinSts=-60
    _TrigWinMaxSts=60
    _TrigWinMinTrd1d=-300
    _TrigWinMaxTrd1d=300
    _TrigWinMinTrd2d=-200
    _TrigWinMaxTrd2d=200

    _TrigWinMinTof=-30
    _TrigWinMaxTof=70

    _TrigWinMinRich=-60
    _TrigWinMaxRich=60
    ;;

  3)
    # NH: Tof track trigger with T0
    _TriggerMinNumberBmon=1
    _TriggerMaxNumberBMon=2

    _TriggerMinNumberTof=8
    _TriggerMinLayersNumberTof=4

    _TrigWinMinBMon=-50
    _TrigWinMaxBMon=50
    _TrigWinMinSts=-60
    _TrigWinMaxSts=60
    _TrigWinMinTrd1d=-300
    _TrigWinMaxTrd1d=300
    _TrigWinMinTrd2d=-200
    _TrigWinMaxTrd2d=200
    _TrigWinMinTof=-20
    _TrigWinMaxTof=60
    _TrigWinMinRich=-60
    _TrigWinMaxRich=60
    ;;

  4)
    # NH: mCbm track trigger Tof, T0 & STS
    _TriggerMinNumberBmon=1
    _TriggerMaxNumberBMon=2

    _TriggerMinNumberSts=2
    _TriggerMinLayersNumberSts=1

    _TriggerMinNumberTof=8
    _TriggerMinLayersNumberTof=4

    _TrigWinMinBMon=-50
    _TrigWinMaxBMon=50
    _TrigWinMinSts=-60
    _TrigWinMaxSts=60
    _TrigWinMinTrd1d=-300
    _TrigWinMaxTrd1d=300
    _TrigWinMinTrd2d=-200
    _TrigWinMaxTrd2d=200
    _TrigWinMinTof=-20
    _TrigWinMaxTof=60
    _TrigWinMinRich=-60
    _TrigWinMaxRich=60
    ;;

  5)
    # NH: mCbm lambda trigger
    _TriggerMinNumberBmon=1
    _TriggerMaxNumberBMon=2

    _TriggerMinNumberSts=8
    _TriggerMinLayersNumberSts=2

    _TriggerMinNumberTof=16
    _TriggerMinLayersNumberTof=8

    _TrigWinMinBMon=-50
    _TrigWinMaxBMon=50
    _TrigWinMinSts=-60
    _TrigWinMaxSts=60
    _TrigWinMinTrd1d=-300
    _TrigWinMaxTrd1d=300
    _TrigWinMinTrd2d=-200
    _TrigWinMaxTrd2d=200
    _TrigWinMinTof=-20
    _TrigWinMaxTof=60
    _TrigWinMinRich=-60
    _TrigWinMaxRich=60
    ;;

  6)
    # NH: One hit per detector system w/ big acceptance=mCbm full track trigger
    _TriggerMinNumberBmon=1
    _TriggerMaxNumberBMon=1;

    _TriggerMinNumberSts=4
    _TriggerMinLayersNumberSts=0

    _TriggerMinNumberTrd1d=2

    _TriggerMinNumberTof=8
    _TriggerMinLayersNumberTof=4

    _TrigWinMinBMon=-50
    _TrigWinMaxBMon=50
    _TrigWinMinSts=-60
    _TrigWinMaxSts=60
    _TrigWinMinTrd1d=-300
    _TrigWinMaxTrd1d=300
    _TrigWinMinTrd2d=-200
    _TrigWinMaxTrd2d=200
    _TrigWinMinTof=-20
    _TrigWinMaxTof=60
    _TrigWinMinRich=-60
    _TrigWinMaxRich=60
    ;;

  7)
    # PAL default: T0 + STS + TOF, only digi cut
    _TriggerMinNumberBmon=1
    _TriggerMinNumberSts=2
    _TriggerMinNumberTof=4
    ;;

  8)
    # PAL: default,  Tof - T0 concidences (pulser)
    _TriggerMinNumberBmon=4
    _TriggerMinNumberTof=2
    _TriggerMinLayersNumberTof=1
    ;;

  9)
    # PAL: Tof standalone track trigger (cosmic)
    _TriggerMinNumberTof=8
    _TriggerMinLayersNumberTof=4
    ;;

  10)
    # PAL: Tof track trigger with T0
    _TriggerMinNumberBmon=1
    _TriggerMinNumberTof=8
    _TriggerMinLayersNumberTof=4
    ;;

  11)
    # PAL: mCbm track trigger Tof, T0 & STS
    _TriggerMinNumberBmon=1
    _TriggerMinNumberSts=2
    _TriggerMinNumberTof=8
    _TriggerMinLayersNumberTof=4
    ;;

  12)
    # PAL: mCbm lambda trigger
    _TriggerMinNumberBmon=1
    _TriggerMinNumberSts=8
    _TriggerMinNumberTof=16
    _TriggerMinLayersNumberTof=8
    ;;

  13)
    # PAL: One hit per detector system w/ big acceptance=mCbm full track trigger
    TriggerMinNumberBmon=1
    _TriggerMinNumberSts=4
    _TriggerMinNumberTrd1d=2
    _TriggerMinNumberTrd1d=1
    _TriggerMinNumberTof=8
    _TriggerMinNumberRich=1
    ;;

  14)
    # PAL:  mCbm track trigger Tof, T0 & STS
    _TriggerMinNumberBmon=1
    _TriggerMinNumberSts=4
    _TriggerMinNumberTof=8
    _TriggerMinLayersNumberTof=4
    _TriggerMinLayersNumberSts=2
    ;;

  *)
    bTrigSet=false;
    ;;

esac
echo Using MQ trigger par set: ${_TriggSet}

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

# Each slurm job has a singledevice instance
case $SLURM_ARRAY_TASK_ID in
  1) ############################
     # Histogram server         #
     ############################
    HISTSERVER="MqHistoServer"
    HISTSERVER+=" --control static"
    HISTSERVER+=" --id server1"
    HISTSERVER+=" --severity info"
    HISTSERVER+=" --histport 8081"
    HISTSERVER+=" --channel-config name=histogram-in,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11666,rateLogging=$_ratelog"
    HISTSERVER+=" --channel-config name=histo-conf,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11667,rateLogging=0"
    HISTSERVER+=" --channel-config name=canvas-conf,type=sub,method=bind,transport=zeromq,address=tcp://127.0.0.1:11668,rateLogging=0"
    echo ${_BuildDir}/bin/MQ/histogramServer/$HISTSERVER
    ${_BuildDir}/bin/MQ/histogramServer/$HISTSERVER
    ;;

  2) ############################
     # Sampler                  #
     ############################
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
    SAMPLER+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11666,rateLogging=$_ratelog"
    SAMPLER+=" --channel-config name=missedts,type=pub,method=bind,address=tcp://127.0.0.1:11006,rateLogging=$_ratelog"
    SAMPLER+=" --channel-config name=commands,type=pub,method=bind,address=tcp://127.0.0.1:11007,rateLogging=$_ratelog"
    SAMPLER+=" --transport zeromq"
    echo ${_BuildDir}/bin/MQ/source/$SAMPLER
    ${_BuildDir}/bin/MQ/source/$SAMPLER
    ;;

  3) ############################
     # Parameter server         #
     ############################
    PARAMETERSERVER="parmq-server"
    PARAMETERSERVER+=" --control static"
    PARAMETERSERVER+=" --id parmq-server"
    PARAMETERSERVER+=" --severity info"
    PARAMETERSERVER+=" --channel-name parameters"
    PARAMETERSERVER+=" --channel-config name=parameters,type=rep,method=bind,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
    PARAMETERSERVER+=" --first-input-name $_parfileSts;$_parfileMuch;$_parfileTrdAsic;$_parfileTrdDigi;$_parfileTrdGas;$_parfileTrdGain;$_parfileTof;$_parfileBmon;$_parfileRich;$_parfilePsd"
    PARAMETERSERVER+=" --first-input-type ASCII"
    PARAMETERSERVER+=" --setup $_setup_name"
    echo ${_BuildDir}/bin/MQ/parmq/$PARAMETERSERVER
    ${_BuildDir}/bin/MQ/parmq/$PARAMETERSERVER
    ;;

  4) ############################
     # Event Sink               #
     ############################
    EVTSINK="DigiEventSink"
    EVTSINK+=" --control static"
    EVTSINK+=" --id evtsink1"
    EVTSINK+=" --severity info"
#    EVTSINK+=" --severity debug"
    EVTSINK+=" --StoreFullTs 0"
#    EVTSINK+=" --BypassConsecutiveTs 1"
#    EVTSINK+=" --BypassConsecutiveTs true"
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
    EVTSINK+=" --channel-config name=events,type=pull,method=bind,transport=zeromq,rcvBufSize=1,address=tcp://127.0.0.1:11556,rateLogging=$_ratelog"
    EVTSINK+=" --channel-config name=missedts,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11006,rateLogging=$_ratelog"
    EVTSINK+=" --channel-config name=commands,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11007,rateLogging=$_ratelog"
    EVTSINK+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11666,rateLogging=$_ratelog"
    echo ${_BuildDir}/bin/MQ/mcbm/$EVTSINK
    ${_BuildDir}/bin/MQ/mcbm/$EVTSINK
    ;;

  *) # Processing branches
    (( _iBranch = SLURM_ARRAY_TASK_ID%2 ))
    (( _iBranch -= 4 ))
    (( _iPort = 11680 + _iBranch ))

    if [ $((SLURM_ARRAY_TASK_ID%2)) -eq 1 ]; then
      ##########################
      # Unpacker               #
      ##########################
      UNPACKER="MqUnpack"
      UNPACKER+=" --control static"
      UNPACKER+=" --id unp$_iBranch"
#      UNPACKER+=" --severity error"
      UNPACKER+=" --severity info"
#      UNPACKER+=" --severity debug"
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
#      if [ ${_iBranch} -eq 0 ]; then
#        UNPACKER+=" --FillHistos true"
#      else
#        UNPACKER+=" --FillHistos false"
#      fi
      UNPACKER+=" --TsNameOut unpts$_iBranch"
      UNPACKER+=" --channel-config name=ts-request,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11555,rateLogging=$_ratelog"
      UNPACKER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
      UNPACKER+=" --channel-config name=unpts$_iBranch,type=push,method=bind,transport=zeromq,sndBufSize=1,address=tcp://127.0.0.1:$_iPort,rateLogging=$_ratelog"
#      UNPACKER+=" --channel-config name=commands,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11007"
      UNPACKER+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11666,rateLogging=$_ratelog"
      UNPACKER+=" --transport zeromq"
      echo ${_BuildDir}/bin/MQ/mcbm/$UNPACKER
      ${_BuildDir}/bin/MQ/mcbm/$UNPACKER

    else
        ##########################
        # Event Builder          #
        ##########################
        EVTBUILDER="BuildDigiEvents"
        EVTBUILDER+=" --control static"
        EVTBUILDER+=" --id build$_iBranch"
        EVTBUILDER+=" --severity info"
#        EVTBUILDER+=" --severity debug"
        EVTBUILDER+=" --PubFreqTs $_pubfreqts"
        EVTBUILDER+=" --PubTimeMin $_pubminsec"
        EVTBUILDER+=" --PubTimeMax $_pubmaxsec"
        if [ ${_iBranch} -eq 0 ]; then
          EVTBUILDER+=" --FillHistos true"
        else
          EVTBUILDER+=" --FillHistos false"
        fi
        EVTBUILDER+=" --IgnTsOver false"
        EVTBUILDER+=" --EvtOverMode AllowOverlap"
        EVTBUILDER+=" --RefDet kT0"
        EVTBUILDER+=" --DelDet kMuch"
        EVTBUILDER+=" --DelDet kPsd"

        EVTBUILDER+=" --SetTrigWin kT0,$_TrigWinMinBMon,$_TrigWinMaxBMon"
        EVTBUILDER+=" --SetTrigWin kSts,$_TrigWinMinSts,$_TrigWinMaxSts"
        EVTBUILDER+=" --SetTrigWin kTrd,$_TrigWinMinTrd1d,$_TrigWinMaxTrd1d"
        EVTBUILDER+=" --SetTrigWin kTrd2D,$_TrigWinMinTrd2d,$_TrigWinMaxTrd2d"
        EVTBUILDER+=" --SetTrigWin kTof,$_TrigWinMinTof,$_TrigWinMaxTof"
        EVTBUILDER+=" --SetTrigWin kRich,$_TrigWinMinRich,$_TrigWinMaxRich"

        EVTBUILDER+=" --SetTrigMinNb kT0,$_BmonMin"
        EVTBUILDER+=" --SetTrigMinNb kSts,$_StsMin"
        EVTBUILDER+=" --SetTrigMinNb kTrd,$_Trd1dMin"
        EVTBUILDER+=" --SetTrigMinNb kTrd2D,$_Trd2dMin"
        EVTBUILDER+=" --SetTrigMinNb kTof,$_TofMin"
        EVTBUILDER+=" --SetTrigMinNb kRich,$_RichMin"

        EVTBUILDER+=" --SetTrigMaxNb kT0,$_BmonMax"
        EVTBUILDER+=" --SetTrigMaxNb kSts,$_StsMax"
        EVTBUILDER+=" --SetTrigMaxNb kTrd,$_Trd1dMax"
        EVTBUILDER+=" --SetTrigMaxNb kTrd2D,$_Trd2dMax"
        EVTBUILDER+=" --SetTrigMaxNb kTof,$_TofMax"
        EVTBUILDER+=" --SetTrigMaxNb kRich,$_RichMax"

        EVTBUILDER+=" --SetTrigMinLayersNb kTof,$_TofMinLay"
        EVTBUILDER+=" --SetTrigMinLayersNb kSts,$_StsMinLay"
        EVTBUILDER+=" --TsNameIn unpts$_iBranch"
        EVTBUILDER+=" --EvtNameOut events"
        EVTBUILDER+=" --DoNotSend true"
        EVTBUILDER+=" --channel-config name=unpts$_iBranch,type=pull,method=connect,transport=zeromq,rcvBufSize=1,address=tcp://127.0.0.1:$_iPort,rateLogging=$_ratelog"
        EVTBUILDER+=" --channel-config name=events,type=push,method=connect,transport=zeromq,sndBufSize=1,address=tcp://127.0.0.1:11556,rateLogging=$_ratelog"
#        EVTBUILDER+=" --channel-config name=commands,type=sub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11007"
        EVTBUILDER+=" --channel-config name=parameters,type=req,method=connect,transport=zeromq,address=tcp://127.0.0.1:11005,rateLogging=0"
        EVTBUILDER+=" --channel-config name=histogram-in,type=pub,method=connect,transport=zeromq,address=tcp://127.0.0.1:11666,rateLogging=$_ratelog"
        EVTBUILDER+=" --transport zeromq"
        echo ${_BuildDir}/bin/MQ/mcbm/$EVTBUILDER
        ${_BuildDir}/bin/MQ/mcbm/$EVTBUILDER
    fi
    ;;
esac

sleep 10
