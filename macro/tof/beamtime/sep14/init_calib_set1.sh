# shell script to iterate clusterizer calibrations 
#cRun='MbsTrbSat1805'
cRun='MbsTrbSat2035'
#cRun='MbsTrbSun1205_tsu'
nIter=1
echo 'Iterate clusterizer calibration for run '$cRun, execute $nIter iterations
#lastOpt=''

while [[ $nIter > 0 ]]; do 
for inOpt in '100000,0,0,0,51' '100000,03,2,6,51' '100000,03,3,1,51' '100000,03,2,6,1' '100000,03,3,1,1' '100000,13,2,6,1' '100000,13,3,1,1' '100000,23,2,6,1' '100000,23,3,1,1' '100000,33,2,6,1' '100000,33,3,1,1' '100000,43,2,6,1' '100000,43,3,1,1' '100000,53,2,6,1' '100000,53,3,1,1' '10000000,04,3,5,1' '10000000,04,2,5,1' '10000000,04,3,2,1' '10000000,04,2,2,1' '100000,63,2,6,1' '100000,63,3,1,1' '100000,73,2,6,1' '100000,73,3,1,1' '100000,73,2,6,1' '100000000,24,2,1,1' '100000,73,3,1,1' '100000000,24,3,6,1' '100000,83,2,6,1' '100000,83,3,1,1' '100000,93,2,6,1' '100000,93,3,1,1' 
 
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
root -b -q 'ana_digi_cal.C('$inOpt',"'${cRun}'",1) '

lastOpt=$inOpt
done
(( nIter -= 1))
done 

#
