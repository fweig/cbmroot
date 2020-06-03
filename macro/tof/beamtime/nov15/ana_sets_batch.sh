#!/bin/bash
#SBATCH --time=1-00:00:00
#SBATCH --mem-per-cpu=3072
#SBATCH --cpus-per-task=1
#SBATCH --partition=long
#SBATCH --array=0-39
#SBATCH --job-name="ana"
#SBATCH --comment="CBM TOF"
#SBATCH --workdir=/lustre/nyx/cbm/users/csimon/analysis/nov15
#SBATCH --output=slurm-%j-%A_%a.out
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
  echo 'no run to analyze specified'
  exit 1
fi

if [ ! -z "$2" -a "${#2}" == "13" ]; then
  InSet=$2
else
  echo 'no valid calibration set specified'
  exit 1
fi

if [ ! -z "$3" ] && [[ "$3" =~ [0-9]\.[0-9] ]]; then
  dScalFac=$3
else
  echo 'no valid scaling factor specified'
  exit 1
fi


if [ -z "$4" ]; then
  if [ ! -z "$SLURM_SUBMIT_DIR" ]; then

    AnaSetList=' 
    300_400_920
    300_400_921
    300_920_400
    300_920_921
    300_921_400
    300_921_920
    400_920_300
    400_920_921
    400_921_300
    400_921_920
    900_400_920
    900_400_921
    900_920_300
    900_920_400
    900_920_921
    900_921_300
    900_921_400
    900_921_920
    901_400_920
    901_400_921
    901_920_300
    901_920_400
    901_920_921
    901_921_300
    901_921_400
    901_921_920
    910_400_920
    910_400_921
    910_920_300
    910_920_400
    910_920_921
    910_921_300
    910_921_400
    910_921_920
    920_400_921
    920_921_300
    920_921_400
    921_400_920
    921_920_300
    921_920_400
    '

    I=0
    AnaSet=""
    FoundAnaSet=""
    for AnaSet in $AnaSetList
    do
      if [ "$I" == "$SLURM_ARRAY_TASK_ID" ]; then
        echo "found AnaSet $AnaSet"
        FoundAnaSet=1
        break
      fi

      ((I=$I+1))
    done

    if [ -z "$FoundAnaSet" ]; then
      echo 'too many array tasks allocated'
      exit 1
    fi

    iDut=${AnaSet:0:3}
    iMRef=${AnaSet:4:3}
    iSel2=${AnaSet:8:3}

  else
    echo 'can run multiple analysis sets on the batch farm only'
    exit 1
  fi
elif [ -z "$5" ]; then
  echo 'no MRef MRPC specified'
  exit 1
elif [ -z "$6" ]; then
  echo 'no Sel2 MRPC specified'
  exit 1
elif [ ! "${#4}" == "3" ] || [ ! "${#5}" == "3" ] || [ ! "${#6}" == "3" ]; then
  echo 'no valid Dut, MRef or Sel2 MRPC specified'
  exit 1
else
  iDut=$4
  iMRef=$5
  iSel2=$6
  if [ ! -z "$SLURM_SUBMIT_DIR" ]; then
    if [ "$SLURM_ARRAY_TASK_ID" != "0" ]; then
      echo 'more than one array task allocated'
      exit 0
    fi
  fi
fi


outdir=${wdir}/CbmTofSps_${RunId}/Calib_${InSet}/Ana_${iDut}_${iMRef}_${iSel2}_${dScalFac}
mkdir ${outdir}

if [ ! -d "${outdir}" ]; then
  echo 'ana directory does not exist and cannot be created'
  exit 1
else

  cp -v ${VMCWORKDIR}/macro/tof/beamtime/nov15/.rootrc ${outdir}/.rootrc
  cp -v ${VMCWORKDIR}/macro/tof/beamtime/nov15/rootlogon.C ${outdir}/rootlogon.C

  source ${VMCWORKDIR}/macro/tof/beamtime/nov15/iter_ana_hits.sh

  if [ ! -z "$SLURM_SUBMIT_DIR" ]; then
    mv -v ${wdir}/slurm-${SLURM_JOB_ID}-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out ${outdir}/ana_CbmTofSps_${RunId}_${iDut}_${iMRef}_${iSel2}_${InSet}_${dScalFac}.log
  else
    cd ${origdir}
  fi

fi
