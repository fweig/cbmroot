#!/bin/bash
# @file align_tof.sh
# @copyright Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# * @license SPDX-License-Identifier: GPL-3.0-only
# * @authors Norbert Herrmann [orginator] **/

# shell script to iterate tracklet calibration histograms
#SBATCH -J align
#SBATCH -D /lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021
#SBATCH --time=8:00:00
##SBATCH --time=6-00:00:00
#SBATCH --mem=4000
##SBATCH --partition=long

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

cCalIdset=$5;
if [[ $cCalIdset = "" ]]; then 
    cCalIdset=${cSet:1:9};
fi

iTrkPar=$6
if [[ $iTrkPar = "" ]]; then 
  #iTrkPar=0   # beam
  #iTrkPar=1   # beam in limited acceptance (test counter in 2-stack)
  iTrkPar=2  # cosmics
  #iTrkPar=3   # July2021 acceptance of CRI -mTOF
fi 

nEvt=$7
if [[ $nEvt = "" ]]; then 
  nEvt=2000000  # default event number
fi

iMc=0
McId=${cRun:0:4}
if [ "$McId" = "mcbm" ]; then 
  echo processing MC simulation
  iMc=1
fi

iSel=$8
if [[ $iSel = "" ]]; then 
  iSel=12002
fi
iSel2=-1

dTOffScal=$9
if [[ $dTOffScal = "" ]]; then 
  dTOffScal=1.
fi


dDTres=200000
dDTRMSres=200000
iter=0;

if [ -e /lustre/cbm ]; then
source /lustre/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh -a
wdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021
outdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021/${cRun}
else 
wdir=`pwd`
outdir=${wdir}/${cRun}
fi

# frange2 limits chi2
fRange2=9.
TRange2Limit=4. 

#frange1 limits DT spectrum range 
fRange1=5.
TRange1Limit=2.5
dDeadtime=50

if [ ! -e ${cRun} ]; then 
  mkdir $cRun
fi
cd ${cRun}
cp ../.rootrc .
cp ../rootlogon.C .

# clean directory for start from scratch
#rm -v ${cRun}_tofFindTracks.hst.root
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

fRange2=`echo "$fRange2 * 0.9" | bc`
compare_TRange2=`echo "$fRange2 < $TRange2Limit" | bc`
if  [[ $compare_TRange2 > 0 ]]; then
  fRange2=$TRange2Limit
fi

fRange1=`echo "$fRange1 * 0.9" | bc`
compare_TRange=`echo "$fRange1 < $TRange1Limit" | bc`
if  [[ $compare_TRange > 0 ]]; then
fRange1=$TRange1Limit
fi

# correction modes: 2 - TOff from Tt, 3 - Pull t, 4 - x, 5 - y, 6 - z, >10 - Pull t of individual stations 
#for iCal in 3 2 10 11 12 13 14 15 4 5; do
#for iCal in 3 2 4 5; do
#for iCal in 3 2 4; do
#for iCal in 3 2 ; do
#for iCal in 3 80 81; do
for iCal in 3 ; do
#for iCal in 3 5; do
#for iCal in 4 3 ; do  # cosmic
    nIt=1
    if [ $iter -eq 0 ] && [ $iMc -eq 1 ]; then
      echo skip iCal $iCal for MC calibration
      iCal=5
    fi
    while [[ $nIt > 0 ]]; do
	((iter += 1))
	root -b -q '../ana_trks.C('$nEvt','$iSel','$iCal',"'$cRun'","'$cSet'",'$iSel2','$iTraSetup','$fRange1','$fRange2','$dDeadtime',"'$cCalId'",1,0,'$iCalSet',1,'$iTrkPar','$dTOffScal','$iMc')'
	cp -v tofFindTracks.hst.root ${cRun}_tofFindTracks.hst.root
	cp -v tofFindTracks.hst.root ${cRun}_tofFindTracks.hst${iter}.root
	cp -v tofAnaTestBeam.hst.root ${cRun}_TrkAnaTestBeam.hst.root
	dTOffScal=1.
	((nIt -= 1))
    done
done

iTres=`cat TCalib.res`
if [[ $iTres = 0 ]]; then
    echo All tracks lost, stop at iter = $iter
    return
fi

((TRMSres=$iTres%10000))
((iTres -= TRMSres ))
((Tres   = iTres / 10000)) 

if [[ $Tres = 0 ]]; then
    Tres=1
fi
dTdif=`echo "$dDTres - $Tres" | bc`
compare_result=`echo "$Tres < $dDTres" | bc`

dTRMSdif=`echo "$dDTRMSres - $TRMSres" | bc`
compare_RMS=`echo "$TRMSres < $dDTRMSres" | bc`

echo `date`: iter=$iter got TOff = $Tres, compare to $dDTres, dTdif = $dTdif, result = $compare_result, TRMS = $TRMSres, old $dDTRMSres, dif = $dTRMSdif, result = $compare_RMS 

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
mv -v slurm-${SLURM_JOB_ID}.out ${outdir}/AlignTof_${cRun}_${cSet}_${iTraSetup}_${iTrkPar}.out

