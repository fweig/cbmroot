#!/bin/bash
# Copyright (C) 2016 PI-UHd,GSI
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Norbert Herrmann

# shell script to iterate clusterizer calibrations 

#cRun=$1
#cRun='CbmTofPiHd_09Aug1533'
#cRun='CbmTofPiHd_10Aug1728'
#cRun='CbmTofPiHd_11Aug1224'
#cRun='CbmTofPiHd_12Aug1744'
#cRun='CbmTofPiHd_13Aug0855'
#cRun='CbmTofPiHd_15Aug0818'
#cRun='CbmTofPiHd_16Aug1016'
#cRun='CbmTofPiHd_17Aug1724'
#cRun='CbmTofPiHd_22Aug1616'
cRun='CbmTofPiHd_29Aug1401'
#cRun='CbmTofPiHd_07Sep1333'
#cRun=CbmTofPiHd_12Sep1601'

#iDut=901; iRef=900; iSel2=921
#((iSet=$iDut*1000+$iRef))
  
c0='00000'

#iCalSet=$2
#iCalSet=iDutiMRefiBRef
#iCalSet=901900921
#iCalSet=901900910
iCalSet=601600921

((iTmp  = $iCalSet ))
((iBRef = $iTmp % 1000))
((iTmp  = $iTmp - $iBRef))
((iSet  = $iTmp / 1000))
((iRef  = $iTmp % 1000000))
((iRef  = $iRef / 1000))
((iTmp  = $iTmp - $iRef))
((iDut  = $iTmp / 1000000))

#iSel2=$3
#iSel2=-$iBRef
iSel2=901

if((${iSel2}<0));then
 ((iBRef=-$iSel2))
fi

# ----------------------- Clean up before fresh Re-run ------------------------
if((0)); then
rm all_*.par core_dump_* *.pdf
rm -rf ${cRun}/Iter*
rm digi_${cRun}.out.root
rm tofAnaTestBeam.hst.root
rm tofTestBeamClust_${cRun}_set${iCalSet}.hst.root
rm digi_${cRun}_${iCalSet}_${iSel2}.out.root
fi
# ------------------------------- End Clean up --------------------------------
echo iter_calib with iDut=$iDut, iRef=$iRef, iSet=$iSet, iSel2=$iSel2, iBRef=$iBRef

#mkdir ${cRun}
#cp rootlogon.C ${cRun}
#cp .rootrc ${cRun}
cd ${cRun}

iStep=0
iStepLast=0
# history for 1805: 1 6 8 2 10 2 10
#for mode in 1 6 8 2 10 4 5 4
#for mode in 2 10 2 10 
#for mode in 3
#for mode in 5
#for mode in 8
#for mode in 6 8 2 10 4 5 4
for mode in 1   
do

case $mode in 
1)
nIter=3
alist=`echo '1'$c0'0,93,1,'$iRef'' '1'$c0'0,64,1,-'$iRef'' '1'$c0'0,93,1,'$iRef'' '1'$c0'0,93,0,'$iDut'' '1'$c0'0,64,0,-'$iDut'' '1'$c0'0,93,0,'$iDut'' '1'$c0'0,93,1,'$iRef'' '1'$c0'00,41,1,-'$iRef'' '1'$c0'0,93,1,'$iRef'' '1'$c0'0,93,0,'$iDut'' '1'$c0'00,41,0,-'$iDut''  '1'$c0'0,93,0,'$iDut'' '1'$c0'0,93,1,'$iRef''`
;;
2)
nIter=2
alist=`echo '1'$c0'0,92,1,'$iRef'' '1'$c0'00,64,1,-'$iRef'' '1'$c0'00,64,1,'$iBRef'' '1'$c0'0,92,0,'$iDut'' '1'$c0'00,64,0,-'$iDut'' '1'$c0'00,64,0,'$iBRef'' '1'$c0'0,92,1,'$iRef''`
;;
3)
nIter=2
alist=`echo '1'$c0'0,93,1,'$iRef''  '1'$c0'0,64,1,-'$iRef'' '1'$c0'0,93,0,'$iDut'' '1'$c0'0,64,0,-'$iDut''`
;;
4)
nIter=5
alist=`echxfo '5'$c0'0,93,1,'$iRef'' '5'$c0'0,93,-2,2' '1'$c0'000,61,-1,-2' '5'$c0'0,93,-3,2' '5'$c0'0,93,1,'$iRef'' ` 
;;
5)
nIter=2
alist=`echo '5'$c0'0,93,1,'$iRef'' '5'$c0'0,93,-2,2' '1'$c0'00,64,1,-'$iRef'' '1'$c0'00,64,0,-'$iDut'' '5'$c0'0,93,-3,2' ` 
;;
6)
nIter=2
alist=`echo '1'$c0'0,93,1,'$iRef'' '1'$c0'00,93,-2,-2' '1'$c0'0,93,0,'$iDut'' '1'$c0'00,93,-2,-2' '1'$c0'0,93,1,'$iRef''` 
;;
7)
nIter=3
alist=`echo '1'$c0',92,1,4' '1'$c0'00,93,-2,-2' '1'$c0',92,0,3' '1'$c0'00,93,-2,2' '1'$c0',92,1,4'` 
;;
8)
#derive walk corrections from comparion of cluster strips 
nIter=5
alist=`echo '1'$c0'0,93,1,'$iRef'' '1'$c0'0,92,0,'$iDut'' '1'$c0'00,61,-1,-2' '1'$c0'00,93,-2,2' '1'$c0'0,93,1,'$iRef''`
;;
9)
nIter=2
alist=`echo '1'$c0',92,1,4' '1'$c0',92,0,3' '1'$c0'00,61,-1,-2' '1'$c0'0,93,-2,2' '1'$c0',92,1,4'`
;;
10)
nIter=1
alist=`echo '1'$c0'0,92,1,'$iRef'' '1'$c0'00,93,-3,2' '1'$c0'00,93,-2,2' '1'$c0'00,61,-1,-2' '1'$c0'0,92,1,'$iRef''`
;;
11)
nIter=1
alist=`echo '1'$c0',92,1,'$iRef'' '1'$c0'00,93,-3,2' '1'$c0'00,93,-2,2' '1'$c0'00,61,-1,-2' '1'$c0'0,92,1,'$iRef''`
;;
esac

