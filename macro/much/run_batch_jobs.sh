#!/bin/bash

batch_dir=batch
if [ ! -d $batch_dir ]; then
    mkdir $batch_dir
fi

cbmroot_dir=$1

sbatch batch_run_bg.sh $cbmroot_dir

for i in `seq 0 6`;
do

  sbatch batch_run_sgn.sh $cbmroot_dir $i
  
done
