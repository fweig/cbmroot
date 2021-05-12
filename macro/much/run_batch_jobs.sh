#!/bin/bash
# Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Anna Senger


cbmroot_dir=$1

chmod +x batch_run_bg.sh
chmod +x batch_run_sgn.sh

sbatch -p main --array=0-99 --time=08:00:00 -- /lustre/cbm/users/anna/trunk_NOV20/macro/much/batch_run_bg.sh $cbmroot_dir

for i in `seq 0 6`;
do

  sbatch -p main --array=0-99 --time=08:00:00 -- /lustre/cbm/users/anna/trunk_NOV20/macro/much/batch_run_sgn.sh $cbmroot_dir $i
  
done

