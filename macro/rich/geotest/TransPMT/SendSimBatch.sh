#!/bin/sh

nEvs=10
GeoCase=2
PtNotP=1
MomMin=0
MomMax=4
RX=5
RY=5
RotMir=-10

export SCRIPT=yes

export N_EVS=$nEvs
export GEO_CASE=$GeoCase
export PT_NOT_P=$PtNotP
export MOM_MIN=$MomMin
export MOM_MAX=$MomMax
export PMT_ROTX=$RX
export PMT_ROTY=$RY
export ROTMIR=$RotMir


#for TrY in -20 -15 -10 -5 0 5 10 15 20;do
for TrY in 20;do
    export PMT_TRAY=$TrY
#    for TrZ in -20 -15 -10 -5 0 5 10 15 20;do
    for TrZ in 20;do
	export PMT_TRAZ=$TrZ
	
	#root -b -q "CreateGeo/CreateTransGDMLfile_April2015.C($TrY, $TrZ, $RotMir, $RX, $RY)"
	#root -b -q "CreateGeo/GDML_to_ROOT_Trans.C($TrY, $TrZ, $RotMir, $RX, $RY)"
	
	root -b -l -q "Run_Sim_GeoOpt_Batch.C($nEvs,  $TrY, $TrZ, $RX, $RY, $RotMir )"
	root -b -l -q "Run_Reco_GeoOpt_Batch.C($nEvs,  $TrY, $TrZ, $RX, $RY, $RotMir )"
	root -b -l -q "Run_Ana_GeoOpt_Batch.C($nEvs,  $TrY, $TrZ, $RX, $RY, $RotMir )"
    done
done
export SCRIPT=no
