#!/bin/bash
# Copyright (C) 2016 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Maxim Zyzak


MAINDIR=`pwd`

LOGDIR=/lustre/nyx/cbm/users/$USER/log
cd $LOGDIR

for f in $LOGDIR/out/*.log;
do
  if [  -f $f ]
  then
    echo "deleting $f"
    rm -rf "$f"
  fi
  sleep 0.005
done

for f in $LOGDIR/error/*.log;
do
  if [  -f $f ]
  then
    echo "deleting $f"
    rm -rf "$f"
  fi
  sleep 0.005
done

cd $MAINDIR
