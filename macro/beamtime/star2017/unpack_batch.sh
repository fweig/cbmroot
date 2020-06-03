#!/bin/bash
#SBATCH -J unpack
#SBATCH -D /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/star2017
#SBATCH --time=8:00:00
##SBATCH --mem=2000
##SBATCH --partition=long

X=$((${SLURM_ARRAY_TASK_ID} - 0))
XXX=$(printf "%03d" "$X")

DeltaT=$1
if [[ ${DeltaT} = "" ]]; then
DeltaT=50.
fi

ReqDet=$2
if [[ ${ReqDet} = "" ]]; then
ReqDet=0
fi

EpSupp=$3
if [[ ${EpSupp} = "" ]]; then
EpSupp=1
fi

RunId="$4"
if [[ ${RunId} = "" ]]; then
    RunList1=' 
r0001_20170121_2310_0000
'

    I=1
    RunId=""
    for RunId in $RunList1
    do
    #echo check $RunId
     if (($I == $X)); then
       echo found RunId $RunId
       break
     fi
     ((I=$I+1))
    done
fi

if [ -e /lustre/nyx ]; then
echo define root execution environment at I=$I for Run $RunId

source /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/build6/config.sh 
wdir=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/star2017
outdir=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/star2017/${RunId}
else 
wdir=`pwd`
outdir=${wdir}/${RunId}
fi
mkdir ${outdir}

cd  ${wdir}
#root -b -q './unpack_tsa.C(-1,'$DeltaT','$ReqDet','$EpSupp',"'$RunId'") '
root -b -q './unpack_tsa.C(10000,'$DeltaT','$ReqDet','$EpSupp',"'$RunId'") '

mv -v slurm-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out ${outdir}/Unpack_$DeltaT_$ReqDet_${RunId}.out
