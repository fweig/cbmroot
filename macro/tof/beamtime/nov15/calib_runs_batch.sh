#!/bin/bash
#SBATCH --time=3-00:00:00
#SBATCH --mem-per-cpu=3072
#SBATCH --cpus-per-task=1
#SBATCH --partition=long
#SBATCH --array=0-90
#SBATCH --job-name="calib"
#SBATCH --comment="CBM TOF"
#SBATCH --workdir=/lustre/nyx/cbm/users/csimon/analysis/nov15
#SBATCH --open-mode=truncate

# The  batch  script may contain options preceded with "#SBATCH" before any executable
# commands in the script.  After the first line of the batch script, which typically
# identifies  the  shell  to be used, sbatch will stop processing options at the first
# line which does NOT begin with "#SBATCH".

# SLURM partitions on cluster kronos (/etc/slurm-llnl/slurm.conf)
# main
#   Default=YES
#   DefaultTime=01:00:00
#   MaxTime=08:00:00
#   DefMemPerCPU=2048
#   MaxMemPerCPU=3072
# long
#   Default=NO
#   DefaultTime=01:00:00
#   MaxTime=7-00:00:00
#   DefMemPerCPU=2048
#   MaxMemPerCPU=3072

# Supported replacement symbols in SBATCH options
# %A     Job array's master job allocation number.
# %a     Job array ID (index) number.
# %j     Job allocation number.
# %N     Node name.  Only one file is created, so %N will be replaced by the name of the 
#        first node in the job, which is the one that runs the script.
# %u     User name.

# Some SLURM environment variables available in this batch script
# SLURM_ARRAY_TASK_ID : Job array ID (index) number.
# SLURM_ARRAY_JOB_ID  : Job array's master job ID number.
# SLURM_JOB_ID        : The ID of the job allocation.
# SLURM_JOB_NAME      : Name of the job.
# SLURM_JOB_PARTITION : Name of the partition in which the job is running.
# SLURM_SUBMIT_DIR    : The directory from which sbatch was invoked.
# SLURM_SUBMIT_HOST   : The hostname of the computer from which sbatch was invoked.

if [ ! -z "$SLURM_SUBMIT_DIR" ]; then
  wdir=`pwd`
else
  wdir=/lustre/nyx/cbm/users/csimon/analysis/nov15
  origdir=`pwd`
fi


#source /lustre/nyx/cbm/users/csimon/cbmroot/trunk_v1606b_may16/bin/CbmRootConfig.sh
source /lustre/nyx/cbm/users/csimon/cbmroot/trunk/bld/v1606b_may16/config.sh

if [ ! -z "$1" -a "${#1}" == "9" ]; then
  iCalSet=$1
else
  echo 'no valid calibration set specified'
  exit 1
fi

if [ ! -z "$2" -a "${#2}" == "3" ]; then
  iSel2=$2
else
  echo 'no valid Sel2 MRPC specified'
  exit 1
fi

if [ -z "$3" ]; then
  if [ ! -z "$SLURM_SUBMIT_DIR" ]; then

    RunList=' 
    27Nov2115
    28Nov0001
    28Nov0041
    28Nov0126
    28Nov0220
    28Nov0258
    28Nov0330
    28Nov0404
    28Nov0449
    28Nov0528
    28Nov0606
    28Nov0645
    28Nov0728
    28Nov0829
    28Nov0946
    28Nov1026
    28Nov1129
    28Nov1231
    28Nov1234
    28Nov1256
    28Nov1300
    28Nov1328
    28Nov1445
    28Nov1527
    28Nov1637
    28Nov1851
    28Nov1936
    28Nov1957
    28Nov2030
    28Nov2141
    28Nov2220
    28Nov2322
    29Nov0009
    29Nov0043
    29Nov0120
    29Nov0206
    29Nov0248
    29Nov0319
    29Nov0351
    29Nov0425
    29Nov0525
    29Nov0600
    29Nov0625
    29Nov0626
    29Nov0652
    29Nov0718
    29Nov0750
    29Nov0821
    29Nov0932
    29Nov0952
    29Nov1202
    29Nov1257
    29Nov1609
    29Nov1803
    29Nov1903
    29Nov1958
    29Nov2041
    29Nov2103
    30Nov0123
    30Nov0234
    30Nov0312
    30Nov0404
    30Nov0904
    30Nov1027
    30Nov1134
    30Nov1222
    30Nov1317
    30Nov1355
    30Nov1506
    30Nov1541
    30Nov1619
    30Nov1637
    30Nov1740
    30Nov1816
    30Nov1851
    30Nov1933
    30Nov2003
    30Nov2031
    30Nov2134
    01Dec0045
    01Dec0109
    01Dec0128
    01Dec0146
    01Dec0206
    01Dec0224
    01Dec0225
    01Dec0241
    01Dec0258
    01Dec0317
    01Dec0351
    01Dec0427
    '

    I=0
    RunId=""
    FoundRunId=""
    for RunId in $RunList
    do
      if [ "$I" == "$SLURM_ARRAY_TASK_ID" ]; then
        echo "found RunId $RunId"
        FoundRunId=1
        break
      fi

      ((I=$I+1))
    done

    if [ -z "$FoundRunId" ]; then
      echo 'too many array tasks allocated'
      exit 1
    fi  

  else
    echo 'can calibrate multiple runs on the batch farm only'
    exit 1
  fi
else
  RunId=$3
  if [ ! -z "$SLURM_SUBMIT_DIR" ]; then
    if [ "$SLURM_ARRAY_TASK_ID" != "0" ]; then
      echo 'more than one array task allocated'
      exit 0
    fi
  fi
fi



((iTmp  = $iCalSet ))
((iBRef = $iTmp % 1000))
((iTmp  = $iTmp - $iBRef))
((iSet  = $iTmp / 1000))
((iMRef = $iTmp % 1000000))
((iMRef = $iMRef / 1000))
((iTmp  = $iTmp - $iMRef))
((iDut  = $iTmp / 1000000))


outdir=${wdir}/CbmTofSps_${RunId}/Calib_${iCalSet}_${iSel2}
mkdir ${outdir}

if [ ! -d "${outdir}" ]; then
  echo 'calib directory does not exist and cannot be created'
  exit 1
else

  cp -v ${VMCWORKDIR}/macro/tof/beamtime/nov15/.rootrc ${outdir}/.rootrc
  cp -v ${VMCWORKDIR}/macro/tof/beamtime/nov15/rootlogon.C ${outdir}/rootlogon.C

  source ${VMCWORKDIR}/macro/tof/beamtime/nov15/init_calib_digi.sh
  source ${VMCWORKDIR}/macro/tof/beamtime/nov15/iter_calib_digi.sh
  source ${VMCWORKDIR}/macro/tof/beamtime/nov15/gen_calib_digi.sh

  if [ ! -z "$SLURM_SUBMIT_DIR" ]; then
    mv -v ${wdir}/slurm-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out ${outdir}/calib_CbmTofSps_${RunId}_${iCalSet}_${iSel2}.log
  else
    cd ${origdir}
  fi

fi
