#!/bin/bash
# Copyright (C) 2018 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Pierre-Alain Loizeau


START_DIR=`pwd`

## Prepare log folders
LOGDIR=/lustre/nyx/cbm/users/ploizeau/log
mkdir -p $LOGDIR
mkdir -p $LOGDIR/out
mkdir -p $LOGDIR/error
mkdir -p $LOGDIR/tmp

#SBATCH -D "/lustre/nyx/cbm/users/ploizeau/log"
#SBATCH -J EffNoCorrCosy2018
#SBATCH -o out/%A_%a.out.log
#SBATCH -e out/%A_%a.out.log
#SBATCH --time=5:00:00
#SBATCH --array=0-88
##SBATCH --array=0-0

#SBATCH --partition long
#SBATCH --mem 5000

### t_SLURM_ARRAY_JOB_ID="test_mcbm"
### t_SLURM_ARRAY_TASK_ID=1

source /lustre/nyx/cbm/users/ploizeau/cbmroot_trunk_local/build/config.sh

export DISPLAY=localhost:0.0

ONE=1
SLURM_INDEX=`expr $SLURM_ARRAY_TASK_ID + $ONE`

INDEX=$(printf "%04u" "$SLURM_INDEX")

IODIR="/lustre/nyx/cbm/users/ploizeau/cbmroot_trunk_local/trunk/macro/beamtime/cosy2018/"

## cleanup
rm $IODIR/data/*NoCorr*r$INDEX.root

cd $IODIR

root -l -b -q "$IODIR/EfficiencyMonitor_kronos.C( $SLURM_INDEX, kFALSE )"

cd $START_DIR
