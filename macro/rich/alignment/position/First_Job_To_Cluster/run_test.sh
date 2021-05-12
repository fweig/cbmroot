#!/bin/bash
# Copyright (C) 2019 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Jordan Bendarouach


sbatch -J position -D /lustre/nyx/cbm/users/jbendar -o %j_%N.out.log -e %j_%N.err.log --time=0:30:00 --array=1-3 ./test1.sh

# sbatch --array=1-3 ./test.sh
