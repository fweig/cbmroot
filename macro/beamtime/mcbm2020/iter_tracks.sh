#!/bin/bash
# shell script to iterate tracklet calibration histograms
#SBATCH -J track
#SBATCH -D /lustre/nyx/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2020
#SBATCH --time=6-00:00:00
#SBATCH --mem=4000
#SBATCH --partition=long

X=$((${SLURM_ARRAY_TASK_ID} - 0))
XXX=$(printf "%03d" "$X")

cRun=$1
iTraSetup=$2

#which file should be analyzed ?  
cSet=$3
if [[ $cSet = "" ]]; then 
    cSet="000014500_500"
    #cSet="900041500_901"
    #cSet="900041500_500"
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

cCalId=$4;
if [[ $cCalId = "" ]]; then 
    cCalId=$cRun;
fi

iMc=0
McId=${cRun:0:4}
if [ "$McId" = "mcbm" ]; then 
  echo processing MC simulation
  iMc=1
fi

# what should be done ?
iDut=900; iRef=41; iSel2=31
((iSel=$iDut*1000+$iRef))

nEvt=100000
dDTres=2000
dDTRMSres=2000
iter=0;

if [ -e /lustre/cbm ]; then
source /lustre/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 
wdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2020
outdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2020/${cRun}
else 
wdir=`pwd`
outdir=${wdir}/${cRun}
fi

# frange2 limits chi2
fRange2=9.
TRange2Limit=3. 

#frange1 limits DT spectrum range 
fRange1=4.
TRange1Limit=2.
dDeadtime=50

if [ ! -e ${cRun} ]; then 
  mkdir $cRun
fi
cd ${cRun}
cp ../.rootrc .
cp ../rootlogon.C .

# clean directory for start from scratch
# rm -v ${cRun}_tofFindTracks.hst.root
rm -v TCalib.res

if [[ $iter > 0 ]]; then
 cp -v  ${cRun}_tofFindTracks.hst${iter}.root  ${cRun}_tofFindTracks.hst.root
fi

nEvtMax=0
(( nEvtMax = nEvt*10 ))

while [[ $dDTres > 0 ]]; do

nEvt=`echo "scale=0;$nEvt * 1./1." | bc`
#nEvt=`echo "scale=0;$nEvt * 1.1/1." | bc`

if [ $nEvt -gt $nEvtMax ]; then
    nEvt=$nEvtMax
fi

#((fRange2 /= 2))
#if((${fRange2}<$Range2Limit));then
# ((fRange2=$Range2Limit))
#fi
fRange2=`echo "$fRange2 * 0.8" | bc`
compare_TRange2=`echo "$fRange2 < $TRange2Limit" | bc`
if  [[ $compare_TRange2 > 0 ]]; then
fRange2=$TRange2Limit
fi

#bash only handles integers!!
#((fRange1 /= 2))  
#if((${fRange1}<1));then
# ((fRange1=1))
#fi
fRange1=`echo "$fRange1 * 0.8" | bc`
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
    if [ $iter -eq 0 ] && [ $iMc -eq 1 ]; then
      echo skip iCal $iCal for MC calibration
      iCal=5
    fi
    while [[ $nIt > 0 ]]; do
	((iter += 1))
	root -b -q '../ana_trks.C('$nEvt','$iSel','$iCal',"'$cRun'","'$cSet'",'$iSel2','$iTraSetup','$fRange1','$fRange2','$dDeadtime',"'$cCalId'",1,1,'$iCalSet',0,'$iMc')'
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

if [[ $Tres = 0 ]]; then
    Tres=1
fi
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
  rm ../${cRun}_tofFindTracks.hst.root
  cp -v  tofFindTracks.hst.root  ../${cRun}_tofFindTracks.hst.root
  cp -v  tofFindTracks.hst.root  ./${cRun}_${cSet}._${iTraSetup}_tofFindTracks.hst.root  # keep a copy 
  rm ../${cRun}_TrkAnaTestBeam.hst.root
  cp -v  tofAnaTestBeam.hst.root ../${cRun}_TrkAnaTestBeam.hst.root
fi

done

cd ..
#mv -v slurm-${SLURM_ARRAY_JOB_ID}_${SLURM_ARRAY_TASK_ID}.out ${outdir}/IterTrack_${cRun}_${cSet}.out
mv -v slurm-${SLURM_JOB_ID}.out ${outdir}/IterTrack_${cRun}_${cSet}.out

