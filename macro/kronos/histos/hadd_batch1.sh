#!/bin/bash
#SBATCH -J hadd
#SBATCH -o %j.out.log
#SBATCH -e %j.err.log
#SBATCH --time=06:00:00
#SBATCH --array=0-499

X=$SLURM_ARRAY_TASK_ID
XXXX=$(printf "%04d" "$X")

. $CBMROOT_DIR/config.sh
indir=$INDIR
outdir=$OUTTMP

outfile=$outdir/analysis.temp-all.$XXXX.root

NUM_DIR=$(($SLURM_ARRAY_TASK_ID*10))
infiles=" "
for i in `seq 1 10`;
do
if [ -f $indir/$NUM_DIR/KFParticleFinder.root ]; then
  infiles=$infiles" "$indir/$NUM_DIR/KFParticleFinder.root
fi
NUM_DIR=$(($NUM_DIR + 1))
done

echo "Adding histograms ${XXXX}: input=${infiles}, output=${outfile}"
hadd  -T -f -k $outfile $infiles
