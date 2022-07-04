#!/bin/bash
# Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# author: Norbert Herrmann

# shell script to apply clusterizer calibrations
#SBATCH -J reco_mcbm
#SBATCH -D /lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2022
#SBATCH --time=8:00:00
#SBATCH --mem=8000
##SBATCH --partition=long

FTs=$((${SLURM_ARRAY_TASK_ID} - 0))
if [[ ${FTs} = "" ]]; then
    echo Start at Ts 0
    FTs=0
fi

cRun=$1

NTs=$2  # number of time slices 
if [[ ${NTs} = "" ]]; then
    echo use all events
    NTs=-1
fi

(( FTs *= NTs )) 

FId=$3
if [[ ${FId} = "" ]]; then
    echo use default
    FId="5.lxbk0598"
fi

echo mcbm_digievents_reco for Task_Id $X, run $cRun, FirstTs  $FTs, NumberTs $NTs, FId $FId

if [ -e /lustre/cbm ]; then
source /lustre/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh -a
wdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2022
outdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2022/rec
else 
wdir=`pwd`
outdir=${wdir}/${cRun}
fi

cd $wdir

root -b -q './mcbm_digievent_reco.C('$cRun','$NTs','$FTs',"'$FId'")'

#cd ..

mv -v slurm-${SLURM_JOB_ID}_${FTs}.out ${outdir}/RecoDigiEvents_${cRun}_${FTs}_${NTs}.out
