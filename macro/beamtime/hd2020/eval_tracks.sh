#!/bin/bash
# shell script to iterate tracklet calibration histograms
#SBATCH -J eval_tracks
#SBATCH -D /lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/hd2020
#SBATCH --time=1-00:00:00
#SBATCH --mem=2000
#SBATCH --partition=long

X=$((${SLURM_ARRAY_TASK_ID} - 0))
XXX=$(printf "%03d" "$X")

cRun=$1
iDut=$2; 
iRef=$3; 
iSel2=$4
((iSel=$iDut*1000+$iRef))
iTraSetup=$5

cSet=$6
if [[ ${cSet} = "" ]]; then
    cSet="900920910_911"
fi
    
CalIdMode=$7
if [[ ${CalIdMode} = "" ]]; then
    echo use native calibration file
    cCalId=$cRun;
else
    cCalId=${CalIdMode}
fi

dDTres=10000000
nEvt=100000

cSel2=$iSel2;
if [[ $iSel2 < 100 ]]; then
    cSel2="0"$iSel2
    if [[ $iSel2 < 10 ]]; then
	cSel2="00"$iSel2
    fi
fi

if [ -e /lustre/cbm ]; then
source /lustre/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 
wdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/hd2020
outdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/hd2020/${cRun}
else 
wdir=`pwd`
outdir=${wdir}/${cRun}
fi

# frange2 limits chi2
fRange2=3.5

#frange1 limits DT spectrum range 
fRange1=1.1
dDeadtime=50

cd ${cRun}
mkdir          Ana_${cSet}_${iSel}_${cSel2}_${iTraSetup}
cp ../rootlogon.C Ana_${cSet}_${iSel}_${cSel2}_${iTraSetup}/
cp ../.rootrc     Ana_${cSet}_${iSel}_${cSel2}_${iTraSetup}/
cd Ana_${cSet}_${iSel}_${cSel2}_${iTraSetup}
rm -v  *AnaTestBeam.hst.root
cp -v ../../${cCalId}_tofFindTracks.hst.root .
cSet9=${cSet:0:9}
ln -s -v ../${cCalId}_set${cSet9}_93_1tofClust.hst.root  ${cCalId}_set${cSet9}_93_1tofClust.hst.root 

while [[ $dDTres > 0 ]]; do

for iCal in 1 2 3 5 6 7 8 1
do

root -b -q '../../ana_trks.C('$nEvt','$iSel',-1,"'$cRun'","'$cSet'",'$iSel2','$iTraSetup','$fRange1','$fRange2','$dDeadtime',"'$cCalId'",'$iCal')'
mv -v tofAnaTestBeam.hst.root ${cRun}_TrkAnaTestBeam.hst.root
rm all_*

if (! (test -f Test.res)); then
echo no resolution file available: scan full statistics and exit
iCal=1
root -b -q '../../ana_trks.C(-1,'$iSel',-1,"'$cRun'","'$cSet'",'$iSel2','$iTraSetup','$fRange1','$fRange2','$dDeadtime',"'$cCalId'",'$iCal')'
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
root -b -q '../../ana_trks.C(-1,'$iSel',-1,"'$cRun'","'$cSet'",'$iSel2','$iTraSetup','$fRange1','$fRange2','$dDeadtime',"'$cCalId'",'$iCal')'
rm all_*
cd ../..

#mv -v slurm-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out ${outdir}/IterTrack_${cRun}_${cSet}.out
mv -v slurm-${SLURM_JOB_ID}.out ${outdir}/EvalTrack_${cRun}_${cSet}_${iSel}_${iSel2}_${iTraSetup}.out
