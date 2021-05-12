#!/bin/sh
# Copyright (C) 2018 UGiessen,JINR-LIT
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Semen Lebedev

cd $MY_BUILD_DIR
. ./config.sh
cd -

#root -l -q -b "./run_sim_mcbm.C($NEVENTS)"
root -l -q -b "./run_reco_mcbm.C($NEVENTS)"

    