echo 'Iterate Clusterizer calibration for run '$cRun, Set ${iCalSet} - execute $nIter iterations of ${alist}
#lastOpt=''

rm -v ./${cRun}_${iCalSet}${cSel2}_tofAnaTestBeam.hst.root

while [[ $nIter > 0 ]]; do
cSel2=$iSel2;
if [[ $iSel2 = 0 ]]; then
cSel2="000"
fi
#rm -v ./${cRun}_${iCalSet}${cSel2}_tofAnaTestBeam.hst.root

for inOpt in $alist 
do

((iStepLast = ${iStep}))
((iStep += 1))
mkdir Iter${iStep}
cp rootlogon.C Iter${iStep}
cp .rootrc Iter${iStep}
cd Iter${iStep}

rm -v ./${cRun}_${iCalSet}${cSel2}_tofAnaTestBeam.hst.root

if [[ ${iStep} = 1 ]]; then 
    lastOpt=$inOpt
fi 

if [[ ${lastOpt:+1} ]] ; then
# echo last round was done with $lastOpt, extract 2. and 3. word
i1=`expr index $inOpt , `
i2=($i1+3)
#echo `expr index $inOpt , ` = $i1
cMode=${inOpt:$i1:2}
cSel=${inOpt:$i2:1}
echo Next iteration: cMode=$cMode, cSel=$cSel 
if [[ ${cSel} = "-" ]];then 
    cSel=${inOpt:$i2:2}
    echo cSel=$cSel 
    cSel="0"
fi
#copy calibration file 

if [[ ${iStep} = 1 ]]; then 
    cp -v ../${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root .
else 
    cp -v ../Iter${iStepLast}/tofTestBeamClust_${cRun}_set${iCalSet}.hst.root ${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
fi
fi 


cp -v  ../${cRun}_${iCalSet}${cSel2}_tofAnaTestBeam.hst.root .

# generate new calibration file
#root -b -q '../../ana_digi.C('$inOpt',0,"'$cRun'",'$iSet',0,'$iSel2') '
root -b -q '../../ana_digi.C('$inOpt',1,"'$cRun'",'$iCalSet',0,'$iSel2') '

lastOpt=$inOpt

#./screenshot.sh 
cp -v tofTestBeamClust_${cRun}_set${iCalSet}.hst.root ../${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root

cp -v tofAnaTestBeam.hst.root ../${cRun}_${iCalSet}${cSel2}_tofAnaTestBeam.hst.root
cp *pdf ../
rm all_*
cd .. 
rm ../${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
ln -s ./${cRun}/${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root ../${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
echo Iter step $iStep with mode ${cMode}, option $inOpt finished
done

(( nIter -= 1))
done
#cp -v tofTestBeamClust_${cRun}_set${iCalSet}.hst.root ${cRun}_set${iCalSet}_${cMode}_${cSel}${mode}tofTestBeamClust.hst.root

done 
#
