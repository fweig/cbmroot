#!/bin/bash
# Copyright (C) 2015 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Egor Ovcharenko


#root -l -b -q "run_analysis_calibrate.C(kTRUE, \"$2\", \"$7\", \"$4\", \"$5\", \"$6\", \"$1\")" > $8
root -l -b -q "run_analysis_ringscan.C(kFALSE, \"$2\", \"$3\", \"$4\", \"$5\", \"$6\", \"$1\")" > $8
