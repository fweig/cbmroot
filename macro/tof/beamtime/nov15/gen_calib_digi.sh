#!/bin/bash

# To run this script in stand-alone mode one needs to provide a ROOT environment
# and to specify an output directory 'outdir'.
#
# source gen_calib_digi.sh RunId CalSet Sel2
#

if [ -z "$RunId" ]; then
  if [ -z "$1" ]; then
    echo 'no run to calibrate specified'
    exit 1
  else
    RunId=$1
  fi
fi

if [ -z "$iCalSet" ]; then
  if [ -z "$2" -o "${#2}" != "9" ]; then
    echo 'no valid calibration set specified'
    exit 1
  else
    iCalSet=$2
  fi
fi

if [ -z "$iSel2" ]; then
  if [ -z "$3" -o "${#3}" != "3" ]; then
    echo 'no valid Sel2 MRPC specified'
    exit 1
  else
    iSel2=$3
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


if [ -z "$iDut" ]; then
  ((iTmp  = $iCalSet ))
  ((iBRef = $iTmp % 1000))
  ((iTmp  = $iTmp - $iBRef))
  ((iSet  = $iTmp / 1000))
  ((iMRef = $iTmp % 1000000))
  ((iMRef = $iMRef / 1000))
  ((iTmp  = $iTmp - $iMRef))
  ((iDut  = $iTmp / 1000000))
fi

echo "Finalize clusterizer calibration for run CbmTofSps_$RunId"
echo "Calib setup is ${iCalSet}_${iSel2}, iSet $iSet, iDut $iDut, iMRef $iMRef, iBRef $iBRef, Sel2 $iSel2"

mkdir ${outdir}/Gen

cp -v ${outdir}/.rootrc ${outdir}/Gen/.rootrc
cp -v ${outdir}/rootlogon.C ${outdir}/Gen/rootlogon.C

cd ${outdir}/Gen

cp -v ${outdir}/final_calib_cluster_init_CbmTofSps_${RunId}_${iCalSet}.cor_out.root ${outdir}/Gen/calib_cluster.cor_in.root

#cp -v ${outdir}/final_calib_cluster_iter_CbmTofSps_${RunId}_${iCalSet}_${iSel2}.cor_out.root ${outdir}/Gen/calib_cluster.cor_in.root
#cp -v ${outdir}/final_calib_ana_iter_CbmTofSps_${RunId}_${iCalSet}_${iSel2}.cor_out.root ${outdir}/Gen/calib_ana.cor_in.root

root -b -q ''${VMCWORKDIR}'/macro/tof/beamtime/nov15/calib_digi.C(100000000,93,1,'$iMRef',0,"CbmTofSps_'$RunId'",'$iCalSet',1,'$cSel2')'

rm ${outdir}/Gen/all_*

mv -v ${outdir}/Gen/digi.out.root ${outdir}/digi_CbmTofSps_${RunId}_${iCalSet}_${iSel2}.out.root

cp -v ${outdir}/Gen/calib_cluster.hst.root ${outdir}/final_calib_cluster_gen_CbmTofSps_${RunId}_${iCalSet}_${iSel2}.hst.root
cp -v ${outdir}/Gen/calib_cluster.cor_out.root ${outdir}/final_calib_cluster_gen_CbmTofSps_${RunId}_${iCalSet}_${iSel2}.cor_out.root
cp -v ${outdir}/Gen/calib_ana.cor_out.root ${outdir}/final_calib_ana_gen_CbmTofSps_${RunId}_${iCalSet}_${iSel2}.cor_out.root
