#!/bin/bash
# shell script to reconstruct mcbm mtof data
#SBATCH -J mtof_reco
#SBATCH -D /lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2020
#SBATCH --time=2-24:00:00
#SBATCH --mem=4000
#SBATCH --partition=long
cRun=$1
dataset="data/unp_mcbm_"$cRun

NTs=$2
if [[ ${NTs} = "" ]]; then
  NTs=20000
fi

ReqTofMul=$3
if [[ ${ReqTofMul} = "" ]]; then
  ReqTofMul=0  
fi

setup=$4
if [[ ${setup} = "" ]]; then
  setup=mcbm_beam_2020_03
fi

CalId=$5
if [[ ${CalId} = "" ]]; then
  CalId=759.100.4.0
fi

CalSet=$6
if [[ ${CalSet} = "" ]]; then
  CalSet=10020500
fi

if [ -e /lustre/cbm ]; then
source /lustre/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 
wdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2020
outdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2020/${cRun}
else 
wdir=`pwd`
outdir=${wdir}/${cRun}
fi

cd $wdir
mkdir $cRun
#cd    $cRun 
#cp    ../.rootrc .
#cp    ../rootlogon.C .
root -l -b -q 'mtof_reco.C('$NTs',"'$dataset'","'$setup'","'$CalId'",'$CalSet','$ReqTofMul')'
#cd ..

mv -v slurm-${SLURM_JOB_ID}.out ${outdir}/RecoMtof_${NTs}_${cRun}_${CalId}_${CalSet}.out
