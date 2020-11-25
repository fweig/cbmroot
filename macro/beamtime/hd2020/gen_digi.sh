#!/bin/bash
# shell script to apply clusterizer calibrations
#SBATCH -J gen_digi
#SBATCH -D /lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/hd2020
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

CalIdMode=$5
if [[ ${CalIdMode} = "" ]]; then
 echo use native calibration file 
 CalIdMode=${cRun}
 CalFile=${cRun}_set${cCalSet}_93_1tofClust.hst.root
else 
 CalFile=${CalIdMode}_set${cCalSet}_93_1tofClust.hst.root
 RunFile=${cRun}_set${cCalSet}_93_1tofClust.hst.root
# rm ${RunFile}
# ln -s ${CalFile} ${RunFile} 
 echo use calibrations from  ${CalFile}
fi

CalIdSet=$6
if [[ ${CalIdSet} = "" ]]; then
    echo use native calibration file
    CalIdSet=$cCalSet
else
    CalFile=${CalIdMode}_set${CalIdSet}_93_1tofClust.hst.root    
fi

Nevt=$7
if [[ ${Nevt} = "" ]]; then
    echo use all events
    Nevt=-1
fi

echo gen_digi for $cRun with iDut=$iDut, iRef=$iRef, iSet=$iCalSet, iSel2=$iSel2, iBRef=$iBRef, Deadtime=$Deadtime, CalFile=$CalFile

if [ -e /lustre/cbm ]; then
source /lustre/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 
wdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/hd2020
outdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/hd2020/${cRun}
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
#root -b -q '../ana_digi_cal.C(100000,93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalIdMode'") '
root -b -q '../ana_digi_cal.C('$Nevt',93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalIdMode'") '
#root -b -q '../ana_digi_cos.C(-1,93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalIdMode'") '
#root -b -q '../ana_digi_star.C(-1,93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalIdMode'") '

cd ..

mv -v slurm-${SLURM_JOB_ID}.out ${outdir}/GenDigi_${cRun}_${iCalSet}_${iSel2}_${CalIdMode}.out
