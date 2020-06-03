#!/bin/bash
#SBATCH --time=0-03:00:00
#SBATCH --mem-per-cpu=3072
#SBATCH --cpus-per-task=1
#SBATCH --partition=long
#SBATCH --array=0-4
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

if [ ! -z "$1" ]; then
  RunId=$1
else
  echo 'no run to calibrate specified'
  exit 1
fi

if [ -z "$2" ]; then
  if [ ! -z "$SLURM_SUBMIT_DIR" ]; then

    InSetList=' 
    601600500_000
    601600510_000
    901900500_000
    920921500_000
    920921510_000
    '

    I=0
    InSet=""
    FoundInSet=""
    for InSet in $InSetList
    do
      if [ "$I" == "$SLURM_ARRAY_TASK_ID" ]; then
        echo "found InSet $InSet"
        FoundInSet=1
        break
      fi

      ((I=$I+1))
    done

    if [ -z "$FoundInSet" ]; then
      echo 'too many array tasks allocated'
      exit 1
    fi

   iCalSet=${InSet:0:9}
   iSel2=${InSet:10:3}

  else
    echo 'can run multiple calibration sets on the batch farm only'
    exit 1
  fi
elif [ -z "$3" ]; then
  echo 'no Sel2 MRPC specified'
  exit 1
elif [ ! "${#2}" == "9" -o ! "${#3}" == "3" ]; then
  echo 'no valid calibration set or no valid Sel2 MRPC specified'
  exit 1
else
  iCalSet=$2
  iSel2=$3
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

#  source ${VMCWORKDIR}/macro/tof/beamtime/nov15/init_calib_digi.sh
#  source ${VMCWORKDIR}/macro/tof/beamtime/nov15/iter_calib_digi.sh
  source ${VMCWORKDIR}/macro/tof/beamtime/nov15/gen_calib_digi.sh

  if [ ! -z "$SLURM_SUBMIT_DIR" ]; then
    mv -v ${wdir}/slurm-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out ${outdir}/calib_CbmTofSps_${RunId}_${iCalSet}_${iSel2}.log
  else
    cd ${origdir}
  fi

fi
