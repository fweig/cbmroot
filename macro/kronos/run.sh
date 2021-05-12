# Copyright (C) 2016 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Maxim Zyzak

LOGDIR=/lustre/nyx/cbm/users/$USER/log
mkdir -p $LOGDIR
mkdir -p $LOGDIR/out
mkdir -p $LOGDIR/error

export MAINDIR=`pwd`

sbatch -D "/lustre/nyx/cbm/users/$USER/log" --export=ALL batch_run.sh
