#!/bin/bash
# Copyright (C) 2016 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Julian Book


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
CONFIG=$5 ## analysis mode
export TESTBIN="$6"
NAME=$7

if [[ $CONFIG == "1" ]] ; then
    if [[ $TESTBIN == "1" ]] ; then
	echo "$OUTDIR/../run_testing.C($NEVT)"	
	${ROOTSYS}/bin/root -l -b -q "$OUTDIR/../run_testing.C($NEVT,kTRUE,$NAME)"
    fi
    if [[ $TESTBIN == "0" ]] ; then
	echo "$OUTDIR/../run_testing.C($NEVT)"
	${ROOTSYS}/bin/root -l -b -q "$OUTDIR/../run_testing.C($NEVT,kFALSE,$NAME)"
    fi
fi
if [[ $CONFIG == "2" ]] ; then
    if [[ $TESTBIN == "0" ]] ; then
	echo "$OUTDIR/../run_common_analysis.C($NEVT)"
	${ROOTSYS}/bin/root -l -b -q "$OUTDIR/../run_common_analysis.C($NEVT,kFALSE)"
    else
	echo "$OUTDIR/../run_common_analysis.C($NEVT)"
	cp -v "$OUTDIR/../../par.root" "$OUTDIR/par.root"
	${ROOTSYS}/bin/root -l -b -q "$OUTDIR/../run_common_analysis.C($NEVT,kTRUE)"
    fi
fi

#${ROOTSYS}/bin/root -l -b -q "$OUTDIR/../run_analysis_old.C($NEVT)"
## cleanup
rm -v L1_histo.root
