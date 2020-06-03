# shell script to generate clusterizer calibrations 
#cRun='MbsTrbSun1205_tsu'
cRun='MbsTrbSun1555_tsu'

nIter3=1
nIter=1
nEvt=400000

echo 'Iterate clusterizer calibration for run '$cRun, execute $nIter iterations
#lastOpt=''

while [[ ${nIter3} > 0 ]]; do 

for inOpt in '100000,0,0,0,51' '100000,03,0,0,51' '100000,03,0,0,51' '100000,03,0,0,51' '100000,13,0,0,51' '100000,13,0,0,51' '100000,23,0,0,51' '100000,23,0,0,51' '100000,33,0,0,51' '100000,33,0,0,51' '100000,43,0,0,51' '100000,43,0,0,51' '100000,43,0,0,51' '100000,53,0,0,51' '100000,53,0,0,51' '100000,53,0,0,51' '100000,63,0,0,51' '100000,63,0,0,51' '100000,63,0,0,51' '100000,73,0,0,51' '100000,73,0,0,51' '100000,73,0,0,51' '100000,83,0,0,51' '100000,83,0,0,51' '100000,83,0,0,51' '100000,93,0,0,51' '100000,93,0,0,51' '100000,93,0,0,51' 
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
root -b -q 'ana_digi3.C('$inOpt',"'${cRun}'",1) '
lastOpt=$inOpt
done

cp -v  ${cRun}_93_1tofTestBeamClust.hst.root ${cRun}_03_0tofTestBeamClust.hst.root
cp -v  ${cRun}_93_1tofTestBeamClust.hst.root ${cRun}_03_00tofTestBeamClust.hst.root
(( nIter3 -= 1))
done

# 2. loop 
while [[ ${nIter} > 0 ]]; do 

for inOpt in ''$nEvt',03,0,0,51' ''$nEvt',03,0,0,51' ''$nEvt',03,0,8,1' ''$nEvt',03,1,1,1' ''$nEvt',13,0,0,51' ''$nEvt',13,0,0,51' ''$nEvt',13,0,8,1' ''$nEvt',13,1,1,1' ''$nEvt',23,0,0,51' ''$nEvt',23,0,0,51' ''$nEvt',23,0,8,1' ''$nEvt',23,1,1,1' ''$nEvt',33,0,0,51' ''$nEvt',33,0,0,51' ''$nEvt',33,0,8,1' ''$nEvt',33,1,1,1' ''$nEvt',43,0,0,51' ''$nEvt',43,0,0,51' ''$nEvt',43,0,8,1' ''$nEvt',43,1,1,1' ''$nEvt',53,0,0,51' ''$nEvt',53,0,0,51' ''$nEvt',53,0,8,1' ''$nEvt',53,1,1,1' '10000000,04,1,5,1' '10000000,04,0,5,1' ''$nEvt',53,0,0,51' ''$nEvt',53,0,8,1' ''$nEvt',53,1,1,1' ''$nEvt',53,0,0,51' '10000000,04,1,5,1' '10000000,04,0,5,1' ''$nEvt',63,0,8,1' ''$nEvt',63,1,1,1' ''$nEvt',63,0,8,1' ''$nEvt',63,1,1,1' ''$nEvt',73,0,8,1' ''$nEvt',73,1,1,1' ''$nEvt',73,0,8,1' '100000000,24,0,1,1' ''$nEvt',73,1,1,1' '10000000,24,1,8,1' '10000000,24,1,5,1' ''$nEvt',73,0,8,1' '100000000,24,0,1,1' '100000000,24,0,5,1' ''$nEvt',73,1,1,1' '1000000,24,1,8,1' ''$nEvt',83,0,8,1' ''$nEvt',83,1,1,1' ''$nEvt',83,0,8,1' ''$nEvt',83,1,1,1' ''$nEvt',93,0,8,1' ''$nEvt',93,1,1,1' 
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
