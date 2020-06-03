#!/bin/bash
# shell script to iterate tracklet calibration histograms
cRun='CbmTofSps_29Nov2103'
iDut=921; iRef=920; iSel2=-300
#iSel=94; iSet=943; iSel2=3

iTraSetup=0;
# what should be done ?
((iSel=$iDut*1000+$iRef))
#which file should be analyzed ?  
cSet="921920_-300";

dDTres=10000000

while [[ $dDTres > 0 ]]; do

for iCal in 2  2  1  1 
do
root -b -q 'ana_hits.C(100000,'$iSel','$iCal',"'$cRun'","'$cSet'",'$iSel2','$iTraSetup')'

cp -v tofFindTracks.hst.root ${cRun}_tofFindTracks.hst.root
done

Tres=`cat TCalib.res`
dTdif=`echo "$dDTres - $Tres" | bc`
compare_result=`echo "$Tres < $dDTres" | bc`

echo got TOff = $Tres, compare to $dDTres, dTdif = $dTdif, compare_result = $compare_result

if [[ $compare_result > 0 ]]; then
dDTres=$Tres
else
dDTres=0
fi

done
