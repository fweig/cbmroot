# shell script to iterate clusterizer calibrations 
cRun=$1

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

cCalSet=$iCalSet
if (( iCalSet<100000000 )); then 
cCalSet="0"$iCalSet
fi
if (( iCalSet<10000000 )); then 
cCalSet="00"$iCalSet
fi
if (( iCalSet<1000000 )); then 
cCalSet="000"$iCalSet
fi
if (( iCalSet<100000 )); then 
cCalSet="0000"$iCalSet
fi
echo cCalSet = $cCalSet

Deadtime=$4
if [[ ${Deadtime} = "" ]]; then
Deadtime=50.
fi

CalIdMode=$5
if [[ ${CalIdMode} = "" ]]; then
 echo use native calibration file 
 CalIdMode=${cRun}
 CalFile=${cRun}_set${cCalSet}_93_1tofCosmicClust.hst.root
else 
 CalFile=${CalIdMode}_set${cCalSet}_93_1tofCosmicClust.hst.root
 RunFile=${cRun}_set${cCalSet}_93_1tofCosmicClust.hst.root
# rm ${RunFile}
# ln -s ${CalFile} ${RunFile} 
 echo use calibrations from  ${CalFile}
fi

echo gen_digi for $cRun with iDut=$iDut, iRef=$iRef, iSet=$iCalSet, iSel2=$iSel2, iBRef=$iBRef, Deadtime=$Deadtime, CalFile=$CalFile

mkdir $cRun
cd    $cRun 
cp    ../.rootrc .
cp    ../rootlogon.C .
cp -v ../${CalFile} ./${RunFile}
#root -b -q './ana_digi.C(-1,93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime') '
#root -b -q '../ana_digi_dev.C(-1,93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalIdMode'") '
root -b -q '../ana_digi_cal.C(-1,93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalIdMode'") '
#root -b -q '../ana_digi_cos.C(-1,93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalIdMode'") '
#root -b -q '../ana_digi_star.C(-1,93,1,'$iRef',1,"'$cRun'",'$iCalSet',1,'$iSel2','$Deadtime',"'$CalIdMode'") '

