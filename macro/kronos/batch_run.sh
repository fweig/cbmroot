#!/bin/bash
# Copyright (C) 2016 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Maxim Zyzak

#SBATCH -J CbmSim
#SBATCH -o out/%j.out.log
#SBATCH -e error/%j.err.log
#SBATCH --time=07:00:00
#SBATCH --array=0-4999

GENERATOR=urqmd
SYSTEM=pni
ENERGY=15gev
CENTRALITY=centr
SETUP=sis100_electron

cd /lustre/nyx/cbm/users/$USER/cbmroot/build
. ./config.sh
export DISPLAY=localhost:0.0

NEVENTS=10000

ONE=1
SLURM_INDEX=`expr $SLURM_ARRAY_TASK_ID + $ONE`
INDEX=$(printf "%05d" "$SLURM_INDEX")

INPUTFILE=/lustre/nyx/cbm/prod/gen/$GENERATOR/$SYSTEM/$ENERGY/$CENTRALITY/$GENERATOR.$SYSTEM.$ENERGY.$CENTRALITY.$INDEX.root; 

INPUTDIR="/lustre/nyx/cbm/users/$USER/mc/"$GENERATOR"_"$SYSTEM"_"$ENERGY"_"$CENTRALITY"_"$SETUP/$SLURM_ARRAY_TASK_ID

OUTDIR="/lustre/nyx/cbm/users/$USER/mc/"$GENERATOR"_"$SYSTEM"_"$ENERGY"_"$CENTRALITY"_"$SETUP/$SLURM_ARRAY_TASK_ID
mkdir -p $OUTDIR
cd $OUTDIR

cp -rf "$MAINDIR/macro/run_mc.C" .
root -l -b -q "run_mc.C(\"$INPUTFILE\",\"$SETUP\",$NEVENTS)"

# cp -rf "$MAINDIR/macro/run_reco.C" .
# root -l -b -q "run_reco.C(\"$SETUP\",$NEVENTS)"

# cp -rf "$MAINDIR/macro/run_phys.C" .
# root -l -b -q "run_phys.C(\"$SETUP\",$NEVENTS,\"$INPUTDIR\")"
