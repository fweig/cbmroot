#!/bin/bash
# Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Florian Uhlig

#SBATCH -J run_mcbm
#SBATCH -D /lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/mcbm
#SBATCH --time=8:00:00
##SBATCH --mem=2000
##SBATCH --partition=long
##SBATCH --partition=debug

X=$((${SLURM_ARRAY_TASK_ID} - 0))
if [ $X -eq 0 ]; then 
X=1
fi

XXX=$(printf "%05d" "$X")

Sys=$1
if [[ ${Sys} = "" ]]; then
Sys="nini"
fi

Ebeam=$2
if [[ ${Ebeam} = "" ]]; then
Ebeam="1.93gev"
fi

Centr=$3
if [[ ${Centr} = "" ]]; then
Centr="mbias"
fi

mcbmGeo=$4
if [[ ${mcbmGeo} = "" ]]; then
mcbmGeo="mcbm_beam_2021_03"
fi

cMode=$5
if [[ ${cMode} = "" ]]; then
cMode="2T0"
fi

iStep=${cMode:0:1}
iBase=${cMode:1:1}
iCut=${cMode:2:1}

echo simulate Step $iStep with Base $iBase and cut $iCut 

EvtRate=1.e6   # 1/s
TSLength=1.e4  # ns 
Tint=100.      # ns 
ReqTofMul=2
NEvt=10      # for debugging
#NEvt=100000 # for production

if [ "$iBase" = "T" ]; then 
 Timebased=kTRUE
else 
 Timebased=kFALSE
fi
 
if [[ -e /lustre ]]; then 
    source /lustre/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 
    export wdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/mcbm
    export outdir=/lustre/cbm/users/nh/mc/mcbm2021
    export indir=/lustre/cbm/prod/gen/urqmd
#    export extdir=/lustre/cbm/users/uhlig/mcbm_proposal/data
    export extdir=$outdir 
    mcfile=${indir}/${Sys}/${Ebeam}/${Centr}/urqmd.${Sys}.${Ebeam}.${Centr}.${XXX}.root
else
    export wdir=.
    export indir=../../input
#    export extdir=../../../../../../uhlig/mcbm_proposal/data
    export outdir=./data
    export extdir=$outdir 
    mcfile=${indir}/urqmd.${Sys}.${Ebeam}.${Centr}.${XXX}.root
fi

cd  ${wdir}

RunId=${mcbmGeo}.${Sys}.${Ebeam}.${Centr}.${XXX}
datfile=$outdir/$RunId

if [ $iStep -gt 2 ]; then 
  echo Generate file with MC tracking data
  root -q -b 'mcbm_transport_nh.C('$NEvt',"'${mcbmGeo}'","'$datfile'","'$mcfile'")'
#  exit
fi 

if [[ "$Timebased" = "kTRUE" ]]; then
## time based mode
  echo simulate in time based mode 
  #cp -v $extdir/${RunId}.*par.root $outdir/
  #root -q -b 'mcbm_digi.C('$NEvt',"'$RunId'","'$extdir'","'$outdir'","'${mcbmGeo}'",kFALSE,'$EvtRate','$TSLength')'
  if [ $iStep -gt 1 ]; then 
    root -q -b 'mcbm_digi_nh.C('$NEvt',"'$RunId'","'$outdir'","'$outdir'","'${mcbmGeo}'",kFALSE,'$EvtRate','$TSLength')'
  fi
  if [ $iStep -gt 0 ]; then 
    root -q -b 'mcbm_reco_event_tb_nh.C('$NEvt',"'$RunId'","'$outdir'","'$outdir'","'${mcbmGeo}'",kTRUE,'$EvtRate','$TSLength')'
  fi
    
else 
## event mode
  echo simulate in event mode 
  ##cp -v $extdir/${RunId}.*par.root $outdir/
  ##root -q -b 'mcbm_digi.C('$NEvt',"'$RunId'","'$extdir'","'$outdir'","'${mcbmGeo}'",kTRUE,'$EvtRate','$TSLength')'
  if [ $iStep -gt 1 ]; then 
    root -q -b 'mcbm_digi_nh.C('$NEvt',"'$RunId'","'$outdir'","'$outdir'","'${mcbmGeo}'",kTRUE,'$EvtRate','$TSLength')'
  fi
  if [ $iStep -gt 0 ]; then 
    root -q -b 'mcbm_reco_event_tb_nh.C('$NEvt',"'$RunId'","'$outdir'","'$outdir'","'${mcbmGeo}'",kFALSE,'$EvtRate','$TSLength')'
  fi
  #exit
fi
## analysis 
# for input from FLorian, use $extdir as 3. argument
root -q -b 'mcbm_hadron_analysis_nh.C('$NEvt',"'$RunId'","'$outdir'","'$outdir'","'${mcbmGeo}'",'$Timebased','$EvtRate','$TSLength','$Tint','$ReqTofMul','$iCut')'

  
mv -v slurm-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out ${outdir}/run_${RunId}.out
