#!/bin/bash
# shell script to iterate Analyzer histograms
cRun=$1
#cRun='CbmTofSps_29Nov2103'
iDut=$2; iRef=$3; iSel2=$4
#iDut=921; iRef=920; iSel2=-300
#iSel=94; iSet=943; iSel2=3

iTraSetup=0;
# what should be done ?
((iSel=$iDut*1000+$iRef))
#which file should be analyzed ?  
cSet=$5
#cSet="921920_-300";
dScalFac=$6

dDTres=10000000

cSel2=$iSel2;
if [[ $iSel2 = 0 ]]; then
cSel2="000"
fi

cd ${cRun}
mkdir          Ana${cRun}_${cSet}_${iSel}_${cSel2}
cp rootlogon.C Ana${cRun}_${cSet}_${iSel}_${cSel2}/
cp .rootrc     Ana${cRun}_${cSet}_${iSel}_${cSel2}/
#cp ../${cRun}_${cSet}_${iSel}_${cSel2}_tofAnaTestBeam.hst.root Ana${cRun}_${cSet}_${iSel}_${cSel2}/
cd Ana${cRun}_${cSet}_${iSel}_${cSel2}
mkdir hst
rm ${cRun}_${cSet}_${iSel}_${cSel2}_tofAnaTestBeam.hst.root

while [[ $dDTres > 0 ]]; do
#while ((0)) ; do

for iCal in 1 2 3 5 6 1
do
rm Test.res
root -b -q '../../ana_hits.C(1000000,'$iSel','$iCal',"'$cRun'","'$cSet'",'$iSel2','$iTraSetup','$dScalFac')'

mv -v tofAnaTestBeam.hst.root ${cRun}_${cSet}_${iSel}_${cSel2}_tofAnaTestBeam.hst.root
cp -v ${cRun}_${cSet}_${iSel}_${cSel2}_tofAnaTestBeam.hst.root ../../
rm all_*
if (! (test -f Test.res)); then
echo no resolution file available: return
return
fi
done

Tres=`cat Test.res`
dTdif=`echo "$dDTres - $Tres" | bc`
dDTres=`echo "$dDTres - 0.001" | bc`
compare_result=`echo "$Tres < $dDTres" | bc`

echo got Tres = $Tres, compare to $dDTres, dTdif = $dTdif, compare_result = $compare_result

if [[ $compare_result > 0 ]]; then
dDTres=$Tres
else
dDTres=0
fi

done
# final action -> scan full statistics 
root -b -q '../../ana_hits.C(-1,'$iSel',1,"'$cRun'","'$cSet'",'$iSel2','$iTraSetup','$dScalFac')'
#echo copy results from `pwd` ?
#cp -v ./hst/* ../../hst/
rm all_*
cd ../..
