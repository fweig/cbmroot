#!/bin/bash

## test train or batch submission
TEST=1

## number of events: default=1000
NEVENT=1000

## number of runs
NRUNS=10000

## storage element and directory
LOCATION=/lustre/nyx
DIR=urqmd

## test parameters
if [[ $TEST == "1" ]] ; then
    DIR="TEST${DIR}"
    NEVENT=2
    NRUNS=1
fi


## output directory for simulated and reconstructed data
outdir=$LOCATION/$USER/cbm/$DIR/auau/8gev/centr
mkdir -p "$outdir"

## configuration file to output directory
## for backup and documentation
cp -nv "inputfile_AuAu_8"   "$outdir/input"

## set number of events per job
sed -i 's/nev 1000/nev '$NEVENT'/' "$outdir/input"

## loop over all runs
I=0
while [ "$I" -lt "$NRUNS" ]; do

  ## create output directory for job $I
  out=$outdir/`printf "%05d" $I`
  mkdir -p "$out"

  ## copy input file for each job and set random seed
  cp "$outdir/input" "$out/."
  sed -i 's/rsd 1/rsd '$I'/' "$out/input"

  ## test analysis (abort after first job)
  if [[ $TEST == "1" ]] ; then

      ./urqmd.sh $out $NEVENT     # LOCAL run
      break;

  else
      #### gsi batch environement for kronos (slurm) ####
      #### NOTE: possible partitions: sinfo -o "%9P  %6g %10l %5w %5D %13C %N"
      ####                      and : scontrol show partition main
      #### long: upto 7d (CPUs=3880)
      #### main upto 8h (CPUs=7840)
      #### debug: upto 20m (CPUs=200)
      #### memory max 4096M, default 2048M

      ## configure batch job
      tmpjobscriptname=$PWD/sr_$(basename $outdir)_$I.sh
      ####
      echo "#! /bin/sh"                                 > $tmpjobscriptname
      echo "#SBATCH --output=$out/urqmd.slurm.out"    >> $tmpjobscriptname
      echo "#SBATCH --error=$out/urqmd.slurm.err"     >> $tmpjobscriptname
      echo "#SBATCH --job-name=sr_${I}"                 >> $tmpjobscriptname
      echo "#SBATCH --mail-type=END"                    >> $tmpjobscriptname
      echo "#SBATCH --mail-user=j.book"                 >> $tmpjobscriptname
      echo "#SBATCH --mem-per-cpu=4000"                 >> $tmpjobscriptname
      echo "#SBATCH --time=1-05:00:00"                  >> $tmpjobscriptname
      echo "#SBATCH --partition=long"                   >> $tmpjobscriptname
      ####
      echo "srun ./urqmd.sh $out $NEVENT"                   >> $tmpjobscriptname

      chmod u+x $tmpjobscriptname

      ## submit job
      sbatch $tmpjobscriptname
  fi

  let I=$I+1

done

