#!/bin/bash

cbmroot_dir=$1

chmod +x batch_run_bg.sh
chmod +x batch_run_sgn.sh

sbatch -p main --array=0-99 --time=08:00:00 -- /lustre/cbm/users/anna/trunk_NOV20/macro/much/batch_run_bg.sh $cbmroot_dir

for i in `seq 0 6`;
do

  sbatch -p main --array=0-99 --time=08:00:00 -- /lustre/cbm/users/anna/trunk_NOV20/macro/much/batch_run_sgn.sh $cbmroot_dir $i
  
done

