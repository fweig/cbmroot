#!/bin/bash
#SBATCH -J histo_bg_mix
#SBATCH -D batch
#SBATCH -o %j.out
#SBATCH -e %j.err
#SBATCH --time=08:00:00
#SBATCH -p main
#SBATCH --array=0-3999

TYPE=invM_mix

home_dir=/lustre/hebe/cbm/users/anna/OCT19
work_dir=${home_dir}/macro/analysis/much

cd ${home_dir}/build
. ./config.sh
export DISPLAY=localhost:0.0

cd ${work_dir}

batch_dir=batch
if [ ! -d $batch_dir ]; then
    mkdir $batch_dir
fi

root -l -b -q "InvariantMassSpectra_mix.C(8,\"$TYPE\",5000,1e8,$SLURM_ARRAY_TASK_ID)"
