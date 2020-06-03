#!/bin/sh

cbmroot_config_path=/hera/cbm/users/tariq/cbmroot/buildcbm/config.sh
source ${cbmroot_config_path}

nEvs=10000
RotMir=$1  
#if [ [ $RotMir -ne 1 ] && [ $RotMir -ne -10 ] ];then
#if [ -n $RotMir ];then
#    echo "RotMir is $RotMir: Set it to 1 or -10"
#    exit 2
#fi

TrY=0
TrZ=0
for ((RX=0; RX<=0; RX++)); do
    for ((RY=0; RY<=0; RY++)); do
	root -b -q "CreateGeo/CreateGDMLfile_April2015.C($TrY, $TrZ, $RotMir, $RX, $RY)"
	root -b -q "CreateGeo/GDML_to_ROOT.C($TrY, $TrZ, $RotMir, $RX, $RY)"
 	qsub -l mem=2G,ct=2:00:00 ./RunSimOnBatch.sh $RX $RY $RotMir $nEvs $TrY $TrZ

    done
done
