#!/bin/bash
# shell script to apply clusterizer calibrations
#SBATCH -J scan_raw
#SBATCH -D /lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021
#SBATCH --time=48:00:00
#SBATCH --mem=2000
#SBATCH --partition=long
cRun=$1

iCalSet=$2
if [[ "$iCalset" = "" ]]; then 
iCalSet=31041500
fi

((iTmp  = $iCalSet ))
((iBRef = $iTmp % 1000))
((iTmp  = $iTmp - $iBRef))
((iSet  = $iTmp / 1000))
((iRef  = $iTmp % 1000000))
((iRef  = $iRef / 1000))
((iTmp  = $iTmp - $iRef))
((iDut  = $iTmp / 1000000))

iSel2=$3
if [[ "$iSel2" = "" ]]; then 
iSel2=500
fi

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

echo scan_raw for $cRun with iDut=$iDut, iRef=$iRef, iSet=$iCalSet, iSel2=$iSel2, iBRef=$iBRef, Deadtime=$Deadtime

if [ -e /lustre/cbm ]; then
source /lustre/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 
wdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021
outdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021/${cRun}
else 
wdir=`pwd`
outdir=${wdir}/${cRun}
fi

cd $wdir
mkdir $cRun
cd    $cRun 
mkdir Raw
cd Raw
cp    ../../.rootrc .
cp    ../../rootlogon.C .

root -b -q '../../ana_digi_cal_all.C(-1,0,0,'$iBRef',50,"'$cRun'",'$iCalSet',0,'$iSel2','$Deadtime') '


cd ../..

mv -v slurm-${SLURM_JOB_ID}.out ${outdir}/ScanRaw_${cRun}_${iCalSet}_${iSel2}.out
