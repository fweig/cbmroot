#!/bin/bash

# To run this script in stand-alone mode one needs to provide a ROOT environment
# and to specify an output directory 'outdir'.
#
# source init_calib_digi.sh RunId CalSet
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

nEvi=100000

echo "Initialize clusterizer calibration for run CbmTofSps_$RunId"
echo "Calib setup is ${iCalSet}, iSet $iSet, iDut $iDut, iMRef $iMRef, iBRef $iBRef"


iRestart=0
iStep=0
iStepLast=0

if false; then
#if true; then

  for inOpt in ''$nEvi',0,0,0,50' ''$nEvi',03,0,0,50' ''$nEvi',13,0,0,50' ''$nEvi',23,0,0,50' ''$nEvi',23,0,0,50' ''$nEvi',33,0,0,50' ''$nEvi',33,0,0,50'  ''$nEvi',33,0,0,50' ''$nEvi',43,0,0,50' ''$nEvi',43,0,0,50' ''$nEvi',43,0,0,50' ''$nEvi',53,0,0,50' ''$nEvi',53,0,0,50' ''$nEvi',63,0,0,50' ''$nEvi',63,0,0,50'
  do

    ((iStepLast = ${iStep}))
    ((iStep += 1))

    mkdir ${outdir}/Init${iStep}

    cp -v ${outdir}/.rootrc ${outdir}/Init${iStep}/.rootrc
    cp -v ${outdir}/rootlogon.C ${outdir}/Init${iStep}/rootlogon.C

    cd ${outdir}/Init${iStep}


    if [ "$iStep" -gt "$iRestart" ]; then
      # copy previous calibration file
      if [ "$iStep" -gt "1" ]; then
        cp -v ${outdir}/Init${iStepLast}/calib_cluster.cor_out.root ${outdir}/Init${iStep}/calib_cluster.cor_in.root
      fi

      # generate new calibration file
      root -b -q ''${VMCWORKDIR}'/macro/tof/beamtime/nov15/calib_digi_ini.C('$inOpt',"CbmTofSps_'${RunId}'",'${iCalSet}')'

      rm ${outdir}/Init${iStep}/all_*
      rm ${outdir}/Init${iStep}/digi_ini.out.root

      echo "Init step $iStep with option $inOpt finished"
    else
      echo "Init step $iStep with option $inOpt skipped"
    fi

  done

  cp -v ${outdir}/Init${iStep}/calib_cluster_ini.hst.root ${outdir}/final_calib_cluster_init_ini_CbmTofSps_${RunId}_${iCalSet}.hst.root
 
fi


iRestart=0
iStep=15
iStepLast=14

#if false; then
if true; then

