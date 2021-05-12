#!/bin/bash
# Copyright (C) 2019 Institut für Kernphysik, Goethe-Universitaet Frankfurt, Frankfurt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Etienne Bechtel


## split level: defines number of files per jobs
split_level=1;

## number of events: 0=all
NEVENT=0

## unique train id
train="test"

## input directory of simulated and reconstructed data
indir=$PWD/testdata

## output directory
out=$indir/$train
mkdir -p $out

## get CBM setup used in simulation and reconstruction
SETUP=$(basename $indir/setup* .C)
SETUP=${SETUP/setup_/}

## copy user config and analysis macro to train output directory
## for backup and documentation
cp -v "$PWD/Config_dilepton_testing.C"    "$out/."
cp -v "$PWD/run_analysis.C" "$out/."

## get run list and sort it
if grep -Fq "$indir/" runListSort.txt; then
    echo "sorted run list already there, only split the list";
else
    ## only collect file with validated reconstruction output
    find $indir -type f -name "rec.root" -printf "%h\n" | head -n $split_level > runList.txt;
    sort runList.txt > runListSort.txt
fi

## split file list by split_level into many files (1 list per job)
split -a 4 -l $split_level runListSort.txt filelist_${train}_
ListOfFilelist=`ls filelist_${train}_*`

# loop over all lists
I=0
for filelist in $ListOfFilelist ; do
    let I=$I+1

    ## create output directory for job $I
    outdir=$out/`printf "%04d/" $I`
    echo "-$outdir-"
    mkdir -p $outdir

    ## test analysis (abort after first job)
    # clean up
    rm $(ls filelist_${train}_* | grep -v aaaa)
    rm -v runListSort.txt;
    ./analysis.sh $indir $filelist $outdir $NEVENT     # LOCAL run
    rm -v filelist_${train}_*
    break;

done
