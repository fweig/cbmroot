# shell script to iterate clusterizer calibrations 
cRun=$1
#cRun='CbmTofSps_01Dec0128'
#cRun='CbmTofSps_29Nov2103'
#cRun='CbmTofSps_28Nov0041'
#cRun='CbmTofSps_27Nov2115'
#iDut=921; iRef=920; iSel2=-300
#iDut=921; iRef=300; iSel2=920
#iDut=400; iRef=300; iSel2=921
#iDut=400; iRef=300; iSel2=0
#iDut=921; iRef=300; iSel2=920
#iDut=921; iRef=920; iSel2=0
#iDut=3; iRef=9; iSel2=9
##iDut=9; iRef=9; iSel2=3
#iDut=9; iRef=4; iSel2=-3
#iDut=9; iRef=7; iSel2=-3
#iDut=6; iRef=1;   iSel2=-8
###iDut=6; iRef=1; iSel2=8;
#((iSet=$iDut*1000+$iRef))
  
c0='00000'

iCalSet=$2
((iTmp  = $iCalSet ))
((iBRef = $iTmp % 1000))
((iTmp  = $iTmp - $iBRef))
((iSet  = $iTmp / 1000))
((iRef  = $iTmp % 1000000))
((iRef  = $iRef / 1000))
((iTmp  = $iTmp - $iRef))
((iDut  = $iTmp / 1000000))

iSel2=$3

if((${iSel2}<0));then
 ((iBRef=-$iSel2))
fi

echo gen_digi with iDut=$iDut, iRef=$iRef, iSet=$iSet, iSel2=$iSel2, iBRef=$iBRef

root -b -q './ana_digi.C(100000000,93,1,'$iRef',0,"'$cRun'",'$iSet',1,'$iSel2') '

