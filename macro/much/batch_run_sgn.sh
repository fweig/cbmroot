#!/bin/bash
#SBATCH -J cbmroot
#SBATCH -D batch
#SBATCH -o %j.out
#SBATCH -e %j.err
#SBATCH --time=08:00:00
#SBATCH -p main
#SBATCH --array=0-999

cbmroot_dir=$1
 
cd ${cbmroot_dir}/build
. ./config.sh
export DISPLAY=localhost:0.0

DATASET="muons"
RATE=1
TIMESLICE=1
USEMC=kTRUE

GENERATOR=urqmd
PGENERATOR=pluto
SYSTEM=auau
ENERGY=8gev
CENTRALITY=centr
SETUP=sis100_muon_lmvm
NEVENTS=1000

index=$(($SLURM_ARRAY_TASK_ID + 1));

work_dir_run=${cbmroot_dir}/macro/run
work_dir_much=${cbmroot_dir}/macro/much

setup_dir=${work_dir_much}/data/$SETUP
if [ ! -d $setup_dir ]; then
    mkdir $setup_dir
fi
    
energy_dir=${setup_dir}/${ENERGY}
if [ ! -d $energy_dir ]; then
    mkdir $energy_dir
   fi
           
ONE=1
SLURM_INDEX=`expr $SLURM_ARRAY_TASK_ID + $ONE`
INDEX=$(printf "%05d" "$SLURM_INDEX")
INDEX2=$(printf "%04d" "$SLURM_INDEX")

INPUTFILE=/lustre/cbm/prod/gen/$GENERATOR/$SYSTEM/$ENERGY/$CENTRALITY/$GENERATOR.$SYSTEM.$ENERGY.$CENTRALITY.$INDEX.root;    

ARRAY1=(omega omega eta eta rho0 phi rapp.qgp)
ARRAY2=(omega omegaD etaD eta rho0 phi qgp)
ARRAY3=(mpmm pi0mpmm gmpmm mpmm mpmm mpmm)
ARRAY4=(cktA cktA cktA cktA cktA cktA cktRapp)

i=$2

PLUTOFILE=/lustre/cbm/prod/gen/$PGENERATOR/$SYSTEM/${ARRAY4[i]}/$ENERGY/${ARRAY1[i]}/${ARRAY3[i]}/$PGENERATOR.$SYSTEM.$ENERGY.${ARRAY1[i]}.${ARRAY3[i]}.$INDEX2.root

sgn_dir=${energy_dir}/${ARRAY2[i]}
if [ ! -d $sgn_dir ]; then
    mkdir $sgn_dir
    fi

run_dir=${sgn_dir}/${index}
if [ ! -d $run_dir ]; then
    mkdir $run_dir
    fi  
    
cd $run_dir 

cp -rf ${work_dir_much}/run_transport.C .
root -l -b -q "run_transport.C($NEVENTS,\"$SETUP\",\"$DATASET\",\"$INPUTFILE\",\"$PLUTOFILE\")"

cp -rf ${work_dir_run}/run_digi.C .
root -l -b -q "run_digi.C($NEVENTS,\"$DATASET\",$RATE,$TIMESLICE,kTRUE)"

cp -rf ${work_dir_run}/run_reco_event.C .
root -l -b -q "run_reco_event.C($NEVENTS,\"$DATASET\",\"$SETUP\",\"$USEMC\",kTRUE)"

cp -rf ${work_dir_much}/run_ana.C .
root -l -b -q "run_ana.C($NEVENTS,\"$DATASET\",\"$SETUP\",\"$USEMC\",\"$PLUTOFILE\")"

rm all_*
rm *C
rm TRhistos.root
rm L1*.root
rm Fair*.root
rm *dat
rm geo*.root
#rm *raw*.root
