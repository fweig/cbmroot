#!/bin/bash

cbmroot_config_path=/lustre/nyx/cbm/users/jbendar/CBMINSTALL/bin/CbmRootConfig.sh
source ${cbmroot_config_path}

nEvs=500000
NUM="00001"

for FLAG in 0 1
do
	root -b -q -l "run_sim_position.C($nEvs, $FLAG)"
	root -b -q -l "run_reco_position.C($nEvs, $FLAG)"
	root -b -q -l "Compute_distance.C("", $FLAG)"
done
