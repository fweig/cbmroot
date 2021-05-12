#!/bin/bash
# Copyright (C) 2019 Institut für Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Etienne Bechtel


LOCATION=/lustre/nyx

## choose cbm root installation
#source $LOCATION/cbm/users/$USER/CBMsoft/CbmRoot/trunk/build/config.sh
#source $LOCATION/cbm/users/$USER/CBMsoft/cbm-env.sh -n 4

## job content
export INDIR="$1"  ## path to simreco directory
export INFILE="$2" ## path to input file list for this jobs
export OUTDIR="$3" ## output directory


## number of events to process
NEVT=$4

${ROOTSYS}/bin/root -l -b -q "$OUTDIR/../run_analysis.C($NEVT)"
## cleanup
rm -v L1_histo.root
