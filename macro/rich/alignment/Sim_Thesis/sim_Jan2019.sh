#!/bin/sh
cd $MY_BUILD_DIR
. ./config.sh
cd -

#root -b -q "./run_sim_Jan2019.C($NEVENTS)"
root -b -q "./run_reco_Jan2019.C($NEVENTS)"
