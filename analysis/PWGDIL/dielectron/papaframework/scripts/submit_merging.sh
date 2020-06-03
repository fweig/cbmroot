#!/bin/bash

## split level: defines number of files per jobs
split_level=10;

## split step: iteration
splitstep=1

## unique train id
train="train0001"

## storage element and directory
LOCATION=/lustre/nyx
DIR=sim

## output directory of analyzed data
indir=$LOCATION/cbm/users/$USER/$DIR/sim_AA_UrQMD_eeCocktail_centr010_JUN16_25mum_4lay_wMVD/$train

## output directory
outdir=$indir/merge
mkdir -p $outdir

## get run list and sort it
if [ -e $outdir/runList_$splitstep.txt ]; then
    echo "run list already there, only split the list";
elif [ $splitstep == "1" ]; then
    # first iteration
    find $indir/. -type f -size +100k -name "sis100_*analysis.root" > "$outdir/runList_$splitstep.txt";
else
    # iterations > 1
    laststep=$(expr $splitstep - 1)
    find $outdir/. -type f -size +100k -name "merged_filelist_temp_$laststep*.root" > "$outdir/runList_$splitstep.txt";
fi

## clean up
rm $outdir/filelist_temp_*
rm "$outdir/merged_filelist_temp_${splitstep}*.root"
rm "$outdir/merge${splitstep}_*.slurm.*"

## split file list by split_level into many files (1 list per job)
split -a 3 -l $split_level "$outdir/runList_$splitstep.txt" "$outdir/filelist_temp_$splitstep"
ListOfFilelist=`ls $outdir/filelist_temp_*`


# loop over all lists
I=0
for filelist in $ListOfFilelist ; do
  let I=$I+1

      #### gsi batch environement for kronos (slurm) ####
      #### NOTE: possible partitions: sinfo -o "%9P  %6g %10l %5w %5D %13C %N"
      ####                      and : scontrol show partition main
      #### long: upto 7d (CPUs=3880)
      #### main upto 8h (CPUs=7840)
      #### debug: upto 20m (CPUs=200)
      #### memory max 4096M, default 2048M

      ## configure batch job
      tmpjobscriptname=$PWD/merge_${train}_${splitstep}_$I.sh
      ####
      echo "#! /bin/sh"                                   > $tmpjobscriptname
      echo "#SBATCH --output=$outdir/merge${splitstep}_${I}.slurm.out" >> $tmpjobscriptname
      echo "#SBATCH --error=$outdir/merge${splitstep}_${I}.slurm.err"  >> $tmpjobscriptname
      echo "#SBATCH --job-name=m${filelist: -4}_${train}"              >> $tmpjobscriptname
      echo "#SBATCH --mail-type=FAIL"                     >> $tmpjobscriptname
      echo "#SBATCH --mail-user=j.book@gsi.de"            >> $tmpjobscriptname
      echo "#SBATCH --mem-per-cpu=2000"                   >> $tmpjobscriptname
      echo "#SBATCH --time=0-00:20:00"                    >> $tmpjobscriptname
      echo "#SBATCH --partition=debug"                    >> $tmpjobscriptname
      ####
      echo "srun ./merging.sh $filelist"  >> $tmpjobscriptname

      chmod u+x $tmpjobscriptname

      ## submit job
      sbatch $tmpjobscriptname


done
