#!/bin/bash
# shell script to apply clusterizer calibrations
#SBATCH -J trk_cal_digi
#SBATCH -D /lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018
##SBATCH --time=8:00:00
#SBATCH --time=5-24:00:00
#SBATCH --mem=4000
#SBATCH --partition=long

trk_cal_digi() {
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

iCalOpt=$6
if [[ ${iCalOpt} = "" ]]; then
  iCalOpt=1	
fi

iTraSetup=$7
if [[ $iTraSetup = "" ]]; then 
  iTraSetup=1
fi

CalIdSet=$8
if [[ ${CalIdSet} = "" ]]; then
    echo use native calibration file
    CalIdSet=$cCalSet
else
    CalFile=${CalIdMode}_set${CalIdSet}_93_1tofClust.hst.root    
fi


echo trk_cal_digi for $cRun with iDut=$iDut, iRef=$iRef, iSet=$iCalSet, iSel2=$iSel2, iBRef=$iBRef, Deadtime=$Deadtime, CalFile=$CalFile

if [[ $iShLev = "" ]]; then 
  iShLev=0
  nEvt=200000
  dDTres=100000
  dDTRMSres=100000
fi 

echo execute trk_cal_digi at shell level $iShLev

if [ -e /lustre/cbm ]; then
source /lustre/cbm/users/nh/CBM/cbmroot/trunk/build/config.sh 
wdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018
outdir=/lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2018/${cRun}
else 
wdir=`pwd`
outdir=${wdir}/${cRun}
fi

iter=0;

cd $wdir
mkdir $cRun
cd    $cRun 
cp    ../.rootrc .
cp    ../rootlogon.C .

echo Execute in `pwd` at $iShLev: ./trk_cal_digi.sh $1 $2 $3 $4 $5 $6 $7 $8

# get initial digi calibration 
#cp -v  ./I*/${CalFile}  .
 
# get latest tracker offsets
# cp -v ../${cRun}_tofFindTracks.hst.root .

rm -v TCalib.res
nEvtMax=0
(( nEvtMax = nEvt*10 ))

#frange1 limits DT spectrum range 
fRange1=2.
# frange2 limits chi2
fRange2=4.0
TRange2Limit=3. 

#iSel=911921
iSel=10020
iGenCor=3
cCalSet2=${cCalSet}_$cSel2

case $iCalOpt in
  1)   # TOff
  ;;
  2)   # Walk
  (( nEvt *= 10 ))
  (( nEvtMax *= 10 ))
  ;;
esac

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
  
  iCalAct=$iCalOpt
  iIter=0
  echo Enter while loop with CalAct $iCalAct in dir `pwd`
  while [[ $iCalAct -gt 0 ]]; do  
    cd $wdir/$cRun
    echo Current loop with CalAct $iCalAct and CalOpt $iCalOpt
    if [[ $iCalOpt = 1 ]] || [[ $iCalAct -gt 1 ]]; then 
      root -b -q '../ana_digi_cal.C('$nEvt',93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalIdMode'") '
      # update calibration parameter file, will only be active in next iteration 
      if [[ $iIter = -10 ]] && [[ $iCalOpt = 1 ]]; then  # exploratory option when iIter set to 0 
        echo Update Calibration file from ana_digi_cal
        cp -v tofClust_${cRun}_set${cCalSet}.hst.root ../${cRun}_set${cCalSet}_93_1tofClust.hst.root
        echo 20000 > TOffAvOff.res
        echo 20000 > TOffAvRMS.res
      else
        root -b -q '../ana_trks.C('$nEvt','$iSel','$iGenCor',"'$cRun'","'$cCalSet2'",'$iSel2','$iTraSetup','$fRange1','$fRange2','$Deadtime',"'$CalIdMode'",1,1,'$iCalSet','$iCalAct')'
      #root -l 'ana_trksi.C(-1,10,1,"385.50.5.0","000014500_020",20,1,1.90,7.60,50,"385.50.5.0",1,1)'
  
        cp -v New_${CalFile} ${CalFile}  
      fi
    else 
      cd $wdir
      # store current status 
      dLDTres=$dDTres
      dLDTRMSres=$dDTRMSres
      iLCalOpt=$iCalOpt
      echo Store limits $dLDTres, $dLDTRMSres
      (( iShLev += 1 ))
      echo exec in `pwd` at level $iShLev: trk_cal_digi $1 $2 $3 $4 $5 1 $7
      trk_cal_digi $1 $2 $3 $4 $5 1 $7
      (( iShLev -= 1 ))
      # restore old status
      dDTres=$dLDTres
      dDTRMSres=$dLDTRMSres
      iCalOpt=$iLCalOpt
      echo exec1done, resume old CalOpt $iCalOpt with status $dDTres, $dDTRMSres
    fi
    (( iCalAct -= 1 ))
    (( iIter   += 1 ))
    echo Continue while loop with CalAct $iCalAct and CalOpt $iCalOpt
  done
  
  cd $wdir/$cRun
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
  echo CMPR result_summary: $compare_result 

#  if [ $iter = 1 ]; then 
#    exit 0  # for debugging 
#  fi

  if [[ $compare_result > 0 ]]; then
    if [[ $Tres = 0 ]]; then
      Tres=1
    fi
    dDTres=$Tres
    dDTRMSres=$TRMSres
    echo Store new res values $dDTres, $dDTRMSres
    (( dDTRMSres -= 1 ))  # next attempt should be at least 1ps better for continuation
    cp -v New_${CalFile} ${CalFile}  
    cp -v New_${CalFile} ${CalFile}_$iter  
  else
    dDTres=0
  fi
  (( iter += 1 ))
done

cd $wdir/$cRun
# generate full statistics digi file 
if [[ $iShLev = 0 ]]; then 
  root -b -q '../ana_digi_cal.C(-1,93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalIdMode'") '
fi

cd $wdir

if [[ $iShLev = 0 ]]; then 
  mv -v slurm-${SLURM_JOB_ID}.out ${outdir}/TrkCalDigi_${cRun}_${iCalSet}_${iSel2}_${CalIdMode}.out
fi

} #end of function body

trk_cal_digi $1 $2 $3 $4 $5 $6 $7 $8
