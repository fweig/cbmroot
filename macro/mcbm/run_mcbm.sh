#!/bin/bash
#SBATCH -J run_mcbm
#SBATCH -D /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/mcbm
#SBATCH --time=8:00:00
##SBATCH --mem=2000
##SBATCH --partition=long

X=$((${SLURM_ARRAY_TASK_ID} - 0))
XXX=$(printf "%03d" "$X")

if [[ ${Sys} = "" ]]; then
Sys="$1"
fi

if [[ ${Ebeam} = "" ]]; then
Ebeam="$2"
fi

if [[ ${Centr} = "" ]]; then
Centr="$3"
fi

if [[ ${TofGeo} = "" ]]; then
mcbmGeo="$4"
fi


if [[ -e /lustre/nyx ]]; then 
source /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/build6/config.sh 

export wdir=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/mcbm
export outdir=/lustre/nyx/cbm/users/nh/mc
else
export wdir=.
export outdir=./data
fi

cd  ${wdir}
root -q -b 'mcbm_mc.C(100000,0,"'${Sys}'","'${Ebeam}'","'${Centr}'",'${X}',"'${mcbmGeo}'")'

root -q -b 'mcbm_reco.C(100000,"'${Sys}'","'${Ebeam}'","'${Centr}'",'${X}',"'${mcbmGeo}'")'

root -q -b 'mcbm_Ana.C(100000,"'${Sys}'","'${Ebeam}'","'${Centr}'",'${X}')'
  
mv -v slurm-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out ${outdir}/runSTAR_${Sys}_${Ebeam}_${Centr}_$X.out
