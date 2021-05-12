#!/bin/bash
# Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Jordan Bendarouach


NEVTS=200
INPUT=4		# 1: 10e- & 10e+ ; 2: AuAu@10 & 5e- & 5e+ ; 3: AuAu@8 & 5e- & 5e+ ; 4: AuAu@25 & 5e- & 5e+
GEOM=0		# 0: aligned ; 1: 1mrad ; 2: 2mrad ; 3: 3mrad ; 4: 5mrad

#for GEOM in 1 2 3 4
#do
	root -l -b -q "run_sim.C(${NEVTS}, ${INPUT}, ${GEOM})"
	root -l -b -q "run_reco.C(${NEVTS}, ${INPUT}, ${GEOM})"
#done
