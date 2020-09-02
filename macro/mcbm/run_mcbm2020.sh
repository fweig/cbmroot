#!/bin/bash
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
Sys="auau"
fi

Ebeam=$2
if [[ ${Ebeam} = "" ]]; then
Ebeam="1.24gev"
fi

Centr=$3
if [[ ${Centr} = "" ]]; then
Centr="mbias"
fi

mcbmGeo=$4
if [[ ${mcbmGeo} = "" ]]; then
mcbmGeo="mcbm_beam_2020_03"
fi

cMode=$5
if [[ ${cMode} = "" ]]; then
cMode="0E0"
fi

iStep=${cMode:0:1}
iBase=${cMode:1:1}
iCut=${cMode:2:1}
iRate=${cMode:3:1}

case $iRate in
   0)
     EvtRate=1.0e+05
     ;;
   1)
     EvtRate=1.0e+06
     ;;
   2)
     EvtRate=1.0e+07
     ;;
   *)
     EvtRate=1.0e+05
     ;;
esac

echo simulate Step $iStep with Base $iBase and cut $iCut at rate $EvtRate 

#EvtRate=1.0e+05   # 1/s
TSLength=1.e4  # ns 
Tint=100      # ns 
ReqTofMul=2
#NEvt=100      # for debugging
NEvt=100000 # for production

if [ "$iBase" = "T" ]; then 
 Timebased=kTRUE
else 
 Timebased=kFALSE
fi
 
if [[ -e /lustre ]]; then 
    source /lustre/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 
    export wdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/mcbm
    export outdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2020/data
    export indir=/lustre/cbm/prod/gen/urqmd
    export extdir=/lustre/cbm/users/uhlig/mcbm_proposal/data
else
    export wdir=.
    export indir=../../input
    export extdir=../../../../../../uhlig/mcbm_proposal/data
    export outdir=./data
fi

cd  ${wdir}

mcfile=${indir}/${Sys}/${Ebeam}/${Centr}/urqmd.${Sys}.${Ebeam}.${Centr}.${XXX}.root
RunId=${mcbmGeo}.${Sys}.${Ebeam}.${Centr}.${XXX}
datfile=$outdir/$RunId

if [  ! -e $outdir ]; then 
  echo create outdir $outdir 
  mkdir $outdir 
fi

if [ $iStep -gt 2 ]; then 
  echo Generate file with MC tracking data
  root -q -b 'mcbm_transport.C('$NEvt',"'${mcbmGeo}'","'$datfile'","'$mcfile'")'
#  exit
fi 

if [[ "$Timebased" = "kTRUE" ]]; then
## time based mode
  echo simulate in time based mode 
  #cp -v $extdir/${RunId}.*par.root $outdir/
  #root -q -b 'mcbm_digi.C('$NEvt',"'$RunId'","'$extdir'","'$outdir'","'${mcbmGeo}'",kFALSE,'$EvtRate','$TSLength')'
  if [ $iStep -gt 1 ]; then 
    root -q -b 'mcbm_digi.C('$NEvt',"'$RunId'","'$outdir'","'$outdir'","'${mcbmGeo}'",kFALSE,'$EvtRate','$TSLength')'
  fi
  if [ $iStep -gt 0 ]; then 
    root -q -b 'mcbm_reco_event_tb.C('$NEvt',"'$RunId'","'$outdir'","'$outdir'","'${mcbmGeo}'",kTRUE,'$EvtRate','$TSLength')'
  fi
  RunId=$RunId.$EvtRate.$Tint.$ReqTofMul
else 
## event mode
  echo simulate in event mode 
  ##cp -v $extdir/${RunId}.*par.root $outdir/
  ##root -q -b 'mcbm_digi.C('$NEvt',"'$RunId'","'$extdir'","'$outdir'","'${mcbmGeo}'",kTRUE,'$EvtRate','$TSLength')'
  if [ $iStep -gt 1 ]; then 
    root -q -b 'mcbm_digi.C('$NEvt',"'$RunId'","'$outdir'","'$outdir'","'${mcbmGeo}'",kTRUE,'$EvtRate','$TSLength')'
  fi
  if [ $iStep -gt 0 ]; then 
    root -q -b 'mcbm_reco_event_tb.C('$NEvt',"'$RunId'","'$outdir'","'$outdir'","'${mcbmGeo}'",kFALSE,'$EvtRate','$TSLength')'
  fi
  #exit
fi

## analysis 
echo Analyze $RunId

cd $outdir
ln -s mcbm*93_1tofClust.hst.root ../
cd .. 
echo execute analysis in directory `pwd` 
#cp ../.rootrc .
#cp ../rootlogon.C .
if [ ! -e data ]; then 
  mkdir data
fi

root -l './ana_trks_eval.C('$NEvt',30040,4,"'$RunId'","030040500_500",500,1,3.2,5.2,50,"XXX",1,1,30040500,0,1)'

# for input from FLorian, use $extdir as 3. argument
#root -q -b 'mcbm_hadron_analysis.C('$NEvt',"'$RunId'","'$outdir'","'$outdir'","'${mcbmGeo}'",'$Timebased','$EvtRate','$TSLength','$Tint','$ReqTofMul','$iCut')'
# tof tracking 
  
mv -v slurm-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out ${outdir}/run_${RunId}.out
