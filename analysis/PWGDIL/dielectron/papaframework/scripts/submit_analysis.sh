#!/bin/bash

# indir need to be simulation directory with a subdirectory structure that contains the individual simulation runs (typically numbers)
indir=/home/$USER/sim/test/
outname="test"   # the output will be written in this directory under this directory name
CONFIG=1           # this one can be used to switch between data naming and configurations ; it is parsed to the next level script and changes the run_script that is called
train="test"     # inside the output directory is a another directory for the specific analysis

# CONFIG 1 - default dilepton testing config - sim names: tra.root/raw.root/rec.root
# CONFIG 2 - default dilepton testing config for common productions - sim names: 1.tra.root/1.event.raw.root/1.reco.root, etc

out=$PWD/$outname/$train
## test train or batch submissio1
TEST=1           # TEST 1 - calculates a testrun of one file locally ; TEST 0 - submits jobs to virgo
GROUP=10         # Number of files that is processed per job
DEBUG=0          # Single small job (50 events) that is send to the DEBUG partition to test that everything runs smoothly
useDEB=0         # For a very small number of very small jobs ; they can be processed on the DEBUG partition in rare cases

## number of events: 0=all
NEVENT=0
STOPJOB=20000
MINJOB=0

if [[ $TEST == "1" ]] ; then
    train="test"
fi

## storage element and directory
LOCATION=/lustre/nyx
DIR=sim

split_level=1;

## test parameters
if [[ $TEST == "1" ]] ; then
    train=${train/train/test};
    train="test"
    GROUP=1;
    split_level=1;
    NEVENT=0
fi

mkdir -p $out

## get CBM setup used in simulation and reconstruction
SETUP=$(basename $indir/setup* .C)
SETUP=${SETUP/setup_/}

## copy user config and analysis macro to train output directory
## for backup and documentation
if [[ $CONFIG == "1" ]] ; then
    rm -v "$out/run_testing.C"
    cp -v "$PWD/run_testing.C" "$out/."
    cp -v "$PWD/Config_dilepton_testing.C"     "$out/."
fi
if [[ $CONFIG == "2" ]] ; then
    rm -v "$out/run_common_analysis.C"
    cp -v "$PWD/run_common_analysis.C" "$out/."
    cp -v "$PWD/Config_dilepton_testing.C"     "$out/."
fi

## get run list and sort it
#if grep -Fq "." runList.txt; then
if grep -Fq "." never.txt; then
    echo "sorted run list already there, only split the list";
    sort runList.txt > runListSort.txt
else
    ## only collect file with validated reconstruction output
    if [[ $TEST == "1" ]] ; then
	#find $indir -type f -name "run_transport_${SETUP}_ok" -printf "%h\n" | head -n $split_level > runList.txt;
	if [[ $CONFIG == "1" ]] ; then
	    if [[ $GROUP == "1" ]] ; then
		find $indir -type f -name "rec.root" -size +100M -printf "%h\n" | head -n $split_level > runList.txt;
	    else
		find $indir -type f -name "rec.root" -size +100M -exec dirname {} \; > runList.txt;
	    fi
	fi
	if [[ $CONFIG == "2" ]] ; then
	    if [[ $GROUP == "1" ]] ; then
 		find $indir -type f -name "*.rec.root" -size +100M -printf "%h\n" | head -n $split_level > runList.txt;
	    else
		find $indir -type f -name "*.rec.root" -size +100M -exec dirname {} \; > runList.txt;
	    fi
	fi
    else
	if [[ $CONFIG == "1" ]] ; then
	    find $indir -type f -name "rec.root" -size +100M -exec dirname {} \; > runList.txt;
	fi
	if [[ $CONFIG == "2" ]] ; then
	    find $indir -type f -name "*.rec.root" -size +100M -exec dirname {} \; > runList.txt;
	fi
    fi
    echo "";
    echo "";
    echo "";
    echo "sorting";
    echo "";
    echo "";
    echo "";
    sort runList.txt > runListSort.txt
