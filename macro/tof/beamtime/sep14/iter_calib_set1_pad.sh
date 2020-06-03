# shell script to iterate clusterizer calibrations 
#cRun='MbsTrbSun1205_tsu'
cRun='MbsTrbSun1555_tsu'
# history: 1 5 6 8 7 3
#for mode in 0 1 6 8 
for mode in 1 6 8 2 10 
do

case $mode in 
0)
nIter=2
alist=`echo '500000,93,1,1' '10000000,54,1,8' '10000000,54,1,5' '500000,93,0,8' '10000000,54,0,1' '10000000,54,0,5' '500000,93,1,1'`
;;
1)
nIter=2
alist=`echo '500000,93,1,1' '10000000,64,1,8' '10000000,64,1,5' '500000,93,0,8' '10000000,64,0,1' '10000000,64,0,5' '500000,93,1,1'`
;;
2)
nIter=1
alist=`echo '100000,92,1,1' '10000000,64,1,8' '10000000,64,1,5' '100000,92,0,8' '10000000,64,0,1' '10000000,64,0,5' '100000,92,1,1'`
;;
3)
nIter=3
alist=`echo '500000,93,1,1'` 
;;
4)
nIter=5
alist=`echo '100000,92,1,1'` 
;;
5)
nIter=5
alist=`echo '100000,92,0,8'` 
;;
6)
nIter=3
alist=`echo '500000,93,1,1' '10000000,93,-2,-2' '500000,93,0,8' '10000000,93,-2,-2' '500000,93,1,1'` 
;;
7)
nIter=3
alist=`echo '100000,92,1,1' '10000000,93,-2,-2' '100000,92,0,8' '10000000,93,-2,-2' '100000,92,1,1'` 
;;
8)
nIter=2
alist=`echo '500000,93,1,1' '500000,93,0,8' '10000000,61,-1,-2' '10000000,93,-2,-2' '500000,93,1,1'`
;;
9)
nIter=2
alist=`echo '100000,92,1,1' '100000,92,0,8' '10000000,61,-1,-2' '1000000,93,-2,-2' '100000,92,1,1'`
;;
10)
nIter=2
alist=`echo '100000,92,1,1' '1000000,93,-3,2' '1000000,93,-2,2' '10000000,61,-1,-2' '100000,92,1,1'`
;;
11)
nIter=1
alist=`echo '100000,92,1,1' '10000000,93,-3,2' '10000000,93,-2,2' '10000000,61,-1,-2' '1000000,92,1,1'`
;;
esac

echo 'Iterate Clusterizer calibration for run '$cRun, execute $nIter iterations of ${alist}
#lastOpt=''

while [[ $nIter > 0 ]]; do

for inOpt in $alist 
do

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
cp -v tofTestBeamClust_${cRun}.hst.root ${cRun}_${cMode}_${cSel}tofTestBeamClust.hst.root
fi 

# generate new calibration file
root -b -q 'ana_digi.C('$inOpt',1,"'$cRun'",1) '

lastOpt=$inOpt
done
(( nIter -= 1))
done
cp -v tofTestBeamClust_${cRun}.hst.root ${cRun}_${cMode}_${cSel}${mode}tofTestBeamClust.hst.root

done 
#
