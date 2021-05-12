#!/bin/sh
# Copyright (C) 2013 UGiessen,JINR-LIT
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Semen Lebedev

cd $MY_BUILD_DIR
. ./config.sh
cd -

root -b -q "./run_ascii_generator.C($NEVENTS)"

root -b -q "./run_sim.C($NEVENTS)"

root -l "./run_reco.C($NEVENTS)"

