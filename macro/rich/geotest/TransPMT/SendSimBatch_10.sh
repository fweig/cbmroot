#!/bin/sh

nEvs=10000
cbmroot_config_path=/hera/cbm/users/tariq/cbmroot/buildcbm/config.sh
source ${cbmroot_config_path}

RotMir=-10;  
RX=37; RY=13;
	  
for TrY in -50 -40 -30 -20 -10 0 10 20 30 40 50;do

    for TrZ in -50 -40 -30 -20 -10 0 10 20 30 40 50;do

	root -b -q "CreateGeo/CreateTransGDMLfile_April2015.C($TrY, $TrZ, $RotMir, $RX, $RY)"
	root -b -q "CreateGeo/GDML_to_ROOT_Trans.C($TrY, $TrZ, $RotMir, $RX, $RY)"
	
	qsub -l mem=5G,ct=2:00:00 ./RunSimOnBatch.sh $TrY $TrZ $RotMir $nEvs $RX $RY
	
    done
done
