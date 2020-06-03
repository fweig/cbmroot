#!/bin/bash
# shell script to apply clusterizer calibrations
#SBATCH -J trk_cal_digi
#SBATCH -D /lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2020
#SBATCH --time=4:00:00
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
cSel2=$iSel2
if (( iSel2<100 )); then 
cSel2="0"$iSel2
fi
if (( iSel2<10 )); then 
cSel2="00"$iSel2
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

echo trk_cal_digi for $cRun with iDut=$iDut, iRef=$iRef, iSet=$iCalSet, iSel2=$iSel2, iBRef=$iBRef, Deadtime=$Deadtime, CalFile=$CalFile

if [ -e /lustre/cbm ]; then
source /lustre/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 
wdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2020
outdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2020/${cRun}
else 
wdir=`pwd`
outdir=${wdir}/${cRun}
fi

nEvt=200000
dDTres=100000
dDTRMSres=100000
iter=0;

cd $wdir
mkdir $cRun
cd    $cRun 
cp    ../.rootrc .
cp    ../rootlogon.C .
# get initial digi calibration 
 cp -v  ./I*/${CalFile}  .
# get latest tracker offsets
# cp -v ../${cRun}_tofFindTracks.hst.root .

rm -v TCalib.res
nEvtMax=0
(( nEvtMax = nEvt*10 ))
iTraSetup=1
#frange1 limits DT spectrum range 
fRange1=3.
# frange2 limits chi2
fRange2=5.0
TRange2Limit=2. 

iSel=10
iGenCor=3
cCalSet2=${cCalSet}_$cSel2
iCalOpt=1 

case $iCalOpt in
  1)   # TOff
  ;;
  2)   # Walk
  (( nEvt *= 10 ))
  ;;
esac

while [[ $dDTres > 0 ]]; do
  nEvt=`echo "scale=0;$nEvt * 1./1." | bc`
  #nEvt=`echo "scale=0;$nEvt * 1.1/1." | bc`
  if [ $nEvt -gt $nEvtMax ]; then
    nEvt=$nEvtMax
  fi

  fRange2=`echo "$fRange2 * 0.8" | bc`
  compare_TRange2=`echo "$fRange2 < $TRange2Limit" | bc`
  if  [[ $compare_TRange2 > 0 ]]; then
   fRange2=$TRange2Limit
  fi

  root -b -q '../ana_digi_cal.C('$nEvt',93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalIdMode'") '

  root -b -q '../ana_trks.C('$nEvt','$iSel','$iGenCor',"'$cRun'","'$cCalSet2'",'$iSel2','$iTraSetup','$fRange1','$fRange2','$Deadtime',"'$CalIdMode'",1,1,'$iCalSet','$iCalOpt')'
  #root -l 'ana_trksi.C(-1,10,1,"385.50.5.0","000014500_020",20,1,1.90,7.60,50,"385.50.5.0",1,1)'

#  iTres=`cat TCalib.res`
#  if [[ $iTres = 0 ]]; then
#    echo All tracks lost, stop at iter = $iter
#    return
#  fi

#  ((TRMSres=$iTres%1000))
#  ((iTres -= TRMSres ))
#  ((Tres   = iTres / 1000)) 

  Tres=`cat TOffAvOff.res`
  TRMSres=`cat TOffAvRMS.res`

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
    cp -v New_${CalFile} ${CalFile}  
  else
    dDTres=0
  fi
  (( iter += 1 ))
done

cd ..

mv -v slurm-${SLURM_JOB_ID}.out ${outdir}/TrkCalDigi_${cRun}_${iCalSet}_${iSel2}_${iCalIdMode}.out
