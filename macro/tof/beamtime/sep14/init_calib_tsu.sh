# shell script to iterate clusterizer calibrations 
#cRun='MbsTrbSat1805'
#cRun='MbsTrbSun1205_tsu'
cRun='MbsTrbSun1555_tsu'
nIter=1
echo 'Iterate clusterizer calibration for run '$cRun, execute $nIter iterations
#lastOpt=''

while [[ $nIter > 0 ]]; do 

for inOpt in '100000,0,0,0,51' '100000,03,0,3,51' '100000,03,1,4,51' '100000,03,0,3,1' '100000,03,1,4,1' '100000,13,0,3,1' '100000,13,1,4,1' '100000,23,0,3,1' '100000,23,1,4,1' '100000,33,0,3,1' '100000,33,1,4,1' '100000,43,0,3,1' '100000,43,1,4,1' '100000,53,0,3,1' '100000,53,1,4,1' '100000,63,0,3,1' '100000,63,1,4,1' 
do
   
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
cp -v tofTestBeamClust_${cRun}.hst.root ${cRun}_${cMode}_${cSel}tofTestBeamClust.hst.root
fi 

# generate new calibration file
root -l -q 'ana_digi3.C('$inOpt',"'${cRun}'",0) '
lastOpt=$inOpt
done

cp -v  ${cRun}_63_1tofTestBeamClust.hst.root ${cRun}_03_0tofTestBeamClust.hst.root

for inOpt in '100000,03,0,3,51' '100000,03,1,4,51' '100000,03,0,3,1' '100000,03,1,4,1' '100000,13,0,3,1' '100000,13,1,4,1' '100000,23,0,3,1' '100000,23,1,4,1' '100000,33,0,3,1' '100000,33,1,4,1' '100000,43,0,3,1' '100000,43,1,4,1' '100000,53,0,3,1' '100000,53,1,4,1' '10000000,04,1,5,1' '10000000,04,0,5,1' '10000000,04,1,2,1' '10000000,04,0,2,1' '100000,63,0,3,1' '100000,63,1,4,1' '100000,73,0,3,1' '100000,73,1,4,1' '100000,73,0,3,1' '100000000,24,0,4,1' '100000,73,1,4,1' '100000000,24,1,3,1' '100000,83,0,3,1' '100000,83,1,4,1' '100000,93,0,3,1' '100000,93,1,4,1' 
do   
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
cp -v tofTestBeamClust_${cRun}.hst.root ${cRun}_${cMode}_${cSel}tofTestBeamClust.hst.root
fi 

# generate new calibration file
root -l -q 'ana_digi_cal.C('$inOpt',"'${cRun}'",0) '

lastOpt=$inOpt
done
(( nIter -= 1))
done 

#
