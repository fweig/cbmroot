#!/bin/bash

## test train or batch submission
TEST=1

## number of events: default=1000
NEVENT=1000

## number of sub jobs: default=10
NJOBS=10

## number of runs
NRUNS=2000

## storage element and directory
LOCATION=/lustre/nyx
DIR=sim

## test parameters
if [[ $TEST == "1" ]] ; then
    DIR="TEST${DIR}"
    NEVENT=2
    NJOBS=1
    NRUNS=1
fi


## output directory for simulated and reconstructed data
outdir=$LOCATION/cbm/users/$USER/$DIR/simFast_AA_UrQMD_eeCocktail_centr010_JUN16_25mum_4lay_wMVD
mkdir -p "$outdir"

## source cbm root environment
source $LOCATION/cbm/users/$USER/CBMsoft/cbm-env.sh -n

## choose CBM setup for simulation and reconstruction
export SETUP="sis100_electron"      #"sis100_muon_jpsi"

## copy cbm setup macro and simulation and reconstruction macros to output directory
## for backup and documentation
cp -nv "$VMCWORKDIR/macro/run/.rootrc"                           "$outdir/."
cp -nv "$VMCWORKDIR/geometry/setup/setup_${SETUP}.C"             "$outdir/."
cp -nv "./run_sim_fast.C"                                        "$outdir/."
cp -nv "./g3Config.C"                                            "$outdir/."
cp -nv "./SetCuts.C"                                             "$outdir/."

## loop over all runs
I=0
while [ "$I" -lt "$NRUNS" ]; do

  ## create output directory for job $I
  out=$outdir/`printf "%05d" $I`
  mkdir -p "$out"

  ## test analysis (abort after first job)
  if [[ $TEST == "1" ]] ; then

      ./simrecoFast.sh $out $NEVENT $NJOBS     # LOCAL run
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

      ## partition
      resources="--mem=4000 --time=0-08:00:00"
      partition="--partition=main"

      scriptdir="$PWD"
      workdir="--workdir ${out}"

      mail_type="--mail-type=FAIL"
      mail_user="--mail-user=j.book"

      job_name="--job-name=sF_${I}"
      log_output="--output=${out}/simfast.slurm.out"
      log_error="--error=${out}/simfast.slurm.err"

      ## submit job
      command="${partition} ${resources} ${workdir} ${log_output} ${log_error} ${mail_type} ${mail_user} ${job_name} ${scriptdir}/simrecoFast.sh $out $NEVENT $NJOBS"
      sbatch $command

  fi

  let I=$I+1

done

