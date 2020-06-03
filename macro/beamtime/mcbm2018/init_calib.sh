#!/bin/bash
# shell script to initialize clusterizer calibrations
cRun=$1

nIter=1
echo 'Initialize clusterizer calibration for run '$cRun, execute $nIter iterations

iCalSet=$2
((iTmp  = $iCalSet ))
((iBRef = $iTmp % 1000))
((iTmp  = $iTmp - $iBRef))
((iSet  = $iTmp / 1000))
((iMRef = $iTmp % 1000000))
((iMRef = $iMRef / 1000))
((iTmp  = $iTmp - $iMRef))
((iDut  = $iTmp / 1000000))
echo Calib setup is ${iCalSet}, iSet=$iSet, iDut=$iDut, iMRef=$iMRef, iBRef=$iBRef
cCalSet="000010020"
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
#iSet=0
#lastOpt=''
nEvi=1000000
mkdir ${cRun}
cp rootlogon.C ${cRun}
cp .rootrc ${cRun}
cd ${cRun}

# Global variables, for for-loops. Runs from 1-15 and 16-79 in two for-loops
#iRestart=32
iRestart=0
iStep=0
iStepLast=0
# ************************** Starting while Loop ***************************** #
while [[ $nIter > 0 ]]; do 

echo execute main loop at $nIter. iteration 

for inOpt in ''$nEvi',0,0,0,50' ''$nEvi',13,0,0,50' ''$nEvi',13,0,0,50' ''$nEvi',13,1,'${iMRef}',0' ''$nEvi',13,0,'${iDut}',0' ''$nEvi',13,1,'${iMRef}',0'  ''$nEvi',13,0,'${iDut}',0' ''$nEvi',23,0,0,50' ''$nEvi',23,0,0,50' ''$nEvi',23,1,'${iMRef}',0'  ''$nEvi',23,0,'${iDut}',0' ''$nEvi',23,1,'${iMRef}',0' ''$nEvi',23,0,'${iDut}',0' ''$nEvi',33,0,0,50' ''$nEvi',33,0,0,50' ''$nEvi',33,1,'${iMRef}',0' ''$nEvi',33,0,'${iDut}',0' ''$nEvi',33,0,0,50' ''$nEvi',33,1,'${iMRef}',0' ''$nEvi',33,0,'${iDut}',0' ''$nEvi',14,0,-'${iDut}',0' ''$nEvi',33,0,'${iDut}',0'  ''$nEvi',14,1,-'${iMRef}',0' ''$nEvi',33,1,'${iMRef}',0' ''$nEvi',33,0,0,50' ''$nEvi',33,0,0,50' ''$nEvi',43,0,0,50' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',43,0,0,50' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',53,0,0,50' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',53,0,0,50' ''$nEvi',24,0,-'${iDut}',0' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',24,1,-'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',53,0,0,50' ''$nEvi',63,0,0,50' ''$nEvi',63,1,'${iMRef}',0' ''$nEvi',63,0,'${iDut}',0' ''$nEvi',63,0,0,50' ''$nEvi',63,1,'${iMRef}',0' ''$nEvi',63,0,'${iDut}',0' ''$nEvi',73,0,0,50' ''$nEvi',73,0,'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' ''$nEvi',73,0,0,50' ''$nEvi',34,0,-'${iDut}',0' ''$nEvi',73,0,'${iDut}',0' ''$nEvi',34,1,-'${iMRef}',0' ''$nEvi',73,1,'${iMRef}',0' ''$nEvi',73,0,0,50' ''$nEvi',83,0,'${iDut}',0'  ''$nEvi',83,1,'${iMRef}',0' ''$nEvi',83,0,0,50' ''$nEvi',83,0,'${iDut}',0'  ''$nEvi',83,1,'${iMRef}',0' ''$nEvi',44,0,-'${iDut}',0'  ''$nEvi',83,0,'${iDut}',0'  ''$nEvi',83,1,'${iMRef}',0' ''$nEvi',44,1,-'${iMRef}',0' ''$nEvi',83,0,0,50' ''$nEvi',83,0,'${iDut}',0'  ''$nEvi',83,1,'${iMRef}',0' ''$nEvi',93,0,0,50' ''$nEvi',93,0,'${iDut}',0' ''$nEvi',93,1,'${iMRef}',0'  
do  

((iStepLast = ${iStep}))
((iStep += 1))
mkdir Init${iStep}
cp rootlogon.C Init${iStep}
cp .rootrc Init${iStep}
cd Init${iStep}

if [[ ${lastOpt:+1} ]] ; then
# echo last round was done with $lastOpt, extract 2. and 3. word
i1=`expr index $inOpt , `
i2=($i1+3)
#echo `expr index $inOpt , ` = $i1
cMode=${inOpt:$i1:2}
cSel=${inOpt:$i2:1}
echo next iteration: cMode=$cMode, cSel=$cSel 
if [[ ${cSel} = "-" ]];then 
    cSel=${inOpt:$i2:2}
    echo cSel=$cSel 
    cSel="0"
fi
#copy calibration file 
if (($iStep > $iRestart)) ; then
  cp -v ../Init${iStepLast}/tofClust_${cRun}_set${cCalSet}.hst.root ${cRun}_set${cCalSet}_${cMode}_${cSel}tofClust.hst.root
fi
fi 

lastOpt=$inOpt
# generate new calibration file
if (($iStep > $iRestart)) ; then 
  root -b -q '../../ana_digi_cal.C('$inOpt',"'${cRun}'",'${iCalSet}',0,'${iBRef}') '
#  root -b -q '../../ana_digi_cos.C('$inOpt',"'${cRun}'",'${iCalSet}',0,'${iBRef}') '
#  root -b -q '../../ana_digi_star.C('$inOpt',"'${cRun}'",'${iCalSet}',0,'${iBRef}') '

  cp -v tofClust_${cRun}_set${cCalSet}.hst.root ../${cRun}_set${cCalSet}_${cMode}_${cSel}tofClust.hst.root
  cp *pdf ../
#./screenshot.sh
  cd .. 
  rm ../${cRun}_set${cCalSet}_${cMode}_${cSel}tofClust.hst.root
  ln -s ./${cRun}/${cRun}_set${cCalSet}_${cMode}_${cSel}tofClust.hst.root ../${cRun}_set${cCalSet}_${cMode}_${cSel}tofClust.hst.root
echo Init step $iStep with mode ${cMode}, option $inOpt  finished
else 
  cd ..
  echo Init step $iStep with mode ${cMode}, option $inOpt  skipped
fi
done
(( nIter -= 1))
done 
#
