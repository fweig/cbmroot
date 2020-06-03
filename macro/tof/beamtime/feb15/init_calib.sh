#!/bin/bash
# shell script to iterate clusterizer calibrations
cRun=$1
#cRun='CernSps05Mar0041'
#cRun='CernSps02Mar2227_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_020_nb'
#cRun='CernSps05Mar0041_up'
#cRun='CernSps03Mar0112_up'
#cRun='CernSps03Mar0830_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_000_nb_up'
#cRun='CernSps02Mar2315_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_000_nb_up'
#cRun='CernSps02Mar2227_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_020_nb_up'
#cRun='CernSps02Mar2202_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_030_nb_up'
#cRun='CernSps02Mar2151_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_030_nb_up'
#cRun='CernSps02Mar2120_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_010_nb_up'
#cRun='CernSps02Mar2056_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_000_nb_up'
#cRun='CernSps02Mar2044_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_000_nb_up'
#cRun='CernSps02Mar2031_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_000_nb_up'
#cRun='CernSps02Mar2024_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_000_nb_up'
#cRun='CernSps01Mar2203_hdref_200_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_0c0_nb_up'
#cRun='CernSps01Mar1253_hdref_3ff_hdp2_220_thupad_170_thustrip_23c_bucref_160_buc2013_160_ustc_220_diam_0c0_nb'
#cRun='CernSps28Feb0448_hdref_210_hdp2_220_thupad_170_thustrip_23c_bucref_140_buc2013_140_ustc_210_diam_0a0_nb_up'
#cRun='CernSps27Feb0510_hdref_3ff_hdp2_260_thupad_140_thustrip_220_bucref_140_buc2013_140_ustc_000_diam_0c0_nb_up'
#cRun='CernSps26Feb1603_hdref_3ff_hdp2_220_thupad_180_thustrip_180_bucref_140_buc2013_140_ustc_09e_diam_080_nb'
nIter=1
echo 'Initialize clusterizer calibration for run '$cRun, execute $nIter iterations
#iDut=3; iMRef=9; iBRef=7; 
#iDut=9; iMRef=4; iBRef=3;  
#iDut=9; iMRef=7; iBRef=3;  
#iDut=9; iMRef=3; iBRef=5;  
#iDut=3; iMRef=9; iBRef=5;  
#iDut=6; iMRef=1; iBRef=8;  
#iDut=6; iMRef=1; iBRef=5;  
#iDut=1; iMRef=6; iBRef=5;  
#iDut=300; iMRef=400; iBRef=510;  
#iDut=300; iMRef=900; iBRef=510;  
iDut=100; iMRef=600; iBRef=510;  

((iSet=$iDut*1000+$iMRef))
#iSet=0
#lastOpt=''
nEvi=100000
mkdir ${cRun}
cp rootlogon.C ${cRun}
cp .rootrc ${cRun}
cd ${cRun}

iStep=0
iStepLast=0
while [[ $nIter > 0 ]]; do 
echo start from scratch with $nIter iterations 
if((0)); then
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
#cp -v tofTestBeamClust_${cRun}.hst.root ${cRun}_${cMode}_${cSel}tofTestBeamClust.hst.root
cp -v ../Init${iStepLast}/tofTestBeamClust_${cRun}_set${iSet}.hst.root ${cRun}_set${iSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
fi 

# generate new calibration file
root -b -q '../../ana_digi_ini.C('$inOpt',"'${cRun}'",'${iSet}','${iBRef}') '
lastOpt=$inOpt

#./screenshot.sh
cp *pdf ../
cd .. 
done

cp -v  ./Init${iStep}/${cRun}_set${iSet}_${cMode}_${cSel}tofTestBeamClust.hst.root ${cRun}_set${iSet}_03_0tofTestBeamClust.hst.root 
fi

echo execute main loop at $nIter. iteration 

for inOpt in ''$nEvi',03,0,0,50' ''$nEvi',03,0,0,50' ''$nEvi',03,1,'${iMRef}',0' ''$nEvi',03,0,'${iDut}',0' ''$nEvi',03,1,'${iMRef}',0' ''$nEvi',03,0,'${iDut}',0' ''$nEvi',13,0,0,50' ''$nEvi',13,0,0,50' ''$nEvi',13,1,'${iMRef}',0'  ''$nEvi',13,0,'${iDut}',0' ''$nEvi',13,1,'${iMRef}',0'  ''$nEvi',13,0,'${iDut}',0' ''$nEvi',04,0,'${iBRef}',0' ''$nEvi',04,1,'${iBRef}',0' ''$nEvi',23,0,0,50' ''$nEvi',23,0,0,50' ''$nEvi',23,1,'${iMRef}',0'  ''$nEvi',23,0,'${iDut}',0' ''$nEvi',23,1,'${iMRef}',0'  ''$nEvi',23,0,'${iDut}',0' ''$nEvi',01,0,'-${iDut}',0' ''$nEvi',01,1,'-${iMRef}',0' ''$nEvi',33,0,0,50' ''$nEvi',33,0,0,50' ''$nEvi',33,1,'${iMRef}',0'  ''$nEvi',33,0,'${iDut}',0' ''$nEvi',04,0,'-${iDut}',0' ''$nEvi',33,0,'${iDut}',0' ''$nEvi',04,1,'-${iMRef}',0' ''$nEvi',33,1,'${iMRef}',0'  ''$nEvi',43,0,0,50' ''$nEvi',43,0,0,50' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,0,50' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',53,0,0,50' ''$nEvi',53,0,0,50' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',52,0,0,0' ''$nEvi',52,0,0,50' '500000,14,0,-'${iDut}',0' '500000,14,1,-'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',63,0,'${iDut}',0' ''$nEvi',63,1,'${iMRef}',0' ''$nEvi',73,0,'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' ''$nEvi',73,0,'${iDut}',0' '2000000,24,0,-'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' '2000000,24,1,-'${iMRef}',0' ''$nEvi',83,0,'${iDut}',0' ''$nEvi',83,1,'${iMRef}',0' ''$nEvi',93,0,'${iDut}',0' ''$nEvi',93,1,'${iMRef}',0'
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
cp -v ../Init${iStepLast}/tofTestBeamClust_${cRun}_set${iSet}.hst.root ${cRun}_set${iSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
else 
iStep=18
cp -v  ../../${cRun}_set${iSet}_03_0tofTestBeamClust.hst.root .
fi 

# generate new calibration file
root -b -q '../../ana_digi_cal.C('$inOpt',"'${cRun}'",'${iSet}','${iBRef}') '

lastOpt=$inOpt

cp -v tofTestBeamClust_${cRun}_set${iSet}.hst.root ../${cRun}_set${iSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
cp *pdf ../
#./screenshot.sh
cd .. 
rm ../${cRun}_set${iSet}_${cMode}_${cSel}tofTestBeamClust.hst.root
ln -s ./${cRun}/${cRun}_set${iSet}_${cMode}_${cSel}tofTestBeamClust.hst.root ../${cRun}_set${iSet}_${cMode}_${cSel}tofTestBeamClust.hst.root

done
(( nIter -= 1))
done 
#
