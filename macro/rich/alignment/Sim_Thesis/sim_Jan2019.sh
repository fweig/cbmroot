#!/bin/sh
# Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Jordan Bendarouach

cd $MY_BUILD_DIR
. ./config.sh
cd -

#root -b -q "./run_sim_Jan2019.C($NEVENTS)"
root -b -q "./run_reco_Jan2019.C($NEVENTS)"