fi

## split file list by split_level into many files (1 list per job)
split -a 4 -l $split_level runListSort.txt filelist_${train}_
ListOfFilelist=`ls filelist_*`

# loop over all lists
I=0
N=1
COUNT=0

out1=""
file1=""
out2=""
file2=""
out3=""
file3=""


#    echo $ListOfFilelist

for filelist in $ListOfFilelist ; do
    if [[ "$I" -lt "$MINJOB" ]] ; then
	let I=$I+1
	continue
    fi
    
    ## create output directory for job $I
    outdir=$out/`printf "%05d/" $I`
    mkdir -p $outdir    
    
    if [[ "$GROUP" -gt "1" ]] ; then	  
	outlist[$COUNT]=$outdir
	file[$COUNT]=$filelist
	let COUNT=$COUNT+1
    fi

    ## test analysis (abort after first job)
    if [[ $TEST == "1" ]] ; then
	# clean up
	if [[ $GROUP == "1" ]] ; then
	    ./analysis.sh $indir $filelist $outdir $NEVENT $CONFIG $TEST $I    # LOCAL run
	    rm $(ls filelist_${train}_* | grep -v aaaa)
	    rm -v runListSort.txt;

	    break
	else
	    if [[ $N == "$GROUP" ]] ; then
		echo ${outlist[*]}
		echo ${file[*]}
		./ana.sh $indir $NEVENT $CONFIG $GROUP $TEST $I ${file[*]} ${outlist[*]}     # LOCAL run
		outdir=""
		file=""
		COUNT=0
		N=0
		rm -v runListSort.txt;
		break
	    fi
	fi
    else
	#### gsi batch environement for kronos (slurm) ####
	#### NOTE: possible partitions: sinfo -o "%9P  %6g %10l %5w %5D %13C %N"
	####                      and : scontrol show partition main
	#### long: upto 7d (CPUs=3880)
	#### main upto 8h (CPUs=7840)
	#### debug: upto 20m (CPUs=200)
	#### memory max 4096M, default 2048M

	## configure batch job
	## copy filelist
	mv $filelist $outdir/.

	## partition
	resources="--mem=4000 --time=0-08:00:00"
	partition="--partition=main"

	if [[ $DEBUG == "1" ]] ; then      
	    partition="--partition=debug"
	    NEVENT=50
	    resources="--mem=4000 --time=0-00:20:00"
	fi

	if [[ $useDEB == "1" ]] ; then      
	    partition="--partition=debug"
	    resources="--mem=4000 --time=0-00:20:00"
	fi
	
	scriptdir="$PWD"
	workdir="--workdir ${outdir}"
	workdir="--chdir ${outdir}"
	
	job_name="--job-name=${I}"
	log_output="--output=${outdir}/ana.slurm.out"
	log_error="--error=${outdir}/ana.slurm.err"

	## submit job
	if [[ $GROUP == "1" ]] ; then
	    command="${partition} ${resources} ${workdir} ${log_output} ${log_error} ${job_name} -- ${scriptdir}/analysis.sh $indir $filelist $outdir $NEVENT $CONFIG $TEST $I"
	    sbatch $command
	    if [[ $DEBUG == "1" ]] ; then
		break
	    fi
	else
	    if [[ $N == "$GROUP" ]] ; then
		command="${partition} ${resources} ${workdir} ${log_output} ${log_error} ${job_name} -- ${scriptdir}/ana.sh $indir $NEVENT $CONFIG $GROUP $TEST $I ${file[*]} ${outlist[*]} "
		echo $command
		sbatch $command
		outdir=""
		file=""
		COUNT=0
		N=0

		if [[ $I == $STOPJOB ]] ; then
	    	    break;
		fi

		if [[ $DEBUG == "1" ]] ; then
		    break
		fi
	    fi
	fi
    fi

    if [[ $I == $STOPJOB ]] ; then
	break;
    fi

    
    let I=$I+1
    let N=$N+1
done
