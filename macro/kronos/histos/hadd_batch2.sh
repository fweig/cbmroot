#!/bin/bash
#SBATCH -J hadd
#SBATCH -o %j.out.log
#SBATCH -e %j.err.log
#SBATCH --time=06:00:00
#SBATCH --array=0-49

X=$SLURM_ARRAY_TASK_ID
XXX=$(printf "%03d" "$X")

. $CBMROOT_DIR/config.sh
indir=$OUTTMP
outdir=$OUTTMP

outfile=$outdir/analysis.temp-all2.${XXX}.root
infiles=$outdir/analysis.temp-all.${XXX}*.root

echo "Adding histograms $XXX: input=$infiles, output=$outfile"
hadd  -T -f -k ${outfile} ${infiles}
