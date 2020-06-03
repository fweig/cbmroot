#!/bin/bash
# To run this script in stand-alone mode one needs to provide a ROOT environment
# and to specify an output directory 'outdir'.
#
# source iter_ana_hits.sh RunId InSet dScalFac iDut iMRef iSel2
#

if [ -z "$RunId" ]; then
  if [ -z "$1" ]; then
    echo 'no run to analyze specified'
    exit 1
  else
    RunId=$1
  fi
fi

if [ -z "$InSet" ]; then
  if [ -z "$2" -o "${#2}" != "13" ]; then
    echo 'no valid calibration set specified'
    exit 1
  else
    InSet=$2
  fi
fi

if [ -z "$dScalFac" ]; then
  if [ -z "$3" ] || [[ ! "$3" =~ [0-9]\.[0-9] ]]; then
    echo 'no valid scaling factor specified'
    exit 1
  else
    dScalFac=$3
  fi
fi

if [ -z "$iDut" ]; then
  if [ -z "$4" -o "${#4}" != "3" ]; then
    echo 'no valid Dut MRPC specified'
    exit 1
  else
    iDut=$4
  fi
fi

if [ -z "$iMRef" ]; then
  if [ -z "$5" -o "${#5}" != "3" ]; then
    echo 'no valid MRef MRPC specified'
    exit 1
  else
    iMRef=$5
  fi
fi

if [ -z "$iSel2" ]; then
  if [ -z "$6" -o "${#6}" != "3" ]; then
    echo 'no valid Sel2 MRPC specified'
    exit 1
  else
    iSel2=$6
  fi
fi

cSel2=$iSel2

if [ "$iSel2" == "000" ]; then
  cSel2=0
fi

if [ -z "$VMCWORKDIR" ]; then
  echo 'no ROOT environment available'
  exit 1
fi

if [ -z "$outdir" ]; then
  echo 'variable outdir not specified'
  exit 1
fi

iBRef=${InSet:6:3}


dDTres=10000000
compare_result=1

iStep=0
iStepLast=0

while [ "$compare_result" -eq "1" ]; do

  for iCal in 1 2 3 5 6 1
  do

    ((iStepLast = ${iStep}))
    ((iStep += 1))

    mkdir ${outdir}/Iter${iStep}

    cp -v ${outdir}/.rootrc ${outdir}/Iter${iStep}/.rootrc
    cp -v ${outdir}/rootlogon.C ${outdir}/Iter${iStep}/rootlogon.C

    cd ${outdir}/Iter${iStep}

    # copy previous calibration file
    if [ "$iStep" -gt "1" ]; then
      cp -v ${outdir}/Iter${iStepLast}/calib_ana.cor_out.root ${outdir}/Iter${iStep}/calib_ana.cor_in.root
    fi

    # generate new calibration file
    root -b -q ''${VMCWORKDIR}'/macro/tof/beamtime/nov15/ana_calib_hits.C(10000000,'${iCal}',"CbmTofSps_'${RunId}'","'${InSet}'",'${iDut}','${iMRef}','${iBRef}','${cSel2}','${dScalFac}')'

    rm ${outdir}/Iter${iStep}/all_*
    rm ${outdir}/Iter${iStep}/hits.out.root

  done

  if [ ! -e ./Test.res ]; then
    echo 'file Test.res does not exist'
    exit 1
  fi

  Tres=`cat ./Test.res`
  dTdif=`echo "$dDTres - $Tres" | bc`
  dDTres=`echo "$dDTres - 0.001" | bc`
  compare_result=`echo "$Tres < $dDTres" | bc`

  echo "got Tres = $Tres, compare to $dDTres, dTdif = $dTdif, compare_result = $compare_result"

  if [ "$compare_result" -eq "1" ]; then
    dDTres=$Tres
  fi

done

# final action -> scan full statistics
mkdir ${outdir}/Final

cp -v ${outdir}/.rootrc ${outdir}/Final/.rootrc
cp -v ${outdir}/rootlogon.C ${outdir}/Final/rootlogon.C

cd ${outdir}/Final

if [ "$iStep" -gt "1" ]; then
  cp -v ${outdir}/Iter${iStep}/calib_ana.cor_out.root ${outdir}/Final/calib_ana.cor_in.root
fi

root -b -q ''${VMCWORKDIR}'/macro/tof/beamtime/nov15/ana_calib_hits.C(1000000000,1,"CbmTofSps_'${RunId}'","'${InSet}'",'${iDut}','${iMRef}','${iBRef}','${cSel2}','${dScalFac}')'

rm ${outdir}/Final/all_*
rm ${outdir}/Final/hits.out.root

cp -v ${outdir}/Final/ana_cluster.hst.root ${outdir}/final_ana_cluster_CbmTofSps_${RunId}_${iDut}_${iMRef}_${iSel2}_${InSet}_${dScalFac}.hst.root
cp -v ${outdir}/Final/calib_ana.cor_out.root ${outdir}/final_calib_ana_CbmTofSps_${RunId}_${iDut}_${iMRef}_${iSel2}_${InSet}_${dScalFac}.cor_out.root

