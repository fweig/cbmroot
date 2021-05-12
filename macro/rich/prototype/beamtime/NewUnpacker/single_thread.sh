#!/bin/bash
# Copyright (C) 2015 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Egor Ovcharenko


root -l -b -q "run_analysis_thr_scan.C(kTRUE, \"$1\", \"$5\", \"$3\", \"$4\")" > $6
root -l -b -q "run_analysis_thr_scan.C(kFALSE, \"$1\", \"$2\", \"$3\", \"$4\")" > $6
