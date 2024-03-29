#!/bin/bash
# Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Florian Uhlig

# shell script to apply clusterizer calibrations
#SBATCH -J gen_hits
#SBATCH -D /lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021
#SBATCH --time=8:00:00
#SBATCH --mem=2000
##SBATCH --partition=long
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
 ln -s ${CalIdMode}/${CalFile} ${RunFile}  
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


echo gen_hits for $cRun with iDut=$iDut, iRef=$iRef, iSet=$iCalSet, iSel2=$iSel2, iBRef=$iBRef, Deadtime=$Deadtime, CalId=$CalId, CalFile=$CalFile

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
#root -b -q '../ana_digi_cal_evt.C('$Nevt',93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalId'") '
root -b -q '../ana_digi_cal_all.C('$Nevt',93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalId'") '
#root -b -q '../ana_digi_cal.C(100000,93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalIdMode'") '
#root -b -q '../ana_digi_cos.C(-1,93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalIdMode'") '
#root -b -q '../ana_digi_star.C(-1,93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalIdMode'") '

cd ..

mv -v slurm-${SLURM_JOB_ID}.out ${outdir}/GenHits_${cRun}_${iCalSet}_${iSel2}_${CalIdMode}.out
