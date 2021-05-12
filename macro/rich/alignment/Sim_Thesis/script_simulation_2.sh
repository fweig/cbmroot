#!/bin/bash
# Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Jordan Bendarouach


NEVTS=50000
macro_dir=/data/ROOT6/trunk/macro/rich/alignment/misalignment_correction/Sim_Thesis

root -l -b -q "${macro_dir}/run_sim_5.C(${NEVTS})"
root -l -b -q "${macro_dir}/run_reco_5_alignment.C(${NEVTS})"
root -l -b -q "${macro_dir}/run_reco_5_correction.C(${NEVTS})"
