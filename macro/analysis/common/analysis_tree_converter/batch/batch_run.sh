#!/bin/bash
# Copyright (C) 2020 Physikalisches Institut, Eberhard Karls Universität Tübingen, Tübingen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Viktor Klochkov

#SBATCH -J CbmAnalysisTree
#SBATCH -o out/%j.out.log
#SBATCH -e error/%j.err.log
#SBATCH --time=8:00:00
#SBATCH --array=1-100

N_EVENTS=500
SETUP="sis100_electron"

CBMROOT_DIR=/lustre/cbm/users/klochkov/soft/cbmroot/c2f_fork/

source $CBMROOT_DIR/build/config.sh

INDEX=$SLURM_ARRAY_TASK_ID

# INPUT_DIR=/lustre/cbm/users/ogolosov/mc/cbmsim/test_prod_AT/dcmqgsm_smm_pluto/auau/12agev/mbias/psd44_hole20_pipe0/TGeant3/
OUTPUT_DIR=/lustre/nyx/cbm/users/klochkov/cbm/test_prod_AT/urqmd/auau/12agev/mbias/TGeant3/$INDEX
INPUT_FILE=/lustre/cbm/users/ogolosov/mc/generators/urqmd/v3.4/auau/pbeam12agev_eos0/mbias/root/urqmd_$INDEX.root

DATA_SET=$INDEX
MACRO_DIR=$CBMROOT_DIR/macro/analysis/common/analysis_tree_converter/

mkdir -p $OUTPUT_DIR
cd $OUTPUT_DIR

INPUT_DIR=$INPUT_DIR/$DATA_SET/

cp $CBMROOT_DIR/macro/run/run_transport.C ./
cp $CBMROOT_DIR/macro/run/run_digi.C ./
cp $CBMROOT_DIR/macro/run/run_reco_event.C ./
cp $CBMROOT_DIR/macro/include/.rootrc ./

cp $MACRO_DIR/run_analysis_tree_maker.C ./

root -l -q -b "run_transport.C($N_EVENTS, \"$SETUP\", \"$DATA_SET\", \"$INPUT_FILE\")" >& log_tr_$INDEX.txt
root -l -q -b "run_digi.C(\"$DATA_SET\", $N_EVENTS, \"$DATA_SET\", -1)" >& log_digi_$INDEX.txt
root -l -q -b "run_reco_event.C($N_EVENTS, \"$DATA_SET\", \"$SETUP\")" >& log_reco_$INDEX.txt
root -l -q -b "run_analysis_tree_maker.C(\"$DATA_SET\", \"$SETUP\",\"$INPUT_FILE\")" >& log_$INDEX.txt

