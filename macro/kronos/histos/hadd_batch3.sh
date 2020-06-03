#!/bin/bash
#SBATCH -J hadd
#SBATCH -o %j.out.log
#SBATCH -e %j.err.log
#SBATCH --time=06:00:00
#SBATCH --array=0-4

X=$SLURM_ARRAY_TASK_ID
XX=$(printf "%02d" "$X")

. $CBMROOT_DIR/config.sh
indir=$OUTTMP
outdir=$OUTTMP

outfile=$outdir/analysis.temp-all3.$XX.root
infiles=$outdir/analysis.temp-all2.$XX*.root

echo "Adding histograms $XX: input=$infiles, output=$outfile"
hadd  -T -f -k $outfile $infiles
