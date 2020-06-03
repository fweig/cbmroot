#!/bin/bash
# shell script to iterate clusterizer calibrations 

# -----------------------------------------------------------------------------
# gen_digi.sh will repeat the last step in "iter_calib.sh", similar to running 
# the ana_digi.C('$inOpt', 1, "'$cRun'", '$iCalSet', 0, '$iSel2') interactively.
# Where $inOpt="100000000, 93, 1, $iRef" in the "iter_calib.sh". Comparison is

# macro def  : ana_digi.C(nEv,calMode,calSel,calSm,RefSel,cFileId,iCalSet,bOut,iSel2)
# interactive: ana_digi.C(100000000,93,1,'$iRef',1,"'$cRun'",'$iCalSet',0,'$iSel2')
# gen_digi.sh: ana_digi.C(100000000,93,1,'$iRef',0,"'$cRun'",'$iCalSet',1,'$iSel2')

# it need file (CbmTofPiHd_29Aug1401_set901900921_93_1tofTestBeamClust.hst.root)
# generated in Iter50/ by iter_calib.sh.
# -----------------------------------------------------------------------------

#cRun=$1
#cRun='CbmTofPiHd_09Aug1533'
#cRun='CbmTofPiHd_10Aug1728'
#cRun='CbmTofPiHd_11Aug1224'
#cRun='CbmTofPiHd_12Aug1744'
#cRun='CbmTofPiHd_13Aug0855'
#cRun='CbmTofPiHd_15Aug0818'
#cRun='CbmTofPiHd_16Aug1016'
#cRun='CbmTofPiHd_17Aug1724'
#cRun='CbmTofPiHd_22Aug1616'
cRun='CbmTofPiHd_29Aug1401'
#cRun='CbmTofPiHd_07Sep1333'
#cRun=CbmTofPiHd_12Sep1601'

#iDut=921; iRef=920; iSel2=-300
#((iSet=$iDut*1000+$iRef))
  
c0='00000'

#iCalSet=$2
#iCalSet=901900921
iCalSet=601600921

((iTmp  = $iCalSet ))
((iBRef = $iTmp % 1000))
((iTmp  = $iTmp - $iBRef))
((iSet  = $iTmp / 1000))
((iRef  = $iTmp % 1000000))
((iRef  = $iRef / 1000))
((iTmp  = $iTmp - $iRef))
((iDut  = $iTmp / 1000000))

#iSel2=$3
iSel2=901

if((${iSel2}<0)); then
((iBRef=-$iSel2))
fi

# ----------------------- Clean up before fresh Re-run ------------------------
if((0)); then
rm all_*.par core_dump_* *.pdf
rm tofAnaTestBeam.hst.root
rm tofTestBeamClust_${cRun}_set${iCalSet}.hst.root
rm digi_${cRun}_${iCalSet}_${iSel2}.out.root
fi
# ------------------------------- End Clean up --------------------------------
echo gen_digi.sh with iDut=$iDut, iRef=$iRef, iCalSet=$iCalSet, iSet=$iSet, iBRef=$iBRef, iSel2=$iSel2

if((1)); then
#void ana_digi(nEvents, calMode, calSel, calSm, RefSel, cFileId, iCalSet, bOut, iSel2) 
root -b -q './ana_digi.C(100000000,93,1,'$iRef',0,"'$cRun'",'$iCalSet',1,'$iSel2')'
fi
