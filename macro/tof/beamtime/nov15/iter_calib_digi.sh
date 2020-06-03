#!/bin/bash

# To run this script in stand-alone mode one needs to provide a ROOT environment
# and to specify an output directory 'outdir'.
#
# source iter_calib_digi.sh RunId CalSet Sel2
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


c0='0000'

echo "Iterate clusterizer calibration for run CbmTofSps_$RunId"
echo "Calib setup is ${iCalSet}_${iSel2}, iSet $iSet, iDut $iDut, iMRef $iMRef, iBRef $iBRef, Sel2 $iSel2"

iRestart=0
iStep=0
iStepLast=0

# history for 1805: 1 6 8 2 10 2 10
#for mode in 1 6 8 2 10 4 5 4
#for mode in 2 10 2 10 
#for mode in 4
#for mode in 5
#for mode in 1
#for mode in 6 8 2 10 4 5 4
for mode in 1 6 8 
do

  case $mode in 
    1)
      nIter=5
      alist=`echo '1'$c0'0,93,1,'$iMRef'' '1'$c0'0,44,0,-'$iDut''  '1'$c0'0,93,0,'$iDut'' '1'$c0'0,44,1,-'$iMRef'' '1'$c0'00,41,1,-'$iMRef'' '1'$c0'0,93,0,'$iMRef'' '1'$c0'00,41,0,-'$iDut'' '1'$c0'0,93,1,'$iMRef''`
      ;;
    2)
      nIter=2
      alist=`echo '1'$c0'0,92,1,'$iMRef'' '1'$c0'00,64,1,-'$iMRef'' '1'$c0'00,64,1,'$iBRef'' '1'$c0'0,92,0,'$iDut'' '1'$c0'00,64,0,-'$iDut'' '1'$c0'00,64,0,'$iBRef'' '1'$c0'0,92,1,'$iMRef''`
      ;;
    3)
      nIter=5
      alist=`echo '1'$c0'0,44,0,-'$iDut'' '1'$c0'0,44,1,-'$iMRef'' `
      ;;
    4)
      nIter=5
      alist=`echo '5'$c0'0,93,1,'$iMRef'' '5'$c0'0,93,-2,2' '1'$c0'000,61,-1,-2' '5'$c0'0,93,-3,2' ` 
      ;;
    5)
      nIter=2
      alist=`echo '5'$c0'0,93,1,'$iMRef'' '5'$c0'0,93,-2,2' '1'$c0'00,64,1,-'$iMRef'' '1'$c0'00,64,0,-'$iDut'' '5'$c0'0,93,-3,2' ` 
      ;;
    6)
      nIter=2
      alist=`echo '1'$c0'0,93,1,'$iMRef'' '1'$c0'00,93,-2,-2' '1'$c0'0,93,0,'$iDut'' '1'$c0'00,93,-2,-2' '1'$c0'0,93,1,'$iMRef''` 
      ;;
    7)
      nIter=3
      alist=`echo '1'$c0',92,1,4' '1'$c0'00,93,-2,-2' '1'$c0',92,0,3' '1'$c0'00,93,-2,2' '1'$c0',92,1,4'` 
      ;;
    8)
      nIter=2
      alist=`echo '1'$c0'00,93,1,'$iMRef'' '1'$c0'0,92,0,'$iDut'' '1'$c0'000,61,-1,-2' '1'$c0'00,93,-2,2' '1'$c0'00,93,1,'$iMRef''`
      ;;
    9)
      nIter=2
      alist=`echo '1'$c0',92,1,4' '1'$c0',92,0,3' '1'$c0'00,61,-1,-2' '1'$c0'0,93,-2,2' '1'$c0',92,1,4'`
      ;;
   10)
      nIter=1
      alist=`echo '1'$c0'0,92,1,'$iMRef'' '1'$c0'00,93,-3,2' '1'$c0'00,93,-2,2' '1'$c0'00,61,-1,-2' '1'$c0'0,92,1,'$iMRef''`
      ;;
   11)
      nIter=1
      alist=`echo '1'$c0',92,1,'$iMRef'' '1'$c0'00,93,-3,2' '1'$c0'00,93,-2,2' '1'$c0'00,61,-1,-2' '1'$c0'0,92,1,'$iMRef''`
      ;;
  esac

  echo "execute $nIter iterations of ${alist}"

  while [ "$nIter" -gt "0" ]; do
    for inOpt in $alist
    do

      ((iStepLast = ${iStep}))
      ((iStep += 1))

      mkdir ${outdir}/Iter${iStep}

      cp -v ${outdir}/.rootrc ${outdir}/Iter${iStep}/.rootrc
      cp -v ${outdir}/rootlogon.C ${outdir}/Iter${iStep}/rootlogon.C

      cd ${outdir}/Iter${iStep}

      if [ "$iStep" -gt "$iRestart" ]; then

        # copy previous calibration files
        if [ "$iStep" -eq "1" ]; then
          cp -v ${outdir}/final_calib_cluster_init_CbmTofSps_${RunId}_${iCalSet}.cor_out.root ${outdir}/Iter${iStep}/calib_cluster.cor_in.root
        else
          cp -v ${outdir}/Iter${iStepLast}/calib_cluster.cor_out.root ${outdir}/Iter${iStep}/calib_cluster.cor_in.root
          cp -v ${outdir}/Iter${iStepLast}/calib_ana.cor_out.root ${outdir}/Iter${iStep}/calib_ana.cor_in.root
        fi

        # generate new calibration file
        root -b -q ''${VMCWORKDIR}'/macro/tof/beamtime/nov15/calib_digi.C('$inOpt',1,"CbmTofSps_'$RunId'",'$iCalSet',0,'$cSel2')'

        rm ${outdir}/Iter${iStep}/all_*
        rm ${outdir}/Iter${iStep}/digi.out.root

        echo "Iter step $iStep with option $inOpt finished"
      else
        echo "Iter step $iStep with option $inOpt skipped"
      fi

    done
    (( nIter -= 1))
  done

done 

cp -v ${outdir}/Iter${iStep}/calib_cluster.hst.root ${outdir}/final_calib_cluster_iter_CbmTofSps_${RunId}_${iCalSet}_${iSel2}.hst.root
cp -v ${outdir}/Iter${iStep}/calib_cluster.cor_out.root ${outdir}/final_calib_cluster_iter_CbmTofSps_${RunId}_${iCalSet}_${iSel2}.cor_out.root
cp -v ${outdir}/Iter${iStep}/calib_ana.cor_out.root ${outdir}/final_calib_ana_iter_CbmTofSps_${RunId}_${iCalSet}_${iSel2}.cor_out.root

