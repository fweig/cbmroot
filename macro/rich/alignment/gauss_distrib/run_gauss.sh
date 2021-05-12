#!/bin/bash
# Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Jordan Bendarouach


export SCRIPT=no
nEvts1=50000
nEvts2=90000
nEvts3=41000
macroDir=/data/ROOT6/trunk/macro/rich/alignment/misalignment_correction/gauss_distrib/

nEvts1=100
nEvts3=100

root -l -b -q "${macroDir}/run_sim.C(${nEvts1})"
#root -l -b -q "${macroDir}/run_reco_alignment.C(${nEvts2})"
#root -l -b -q "${macroDir}/run_reco.C(${nEvts3}, 0)"
#root -l -b -q "${macroDir}/run_reco.C(${nEvts3}, 1)"
root -l -b -q "${macroDir}/run_reco.C(${nEvts3}, 2)"
