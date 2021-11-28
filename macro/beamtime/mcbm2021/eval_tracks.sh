#!/bin/bash
# Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Florian Uhlig

# shell script to iterate tracklet calibration histograms
#SBATCH -J eval_tracks
#SBATCH -D /lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021
#SBATCH --time=6-00:00:00
#SBATCH --mem=4000
#SBATCH --partition=long

X=$((${SLURM_ARRAY_TASK_ID} - 0))
XXX=$(printf "%03d" "$X")

cRun=$1
iDut=$2 
iRef=$3 
iSel2=$4
((iSel=$iDut*1000+$iRef))
iTraSetup=$5

cSet=$6
if [[ ${cSet} = "" ]]; then
    cSet="030040500_500"
fi
   
# extract iCalSet from cSet
i1=0
while [ "${cSet:$i1:1}" = "0" ]; do
(( i1 += 1 ))
done
i2=0
while [ "${cSet:$i2:1}" != "_" ] && [ $i2 -lt  ${#cSet} ]; do
(( i2 += 1 ))
done
(( i2 -= i1 ))
iCalSet=${cSet:$i1:$i2}
echo got i1=$i1, i2=$i2, iCalSet=$iCalSet from $cSet

CalIdMode=$7
if [[ ${CalIdMode} = "" ]]; then
    echo use native calibration file
    cCalId=$cRun;
else
    cCalId=${CalIdMode}
fi

cCalRef=$8
if [[ ${cCalRef} = "" ]]; then
    cCalRef=${cSet:0:9};
    echo use default CalSet $cCalRef
fi

dDTres=10000000
nEvt=2000000

cSel2=$iSel2;
if [[ $iSel2 < 100 ]]; then
    cSel2="0"$iSel2
    if [[ $iSel2 < 10 ]]; then
	cSel2="00"$iSel2
    fi
fi

iMc=0
McId=${cRun:0:4}
if [ "$McId" = "mcbm" ]; then 
  echo processing MC simulation
  iMc=1
fi
  
if [ -e /lustre/cbm ]; then
source /lustre/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 
wdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021
outdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021/${cRun}
else 
wdir=`pwd`
outdir=${wdir}/${cRun}
fi

# frange2 limits chi2
fRange2=2.5

#frange1 limits DT spectrum range 
fRange1=0.9
#dDeadtime=00
dDeadtime=50

#./gen_digi.sh 600.100.5.0 30040500 500 50    600.100.5.0  30040500
#./gen_digi.sh $cRun $iCalSet $iSel2 $Deadtime $CalIdMode CalIdSet

cd $wdir 
echo look for calfile: ls -1 ${cCalId}*set${cCalRef}_93_1tofClust.hst.root
digiCalFile=`ls -1 ./${cCalId}*set${cCalRef}_93_1tofClust.hst.root`
DigiCalFile=`pwd`/$digiCalFile
echo DigiCalFile=$DigiCalFile

if [ ! -e ${cRun} ]; then 
  mkdir $cRun
fi
cd ${cRun}
mkdir             Ana_${cSet}_${iSel}_${cSel2}_${iTraSetup}
cp ../rootlogon.C Ana_${cSet}_${iSel}_${cSel2}_${iTraSetup}/
cp ../.rootrc     Ana_${cSet}_${iSel}_${cSel2}_${iTraSetup}/

cd Ana_${cSet}_${iSel}_${cSel2}_${iTraSetup}
rm -v  *AnaTestBeam.hst.root
cp -v ../../${cCalId}_tofFindTracks.hst.root .
echo create symbolic link to DigiCalFile $DigiCalFile in `pwd`
rm -v ./$digiCalFile
ln -s -v $DigiCalFile ./$digiCalFile

while [[ $dDTres > 0 ]]; do

for iCal in 1 2 3 5 6 7 8 1
do

root -b -q '../../ana_trks_eval.C('$nEvt','$iSel',-1,"'$cRun'","'$cSet'",'$iSel2','$iTraSetup','$fRange1','$fRange2','$dDeadtime',"'$cCalId'",'$iCal',0,'$iCalSet',0,'$iMc')'
mv -v tofAnaTestBeam.hst.root ${cRun}_TrkAnaTestBeam.hst.root
rm all_*

if (! (test -f Test.res)); then
  echo no resolution file available: exit
  exit 1
fi
done

Tres=`cat Test.res`
dTdif=`echo "$dDTres - $Tres" | bc`
dDTres=`echo "$dDTres - 0.005" | bc`
compare_result=`echo "$Tres < $dDTres" | bc`

echo got Tres = $Tres, compare to $dDTres, dTdif = $dTdif, compare_result = $compare_result

if [[ $compare_result > 0 ]]; then
dDTres=$Tres
else
dDTres=0
fi

done

# final action -> scan full statistics 
iCal=1
root -b -q '../../ana_trks_eval.C(-1,'$iSel',-1,"'$cRun'","'$cSet'",'$iSel2','$iTraSetup','$fRange1','$fRange2','$dDeadtime',"'$cCalId'",'$iCal',0,'$iCalSet',1,'$iMc')'
rm all_*
cd ../..

#mv -v slurm-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out ${outdir}/IterTrack_${cRun}_${cSet}.out
mv -v slurm-${SLURM_JOB_ID}.out ${outdir}/EvalTrack_${cRun}_${cSet}_${iSel}_${iSel2}_${iTraSetup}.out
