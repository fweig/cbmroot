#!/bin/bash
#SBATCH -J CbmAnalysisTree
#SBATCH -o out/%j.out.log
#SBATCH -e error/%j.err.log
#SBATCH --time=2:00:00
#SBATCH --array=1-50

N_EVENTS=100
SETUP="sis100_electron"

CBMROOT_DIR=/lustre/cbm/users/klochkov/soft/cbmroot/c2f_fork/

source $CBMROOT_DIR/build/config.sh

INDEX=$SLURM_ARRAY_TASK_ID

# INPUT_DIR=/lustre/cbm/users/ogolosov/mc/cbmsim/test_prod_AT/dcmqgsm_smm_pluto/auau/12agev/mbias/psd44_hole20_pipe0/TGeant3/
OUTPUT_DIR=/lustre/nyx/cbm/users/klochkov/cbm/test_prod_AT/dcmqgsm_smm_pluto/auau/12agev/mbias/psd44_hole20_pipe0/TGeant3/$INDEX
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

cp $MACRO_DIR/run_treemaker.C ./

root -l -q -b "run_transport.C($N_EVENTS, \"$SETUP\", \"$DATA_SET\", \"$INPUT_FILE\")" >& log_tr_$INDEX.txt
root -l -q -b "run_digi.C($N_EVENTS, \"$DATA_SET\", 1e7, 1e4, kTRUE)" >& log_digi_$INDEX.txt
root -l -q -b "run_reco_event.C($N_EVENTS, \"$DATA_SET\", \"$SETUP\")" >& log_reco_$INDEX.txt
root -l -q -b "run_treemaker.C(\"\", \"$DATA_SET\")" >& log_$INDEX.txt
