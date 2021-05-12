#!/bin/bash
# Copyright (C) 2021 Institut für Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Etienne Bechtel


LOCATION=/lustre/nyx

## choose cbm root installation
#source $LOCATION/cbm/users/$USER/CBMsoft/CbmRoot/trunk/build/config.sh
#source $LOCATION/cbm/users/$USER/CBMsoft/cbm-env.sh -n 4

## job content
export INDIR="$1"  ## path to simreco directory
## number of events to process
NEVT=$2
CONFIG=$3 ## analysis mode
GROUP=$4 ## grouping of files
TESTBIN="$5"
NAME=$6

export INFILE="$1" ## path to input file list for this jobs
export OUTDIR="$2"## output directory

I=0
n=0
while [ "$n" -lt "$GROUP" ]; do
    echo "file $n $8"
    filelist[$n]=$7
    shift
    let n=$n+1
done
n=0
while [ "$n" -lt "$GROUP" ]; do
    echo "out $n $8"
    outlist[$n]=$7
    shift
    let n=$n+1

done

echo "outlist  ${outlist[*]}"
echo "filelist  ${filelist[*]}"

while [ "$I" -lt "$GROUP" ]; do
    export OUTDIR="${outlist[$I]}"
    export INFILE="${filelist[$I]}"  ## path to simreco directory

    if [[ $CONFIG == "1" ]] ; then
	if [[ $TESTBIN == "0" ]] ; then
	    echo "$OUTDIR/../run_common.C($NEVT)"
	    ${ROOTSYS}/bin/root -l -b -q "$OUTDIR/../run_testing.C($NEVT,kFALSE,$NAME)"
	else
	    echo "$OUTDIR/../run_common.C($NEVT)"
	    ${ROOTSYS}/bin/root -l -b -q "$OUTDIR/../run_testing.C($NEVT,kTRUE,$NAME)"
	fi
    fi
    if [[ $CONFIG == "2" ]] ; then
	if [[ $TESTBIN == "0" ]] ; then
	    echo "$OUTDIR/../run_common_analysis.C($NEVT)"
	    ${ROOTSYS}/bin/root -l -b -q "$OUTDIR/../run_common_analysis.C($NEVT,kFALSE)"
	else
	    echo "$OUTDIR/../run_common_analysis.C($NEVT)"
	    ${ROOTSYS}/bin/root -l -b -q "$OUTDIR/../run_common_analysis.C($NEVT,kTRUE)"
	fi
    fi

    ## cleanup
    rm -v L1_histo.root

    let I=$I+1
done
