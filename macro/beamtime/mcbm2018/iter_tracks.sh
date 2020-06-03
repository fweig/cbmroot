#!/bin/bash
# shell script to iterate tracklet calibration histograms
#SBATCH -J track
#SBATCH -D /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018
#SBATCH --time=6-00:00:00
#SBATCH --mem=4000
#SBATCH --partition=long

X=$((${SLURM_ARRAY_TASK_ID} - 0))
XXX=$(printf "%03d" "$X")

cRun=$1
#cRun='r0047_20170915_1636_DT50_Req3'
#cRun='r0002_20171215_1810_DT50_Req-2'
iDut=0; iRef=10; iSel2=20
#iDut=911; iRef=900; iSel2=-910

# what should be done ?
((iSel=$iDut*1000+$iRef))
#which file should be analyzed ?  
#cSet="010020000_020"
cSet="000010500_020"
#cSet="020040030_030"

cCalId=$cRun;
iTraSetup=3
nEvt=1000000
dDTres=2000
dDTRMSres=2000
iter=0;

if [ -e /lustre/nyx ]; then
source /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/build6/config.sh 
wdir=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018
outdir=/lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018/${cRun}
else 
wdir=`pwd`
outdir=${wdir}/${cRun}
fi

# frange2 limits chi2
fRange2=5.5
TRange2Limit=3.5

#frange1 limits DT spectrum range 
fRange1=1.5
TRange1Limit=0.8
dDeadtime=50

mkdir ${cRun}
cd ${cRun}
cp ../.rootrc .
cp ../rootlogon.C .

# clean directory for start from scratch
rm -v ${cRun}_tofFindTracks.hst.root
rm -v TCalib.res

if [[ $iter > 0 ]]; then
 cp -v  ${cRun}_tofFindTracks.hst${iter}.root  ${cRun}_tofFindTracks.hst.root
fi
 
while [[ $dDTres > 0 ]]; do

nEvt=`echo "$nEvt * 1.3" | bc`

#((fRange2 /= 2))
#if((${fRange2}<$Range2Limit));then
# ((fRange2=$Range2Limit))
#fi
fRange2=`echo "$fRange2 * 0.95" | bc`
compare_TRange2=`echo "$fRange2 < $TRange2Limit" | bc`
if  [[ $compare_TRange2 > 0 ]]; then
fRange2=$TRange2Limit
fi

#bash only handles integers!!
#((fRange1 /= 2))  
#if((${fRange1}<1));then
# ((fRange1=1))
#fi
fRange1=`echo "$fRange1 * 0.95" | bc`
compare_TRange=`echo "$fRange1 < $TRange1Limit" | bc`
if  [[ $compare_TRange > 0 ]]; then
fRange1=$TRange1Limit
fi

# correction modes: 2 - TOff from Tt, 3 - Pull t, 4 - x, 5 - y, 6 - z, >10 - Pull t of individual stations 
#for iCal in 3 2 10 11 12 13 14 15 4 5; do
for iCal in 3 4 5; do
#for iCal in 3 2 4; do
#for iCal in 3 2 ; do
#for iCal in 2 ; do
    nIt=1
    while [[ $nIt > 0 ]]; do
	((iter += 1))
	root -b -q '../ana_trks.C('$nEvt','$iSel','$iCal',"'$cRun'","'$cSet'",'$iSel2','$iTraSetup','$fRange1','$fRange2','$dDeadtime',"'$cCalId'")'
	cp -v tofFindTracks.hst.root ${cRun}_tofFindTracks.hst.root
	cp -v tofFindTracks.hst.root ${cRun}_tofFindTracks.hst${iter}.root
	cp -v tofAnaTestBeam.hst.root ${cRun}_TrkAnaTestBeam.hst.root
	((nIt -= 1))
    done
done

iTres=`cat TCalib.res`
if [[ $iTres = 0 ]]; then
    echo All tracks lost, stop at iter = $iter
    return
fi

((TRMSres=$iTres%1000))
((iTres -= TRMSres ))
((Tres   = iTres / 1000)) 
dTdif=`echo "$dDTres - $Tres" | bc`
compare_result=`echo "$Tres < $dDTres" | bc`

dTRMSdif=`echo "$dDTRMSres - $TRMSres" | bc`
compare_RMS=`echo "$TRMSres < $dDTRMSres" | bc`

echo at iter=$iter got TOff = $Tres, compare to $dDTres, dTdif = $dTdif, result = $compare_result, TRMS = $TRMSres, old $dDTRMSres, dif = $dTRMSdif, result = $compare_RMS 

((compare_result += $compare_RMS))
echo result_summary: $compare_result 

if [[ $compare_result > 0 ]]; then
  if [[ $Tres = 0 ]]; then
    Tres=1
  fi
  dDTres=$Tres
  dDTRMSres=$TRMSres
else
  dDTres=0
  cp -v  tofFindTracks.hst.root  ../${cRun}_tofFindTracks.hst.root
  cp -v  tofAnaTestBeam.hst.root ../${cRun}_TrkAnaTestBeam.hst.root
fi

done


mv -v slurm-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out ${outdir}/Calib_${cRun}_${cSet}.out