#  for inOpt in ''$nEvi',03,0,0,50' ''$nEvi',03,0,0,50' ''$nEvi',03,1,'${iMRef}',0' ''$nEvi',03,0,'${iDut}',0' ''$nEvi',03,1,'${iMRef}',0' ''$nEvi',03,0,'${iDut}',0' ''$nEvi',13,0,0,50' ''$nEvi',13,0,0,50' ''$nEvi',13,1,'${iMRef}',0'  ''$nEvi',13,0,'${iDut}',0' ''$nEvi',13,1,'${iMRef}',0'  ''$nEvi',13,0,'${iDut}',0' ''$nEvi',23,0,0,50' ''$nEvi',23,0,0,50' ''$nEvi',23,1,'${iMRef}',0'  ''$nEvi',23,0,'${iDut}',0' ''$nEvi',23,1,'${iMRef}',0'  ''$nEvi',23,0,'${iDut}',0' ''$nEvi',33,0,0,50' ''$nEvi',33,0,0,50' ''$nEvi',33,1,'${iMRef}',0'  ''$nEvi',33,0,'${iDut}',0' ''$nEvi',04,0,'-${iDut}',0' ''$nEvi',33,0,'${iDut}',0' ''$nEvi',04,1,'-${iMRef}',0' ''$nEvi',33,1,'${iMRef}',0'  ''$nEvi',43,0,0,50' ''$nEvi',43,0,0,50' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,0,50' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',53,0,0,50' ''$nEvi',53,0,0,50' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',52,0,0,0' ''$nEvi',52,0,0,50' '500000,14,0,-'${iDut}',0' '500000,14,1,-'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',63,0,'${iDut}',0' ''$nEvi',63,1,'${iMRef}',0' ''$nEvi',73,0,'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' ''$nEvi',73,0,'${iDut}',0' '500000,24,0,-'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' '500000,24,1,-'${iMRef}',0' ''$nEvi',83,0,'${iDut}',0' '500000,34,0,-'${iDut}',0' ''$nEvi',83,1,'${iMRef}',0' '500000,34,1,-'${iMRef}',0' ''$nEvi',93,0,'${iDut}',0' ''$nEvi',93,1,'${iMRef}',0'
  for inOpt in ''$nEvi',03,0,0,50' ''$nEvi',03,1,'${iMRef}',0' ''$nEvi',03,0,'${iDut}',0' ''$nEvi',03,1,'${iMRef}',0' ''$nEvi',03,0,'${iDut}',0' ''$nEvi',13,0,0,50' ''$nEvi',13,0,0,50' ''$nEvi',13,1,'${iMRef}',0' ''$nEvi',13,0,'${iDut}',0' ''$nEvi',13,1,'${iMRef}',0'  ''$nEvi',13,0,'${iDut}',0' ''$nEvi',23,0,0,50' ''$nEvi',23,0,0,50' ''$nEvi',23,1,'${iMRef}',0'  ''$nEvi',23,0,'${iDut}',0' ''$nEvi',23,1,'${iMRef}',0' ''$nEvi',23,0,'${iDut}',0' ''$nEvi',33,0,0,50' ''$nEvi',33,0,0,50' ''$nEvi',33,1,'${iMRef}',0' ''$nEvi',33,0,'${iDut}',0'  ''$nEvi',33,1,'${iMRef}',0' ''$nEvi',43,0,0,50' ''$nEvi',43,0,0,50' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,0,50' ''$nEvi',43,0,0,50' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',43,1,'${iMRef}',0' ''$nEvi',43,0,'${iDut}',0' ''$nEvi',53,0,0,50' ''$nEvi',53,0,0,50' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' '500000,14,0,-'${iDut}',0' '500000,14,1,-'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',53,0,'${iDut}',0' ''$nEvi',53,1,'${iMRef}',0' ''$nEvi',63,0,'${iDut}',0' ''$nEvi',63,1,'${iMRef}',0' ''$nEvi',63,0,0,50' ''$nEvi',63,0,0,50' ''$nEvi',63,0,'${iDut}',0' ''$nEvi',63,1,'${iMRef}',0' ''$nEvi',63,0,'${iDut}',0' ''$nEvi',63,1,'${iMRef}',0' ''$nEvi',73,0,'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' ''$nEvi',73,0,0,50' ''$nEvi',73,0,0,50' ''$nEvi',73,0,'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' ''$nEvi',73,0,'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' ''$nEvi',73,0,'${iDut}',0' '500000,24,0,-'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' '500000,24,1,-'${iMRef}',0' ''$nEvi',73,0,0,50' ''$nEvi',73,0,0,50' ''$nEvi',73,0,'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' ''$nEvi',73,0,0,50' ''$nEvi',73,0,0,50' ''$nEvi',73,0,'${iDut}',0' ''$nEvi',73,1,'${iMRef}',0' ''$nEvi',83,0,'${iDut}',0'  ''$nEvi',83,1,'${iMRef}',0'  ''$nEvi',83,0,'${iDut}',0'  ''$nEvi',83,1,'${iMRef}',0' '500000,34,0,-'${iDut}',0' ''$nEvi',83,1,'${iMRef}',0' '500000,34,1,-'${iMRef}',0' ''$nEvi',93,0,'${iDut}',0' ''$nEvi',93,1,'${iMRef}',0' ''$nEvi',93,0,'${iDut}',0'  ''$nEvi',93,1,'${iMRef}',0' 
  do   
    ((iStepLast = ${iStep}))
    ((iStep += 1))

    mkdir ${outdir}/Init${iStep}

    cp -v ${outdir}/.rootrc ${outdir}/Init${iStep}/.rootrc
    cp -v ${outdir}/rootlogon.C ${outdir}/Init${iStep}/rootlogon.C

    cd ${outdir}/Init${iStep}


    if [ "$iStep" -gt "$iRestart" ]; then
      # copy previous calibration file
      if [ "$iStep" -gt "1" ]; then
        cp -v ${outdir}/Init${iStepLast}/calib_cluster.cor_out.root ${outdir}/Init${iStep}/calib_cluster.cor_in.root
      fi

      # generate new calibration file
      root -b -q ''${VMCWORKDIR}'/macro/tof/beamtime/nov15/calib_digi_cal.C('$inOpt',"CbmTofSps_'${RunId}'",'${iCalSet}')'

      rm ${outdir}/Init${iStep}/all_*
      rm ${outdir}/Init${iStep}/digi_cal.out.root

      echo "Init step $iStep with option $inOpt finished"
    else
      echo "Init step $iStep with option $inOpt skipped"
    fi

  done

  cp -v ${outdir}/Init${iStep}/calib_cluster_cal.hst.root ${outdir}/final_calib_cluster_init_cal_CbmTofSps_${RunId}_${iCalSet}.hst.root
  cp -v ${outdir}/Init${iStep}/calib_cluster.cor_out.root ${outdir}/final_calib_cluster_init_CbmTofSps_${RunId}_${iCalSet}.cor_out.root

fi
