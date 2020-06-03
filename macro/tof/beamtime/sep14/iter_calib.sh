# shell script to iterate clusterizer calibrations 
#cRun='MbsTrbSat1805'
cRun='MbsTrbSat2035'
#cRun='MbsTrbSun1205_tsu'
#cRun='MbsTrbSun1555_tsu'
# history for 1805: 1 6 8 2 10 2 10
for mode in 1 6 8 2 10 2 10
#for mode in 2 10 2 10 
#for mode in 5
do

case $mode in 
1)
nIter=1
alist=`echo '100000,93,1,4' '10000000,64,1,3' '10000000,64,1,5' '100000,93,0,3' '10000000,64,0,4' '10000000,64,0,5' '100000,93,1,4'`
;;
2)
nIter=1
alist=`echo '100000,92,1,4' '10000000,64,1,3' '10000000,64,1,5' '100000,92,0,3' '10000000,64,0,4' '10000000,64,0,5' '100000,92,1,4'`
;;
3)
nIter=3
alist=`echo '100000,93,1,4' `
;;
4)
nIter=10
alist=`echo '100000,93,-2,2'` 
;;
5)
nIter=5
alist=`echo '100000,92,1,4' '100000,92,0,3' '100000,93,-3,2' '100000,93,-2,2' '100000,61,-1,-2' '100000,92,1,4'` 
;;
6)
nIter=3
alist=`echo '100000,93,1,4' '1000000,93,-2,-2' '100000,93,0,3' '1000000,93,-2,-2' '100000,93,1,4'` 
;;
7)
nIter=3
alist=`echo '100000,92,1,4' '10000000,93,-2,-2' '100000,92,0,3' '10000000,93,-2,2' '100000,92,1,4'` 
;;
8)
nIter=2
alist=`echo '100000,93,1,4' '100000,92,0,3' '10000000,61,-1,-2' '1000000,93,-2,2' '100000,93,1,4'`
;;
9)
nIter=2
alist=`echo '100000,92,1,4' '100000,92,0,3' '10000000,61,-1,-2' '1000000,93,-2,2' '100000,92,1,4'`
;;
10)
nIter=1
alist=`echo '100000,92,1,4' '1000000,93,-3,2' '1000000,93,-2,2' '10000000,61,-1,-2' '100000,92,1,4'`
;;
11)
nIter=1
alist=`echo '100000,92,1,4' '10000000,93,-3,2' '10000000,93,-2,2' '10000000,61,-1,-2' '1000000,92,1,4'`
;;
esac

echo 'Iterate Clusterizer calibration for run '$cRun, execute $nIter iterations of ${alist}
#lastOpt=''

while [[ $nIter > 0 ]]; do
#10 for inOpt in '10000,52,1,4'
#9 for inOpt in '10000,92,1,4' '10000,92,0,3' 
#8 for inOpt in '100000,93,1,4' '100000,92,0,3' '10000000,61,-1,-2' '1000000,93,-2,-2'
#7 for inOpt in '100000,93,1,4' '1000000,93,-2,-2' '100000,92,0,3' '1000000,93,-2,-2'   
#6 for inOpt in '100000,93,1,4' '10000000,93,-2,-2' '100000,93,0,3' '10000000,93,-2,-2'  
#5 for inOpt in '10000000,93,0,3' '10000000,61,-1,4'   
#4 for inOpt in '100000,93,0,3' '100000,93,1,4' '10000000,93,-2,-1'   
#3 for inOpt in '10000000,93,-2,-1' '10000000,93,-3,3' '10000000,93,-3,4'    
#2 for inOpt in '10000000,93,1,4' '10000000,61,-1,3'
#for inOpt in '100000,93,1,4' '10000000,64,1,3' '10000000,64,1,5' '100000,93,0,3' '10000000,64,0,4' '10000000,64,0,5' 
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
root -b -q 'ana_digi.C('$inOpt',1,"'$cRun'",0) '

lastOpt=$inOpt
done
(( nIter -= 1))
done
cp -v tofTestBeamClust_${cRun}.hst.root ${cRun}_${cMode}_${cSel}${mode}tofTestBeamClust.hst.root

done 
#
