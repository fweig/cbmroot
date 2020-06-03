#!/bin/bash
# shell script to iterate Analyzer histograms
cRun='MbsTrbSun1555_tsu'

iSel=92
iSet=1
dDTres=1000000

while [[ $dDTres > 0 ]]; do

for iCal in 1 2 3 4 1
do
root -b -q 'ana_hits.C(2000000,'$iSel','$iCal',"'$cRun'",'$iSet') '
cp -v tofAnaTestBeam.hst.root ${cRun}_${iSet}_${iSel}_tofAnaTestBeam.hst.root
done

Tres=`cat Test.res`
dTdif=`echo "$dDTres - $Tres" | bc`
compare_result=`echo "$Tres < $dDTres" | bc`

echo got Tres = $Tres, compare to $dDTres, dTdif = $dTdif, compare_result = $compare_result

if [[ $compare_result > 0 ]]; then
dDTres=$Tres
else
dDTres=0
fi

done
