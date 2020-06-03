#!/bin/bash
# shell script to initialize clusterizer calibrations
cRun=$1
#cRun='CbmTofSps_01Dec0128'
#cRun='CbmTofSps_29Nov2103'
#cRun='CbmTofSps_29Nov1257'
#cRun='CbmTofSps_29Nov1202'
#cRun='CbmTofSps_29Nov0952'
#cRun='CbmTofSps_28Nov0728'
#cRun='CbmTofSps_28Nov0001'
#cRun='CbmTofSps_28Nov0041'
#cRun='CbmTofSps_27Nov2115'
#cRun='CbmTofSps_27Nov1728'
nIter=1
echo 'Initialize clusterizer calibration for run '$cRun, execute $nIter iterations
#iDut=3; iMRef=9; iBRef=7; 
#iDut=9; iMRef=4; iBRef=3;  
#iDut=9; iMRef=7; iBRef=3;  
#iDut=9; iMRef=3; iBRef=5;  
###iDut=3; iMRef=9; iBRef=5;  
#iDut=9; iMRef=9; iBRef=5;
#iDut=3; iMRef=4; iBRef=5;
#iDut=921; iMRef=920; iBRef=300;
#iDut=400; iMRef=300; iBRef=500;
#iDut=921; iMRef=300; iBRef=500;
#iDut=921; iMRef=920; iBRef=500;

#iDut=6; iMRef=1; iBRef=8;  
#iDut=6; iMRef=1; iBRef=5;  
#iDut=1; iMRef=6; iBRef=5;  

#((iSet=$iDut*1000+$iMRef))
#((iSet=$iDut*1000000+$iMRef*1000+$iBRef))

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
#return
#iSet=0
#lastOpt=''

nEvi=10000

mkdir ${cRun}
cp rootlogon.C ${cRun}
cp .rootrc ${cRun}
cd ${cRun}

# Global variables, for for-loops. Runs from 1-15 and 16-79 in two for-loops
iRestart=0
iStep=0
iStepLast=0

while [[ $nIter > 0 ]]; do 
echo start from scratch with $nIter iterations 
if((1)); then
echo really start from scratch with $nIter iterations 

for inOpt in ''$nEvi',0,0,0,50' ''$nEvi',03,0,0,50' ''$nEvi',13,0,0,50' ''$nEvi',23,0,0,50' ''$nEvi',23,0,0,50' ''$nEvi',33,0,0,50' ''$nEvi',33,0,0,50'  ''$nEvi',33,0,0,50' ''$nEvi',43,0,0,50' ''$nEvi',43,0,0,50' ''$nEvi',43,0,0,50' ''$nEvi',53,0,0,50' ''$nEvi',53,0,0,50' ''$nEvi',63,0,0,50' ''$nEvi',63,0,0,50'
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
#cp -v tofTestBeamClust_${cRun}.hst.root ${cRun}_${cMode}_${cSel}tofTestBeamClust.hst.root
	cp -v ../Init${iStepLast}/tofTestBeamClust_${cRun}_set${iCalSet}.hst.root ${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
    fi
fi 

lastOpt=$inOpt
if (($iStep > $iRestart)) ; then 
# generate new calibration file
    root -b -q '../../ana_digi_ini.C('$inOpt',"'${cRun}'",'${iCalSet}','${iBRef}') '
lastOpt=$inOpt
#./screenshot.sh
    cp *pdf ../
    rm all_*
    cd .. 
    rm ../${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
    ln -s ./${cRun}/${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root ../${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
    echo Init step $iStep with mode ${cMode}, option $inOpt  finished
else 
    cd ..
    echo Init step $iStep with mode ${cMode}, option $inOpt  skipped
fi
done

cp -v  ./Init${iStep}/${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root ${cRun}_set${iCalSet}_03_0tofTestBeamClust.hst.root 
 
fi

echo execute main loop at $nIter. iteration 

for inOpt in ''$nEvi',03,0,0,50' ''$nEvi',03,0,0,50' ''$nEvi',03,1,'${iMRef}',0' ''$nEvi',03,0,'${iDut}',0' ''$nEvi',03,1,'${iMRef}',0' ''$nEvi',03,0,'${iDut}',0' ''$nEvi',13,0,0,50' ''$nEvi',13,0,0,50' ''$nEvi',13,1,'${iMRef}',0'  ''$nEvi',13,0,'${iDut}',0' ''$nEvi',13,1,'${iMRef}',0'  ''$nEvi',13,0,'${iDut}',0' ''$nEvi',23,0,0,50' ''$nEvi',23,0,0,50' ''$nEvi',23,1,'${iMRef}',0'  ''$nEvi',23,0,'${iDut}',0' ''$nEvi',23,1,'${iMRef}',0'  ''$nEvi',23,0,'${iDut}',0' ''$nEvi',33,0,0,50' ''$nEvi',33,0,0,50' ''$nEvi',33,1,'${iMRef}',0'  ''$nEvi',33,0,'${iDut}',0' ''$nEvi',04,0,'-${iDut}',0' ''$nEvi',33,0,'${iDut}',0' ''$nEvi',04,1,'-${iMRef}',0' ''$nEvi',33,1,'${iMRef}',0'  ''$nEvi',43,0,0,50' ''$nEvi',43,0,0,50' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,0,50' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',53,0,0,50' ''$nEvi',53,0,0,50' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',52,0,0,0' ''$nEvi',52,0,0,50' '500000,14,0,-'${iDut}',0' '500000,14,1,-'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',63,0,'${iDut}',0' ''$nEvi',63,1,'${iMRef}',0' ''$nEvi',73,0,'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' ''$nEvi',73,0,'${iDut}',0' '500000,24,0,-'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' '500000,24,1,-'${iMRef}',0' ''$nEvi',83,0,'${iDut}',0' '500000,34,0,-'${iDut}',0' ''$nEvi',83,1,'${iMRef}',0' '500000,34,1,-'${iMRef}',0' ''$nEvi',93,0,'${iDut}',0' ''$nEvi',93,1,'${iMRef}',0'
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
cp -v ../Init${iStepLast}/tofTestBeamClust_${cRun}_set${iCalSet}.hst.root ${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
fi 

lastOpt=$inOpt

if (($iStep > $iRestart)) ; then 
# generate new calibration file
    root -b -q '../../ana_digi_cal.C('$inOpt',"'${cRun}'",'${iCalSet}','${iBRef}') '

    cp -v tofTestBeamClust_${cRun}_set${iCalSet}.hst.root ../${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
    cp *pdf ../
    rm all_*
#./screenshot.sh
    cd .. 
    rm ../${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
    ln -s ./${cRun}/${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root ../${cRun}_set${iCalSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
    echo Init step $iStep with mode ${cMode}, option $inOpt  finished
else 
    cd ..
    echo Init step $iStep with mode ${cMode}, option $inOpt  skipped
fi
done
(( nIter -= 1))
done 
#
