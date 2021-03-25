#!/bin/bash

## split level: defines number of files per jobs
split_level=10;
MERGEIT=$1

TEST=0

## storage element and directory
LOCATION=/lustre/nyx
DIR=sim

dirs="
$PWD/test/test/
"

echo $dirs

if [[ $MERGEIT == "1" ]] ; then
    for direc in $dirs ; do
	echo ""
	echo "$direc/merge"
	cd "$direc/merge"
	hadd merged.root merged_*
	rm merge1_*
	rm filelist_temp_1a*
	rm slurm-*
	rm merged_*
	rm -v ../merge.C
	cd "-"
    done
    exit 0
fi

if [[ $MERGEIT == "2" ]] ; then
    for direc in $dirs ; do
	echo ""
	echo "$direc/merge"
	cd "$direc/merge"
	hadd merged.root merged_*
	rm merge1_*
	rm filelist_temp_1a*
	rm merged_*
	rm -rv ../0*
	rm -v ../merge.C
	cd "-"
    done
    exit 0
fi

for direc in $dirs ; do
    indir=$direc

    cp -v merge.C $indir
    ## output directory
    outdir=$indir/merge
    rm -rv $outdir
    mkdir -p $outdir

    echo "- indir $indir"
    echo "- outdir $outdir"

    ## get run list and sort it
    if [ -e $outdir/runList_$splitstep.txt ]; then
	echo "run list already there, only split the list";
    elif [ $splitstep == "1" ]; then
	# first iteration
	echo "- check -"
	find $indir/. -type f -name "analysis.root" > "$outdir/runList_$splitstep.txt";
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

	if [[ $TEST == "1" ]] ; then
	    ./merging.sh $filelist
	    break
	fi
	partition="--partition=debug"
	resources="--mem=4000 --time=0-00:20:00"
	scriptdir="$PWD"
	workdir="--chdir ${outdir}"
	
	job_name="--job-name=${I}"
	log_output="--output=${outdir}/ana.slurm.out"
	log_error="--error=${outdir}/ana.slurm.err"

	command="${partition} ${resources} ${workdir} ${job_name} -- ${scriptdir}/merging.sh $filelist"
	sbatch $command

	
    done
    rm ${PWD}/merge_${train}_*
done

