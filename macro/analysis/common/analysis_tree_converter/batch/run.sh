# Copyright (C) 2020 Physikalisches Institut, Eberhard Karls Universität Tübingen, Tübingen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Viktor Klochkov

LOGDIR=/lustre/cbm/users/$USER/log
mkdir -p $LOGDIR
mkdir -p $LOGDIR/out
mkdir -p $LOGDIR/error

sbatch --partition main -D "$LOGDIR" --export=ALL batch_run.sh
