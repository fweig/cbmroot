#!/bin/bash
# Copyright (C) 2012 Institut fuer Kernphysik, Westfaelische Wilhelms-Universitaet Muenster, Muenster
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Cyrano Bergmann


root -l -b -q run_sim.C
root -l -b -q run_reco_clusterizer.C
