#!/bin/bash
# Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Florian Uhlig

# shell script to apply clusterizer calibrations
#SBATCH -J EvalRaw
#SBATCH -D /lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021
#SBATCH --time=48:00:00
#SBATCH --mem=4000
#SBATCH --partition=long
cRun=$1

iCalSet=$2
((iTmp  = $iCalSet ))
((iBRef = $iTmp % 1000))
((iTmp  = $iTmp - $iBRef))
((iSet  = $iTmp / 1000))
((iRef  = $iTmp % 1000000))
((iRef  = $iRef / 1000))
((iTmp  = $iTmp - $iRef))
((iDut  = $iTmp / 1000000))

iSel2=$3

cCalSet=$iCalSet
if (( iCalSet<100000000 )); then 
cCalSet="0"$iCalSet
fi
if (( iCalSet<10000000 )); then 
cCalSet="00"$iCalSet
fi
if (( iCalSet<1000000 )); then 
cCalSet="000"$iCalSet
fi
if (( iCalSet<100000 )); then 
cCalSet="0000"$iCalSet
fi
echo cCalSet = $cCalSet

Deadtime=$4
if [[ ${Deadtime} = "" ]]; then
Deadtime=50.
fi

Nevt=$5
if [[ ${Nevt} = "" ]]; then
    echo use all events
    Nevt=-1
fi

CalIdMode=$6
if [[ ${CalIdMode} = "" ]]; then
 echo use native calibration file 
 CalIdMode=${cRun}
 CalFile=${cRun}_set${cCalSet}_93_1tofClust.hst.root
else 
 CalFile=${CalIdMode}_set${cCalSet}_93_1tofClust.hst.root
 RunFile=${cRun}_set${cCalSet}_93_1tofClust.hst.root
 rm ${RunFile}
 ln -s ${CalFile} ${RunFile} 
 echo use calibrations from  ${CalFile}
fi

#CalIdSet=$7
#if [[ ${CalIdSet} = "" ]]; then
#    echo use native calibration file
#    CalIdSet=$cCalSet
#else
#    CalFile=${CalIdMode}_set${CalIdSet}_93_1tofClust.hst.root    
#fi
#CalId=${CalIdMode}_set${CalIdSet}
CalId=${CalIdMode}

# fixed parameter, TODO: make them input variables 
iSel=$7
if [[ $iSel = "" ]]; then 
  iSel=900041
fi

iTrackingSetup=$8
if [[ $iTrackingSetup = "" ]]; then 
#iTrackingSetup=42  # for Jun2021
  iTrackingSetup=4  # for May2021  
fi 

iSel22=$9
if [[ $iSel22 = "" ]]; then 
  iSel22=31
fi 

echo eval_raw for $cRun with iDut=$iDut, iRef=$iRef, iSet=$iCalSet, iSel2=$iSel2, iBRef=$iBRef, Deadtime=$Deadtime, CalId=$CalId, CalFile=$CalFile 
echo eval_raw scan $Nevt events for iSel=$iSel, iSel22=$iSel22, iTrackingSetup=$iTrackingSetup

if [ -e /lustre/cbm ]; then
source /lustre/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh -a
wdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021
outdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021/${cRun}
else 
wdir=`pwd`
outdir=${wdir}/${cRun}
fi

cd $wdir
mkdir $cRun
cd    $cRun 
cp    ../.rootrc .
cp    ../rootlogon.C .
cp -v ../${CalFile}  .
FindTracksFile=./${CalId}_tofFindTracks.hst.root
if [ ! -e $FindTracksFile ]; then 
 echo link FindTracksFile to $FindTracksFile
 ln -s -v ../${CalId}_tofFindTracks.hst.root $FindTracksFile
fi

AnaCalFile=./${cRun}_TrkAnaTestBeam.hst.root
rm $AnaCalFile
if [ -e $AnaCalFile ]; then 
 echo AnaCalFile $AnaCalFile existing, check!
 exit 0
else
 echo link AnaCalFile to $AnaCalFile
 ln -s -v ../$CalId/Ana_${cCalSet}_${iBRef}_${iSel}_${iSel22}_${iTrackingSetup}/${CalId}_TrkAnaTestBeam.hst.root $AnaCalFile 
fi

#Int_t iPlot = 0,
#Int_t iSel = 910041, Int_t iSel22=31, Int_t iTrackingSetup=10, Int_t iGenCor=1, Double_t dScalFac=1,
#Double_t dChi2Lim2=3., Bool_t bUseSigCalib=kFALSE, Int_t iCalOpt=1, Int_t iAnaCor=1, Int_t iTrkPar=0, Int_t iMc=0
 
root -b -q '../eval_raw.C('$Nevt',93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalId'",'$iSel','$iSel22','$iTrackingSetup') '

rm -v ./$AnaCalFile
rm -v ./$FindTracksFile
cd ..

mv -v slurm-${SLURM_JOB_ID}.out ${outdir}/EvalRaw_${cRun}_${iCalSet}_${iSel2}_${CalIdMode}_${iSel}_${iSel22}_${iTrackingSetup}.out
