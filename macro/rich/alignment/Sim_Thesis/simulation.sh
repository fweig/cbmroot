#!/bin/bash

export SCRIPT=no

if [ $1 -eq 1 ]	; then
	outDir=/data/Sim_Outputs/Position/minusX/
	setupName=setup_v17a_1e_3mrad_minusX
elif [ $1 -eq 2 ]	; then
	outDir=/data/Sim_Outputs/Position/plusX/
	setupName=setup_v17a_1e_3mrad_plusX
elif [ $1 -eq 3 ]	; then
	outDir/data/Sim_Outputs/Position/minusY/
	setupName=setup_v17a_1e_3mrad_minusY
elif [ $1 -eq 4 ]	; then
	outDir=/data/Sim_Outputs/Position/plusY/
	setupName=setup_v17a_1e_3mrad_plusY
fi

echo ${outDir}
echo ${setupName}
export OUT_DIR=${outDir}
export SETUP_NAME=${setupName}

macro_dir=/data/ROOT6/trunk/macro/rich/alignment/misalignment_correction

#root -l -b -q "${macro_dir}/single_tile/run_sim.C(${2})"
root -l -b -q "${macro_dir}/single_tile/run_reco.C(${2})"
