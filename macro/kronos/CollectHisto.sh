#!/bin/bash

GENERATOR=urqmd
SYSTEM=pni
ENERGY=15gev
CENTRALITY=centr
SETUP=sis100_electron

export MAINDIR=`pwd`

SCRIPT_DIR=$MAINDIR/histos
export CBMROOT_DIR=/lustre/nyx/cbm/users/$USER/cbmroot/build

MAINDIR=`pwd`

. $CBMROOT_DIR/config.sh
export DISPLAY=localhost:0.0

export INDIR="/lustre/nyx/cbm/users/$USER/mc/"$GENERATOR"_"$SYSTEM"_"$ENERGY"_"$CENTRALITY"_"$SETUP

OUTDIR="/lustre/nyx/cbm/users/$USER/Results/"$GENERATOR"_"$SYSTEM"_"$ENERGY"_"$CENTRALITY"_"$SETUP
mkdir -p $OUTDIR
cd $OUTDIR

cp $SCRIPT_DIR/CalculateEfficiency.C .
root -l -b -q "CalculateEfficiency.C(\"$INDIR\")" &> Efficiency.txt &


export OUTTMP=$OUTDIR/TmpHisto
rm -rf $OUTTMP
mkdir -p $OUTTMP

NLINES=1
for i in `seq 1 3`;
do
  mkdir -p $OUTTMP/log$i
  echo $SCRIPT_DIR/hadd_batch$i.sh

  sbatch -D $OUTTMP/log$i --export=ALL $SCRIPT_DIR/hadd_batch$i.sh

  NTASKS="$(squeue -u $USER | wc -l)"
  while [[ "$NTASKS" > "$NLINES" ]]; do
    echo $NTASKS
    NTASKS="$(squeue -u $USER | wc -l)"

    sleep 5s
  done
done 

rm -rf KFParticleFinder.root
hadd KFParticleFinder.root $OUTTMP/analysis.temp-all3.*.root

cd $MAINDIR
