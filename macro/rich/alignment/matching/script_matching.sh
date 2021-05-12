#!/bin/bash
# Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Jordan Bendarouach


nEvs=5000
NUM=00001
BOOL=1

root -b -q -l "run_sim_matching.C($nEvs, \"$NUM\", $BOOL)"
root -b -q -l "run_reco_matching_alignment.C($nEvs, \"$NUM\", $BOOL)"
root -b -q -l "run_reco_matching_correction.C($nEvs, \"$NUM\", $BOOL)"
