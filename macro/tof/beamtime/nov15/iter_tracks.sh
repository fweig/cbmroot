#!/bin/bash
# shell script to iterate tracklet calibration histograms
cRun='CbmTofSps_01Dec0206'
#cRun='CbmTofSps_29Nov2103'
#cRun='CbmTofSps_28Nov0041'
#cRun='CbmTofSps_28Nov0001'
#iDut=921; iRef=920; iSel2=-300
#iDut=400; iRef=300; iSel2=921
iDut=900; iRef=921; iSel2=920


# what should be done ?
((iSel=$iDut*1000+$iRef))
#which file should be analyzed ?  
#cSet="921920_-300";
cSet="920921_000";
#cSet="400300_921";

iTraSetup=6;
nEvt=50000
dDTres=200000
dDTRMSres=200000
iter=0;
fRange2=50
fRange1=2

cd ${cRun}
#rm -v ${cRun}_tofFindTracks.hst.root

if [[ $iter > 0 ]]; then
 cp -v  ${cRun}_tofFindTracks.hst${iter}.root  ${cRun}_tofFindTracks.hst.root
fi
 
while [[ $dDTres > 0 ]]; do

((fRange2 /= 2))
if((${fRange2}<5));then
 ((fRange2=5))
fi

if((0)); then 
iCal=2
nIt=2
while [[ $nIt > 0 ]]; do
((iter += 1))
root -b -q '../ana_trks.C('$nEvt','$iSel','$iCal',"'$cRun'","'$cSet'",'$iSel2','$iTraSetup','$fRange2')'
cp -v tofFindTracks.hst.root ${cRun}_tofFindTracks.hst.root
cp -v tofFindTracks.hst.root ${cRun}_tofFindTracks.hst${iter}.root
#cp -v tofAnaTestBeam.hst.root ${cRun}_TrkAnaTestBeam.hst.root
((nIt -= 1))
done

((fRange1 /= 2))
if((${fRange1}<2));then
 ((fRange1=2))
fi
fi

# correction modes: 2 - Tt, 3 - t, 4 - x, 5 - y, 6 - z
for iCal in 3 10 11 12 13 14 15; do
nIt=1
while [[ $nIt > 0 ]]; do
((iter += 1))
root -b -q '../ana_trks.C('$nEvt','$iSel','$iCal',"'$cRun'","'$cSet'",'$iSel2','$iTraSetup','$fRange1')'
cp -v tofFindTracks.hst.root ${cRun}_tofFindTracks.hst.root
cp -v tofFindTracks.hst.root ${cRun}_tofFindTracks.hst${iter}.root
#cp -v tofAnaTestBeam.hst.root ${cRun}_TrkAnaTestBeam.hst.root
((nIt -= 1))
done
done

iTres=`cat TCalib.res`
if [[ $iTres = 0 ]]; then
    echo All tracks lost, stop at iter = $iter
    return
fi

((TRMSres=$iTres%100000))
((iTres -= TRMSres ))
((Tres   = iTres / 100000)) 
dTdif=`echo "$dDTres - $Tres" | bc`
compare_result=`echo "$Tres < $dDTres" | bc`

dTRMSdif=`echo "$dDTRMSres - $TRMSres" | bc`
compare_RMS=`echo "$TRMSres < $dDTRMSres" | bc`

echo at iter=$iter got TOff = $Tres, compare to $dDTres, dTdif = $dTdif, result = $compare_result, TRMS = $TRMSres, old $dDTRMSres, dif = $dTRMSdif, result = $compare_RMS 
((compare_result += $compare_RMS))

if [[ $compare_result > 0 ]]; then
dDTres=$Tres
dDTRMSres=$TRMSres
else
dDTres=0
cp -v  tofFindTracks.hst.root ../${cRun}_tofFindTracks.hst.root
fi

done
