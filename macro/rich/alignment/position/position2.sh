#!/bin/bash

XXXXX=$(printf "%05d" "$SLURM_ARRAY_TASK_ID")
FLAG=1
NEVTS=10
COLL_ENERGY=8gev

# Specify input and output directories
if [ $FLAG -eq 0 ] ; then
	outdir=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Misaligned
elif [ $FLAG -eq 1 ] ; then
	outdir=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Aligned
fi

echo 'outdir: ' ${outdir}

cbmroot_config_path=/lustre/nyx/cbm/users/jbendar/CBMINSTALL/bin/CbmRootConfig.sh
macro_dir=/u/jbendar/CBMSRC/macro/rich/alignment/misalignment_correction/position

# Needed to run macro via script
export SCRIPT=yes

# Create needed directories
mkdir -p ${outdir}/log

# Create directory for output results
mkdir -p ${outdir}/results/${XXXXX}/

# Setup the run environment
source ${cbmroot_config_path}

# This line is needed, otherwise root will crash
export DISPLAY=localhost:0.0

# Define urqmd and output files
export URQMD_FILE=/lustre/nyx/cbm/prod/gen/urqmd/auau/${COLL_ENERGY}/centr/urqmd.auau.${COLL_ENERGY}.centr.${XXXXX}.root
export MC_FILE=${outdir}/mc.root
export PAR_FILE=${outdir}/params.root
export RECO_FILE=${outdir}/reco.root
export ANALYSIS_FILE=${outdir}/analysis.root
export RESULT_DIR=${outdir}

#Simulation parameters
#--------------------------------------------------
# number of embedded electrons
export NOF_ELECTRONS=1
# number of embedded positrons
export NOF_POSITRONS=1
# If "yes" then UrQMD will be used as background
export URQMD=no
# If "yes" PLUTO particles will be embedded
export PLUTO=no
# Collision energy: 25gev or 8gev -> set proper weight into analysis
export ENERGY=${COLL_ENERGY}

# Geometry setup macro
#export GEO_SETUP_FILE=${setupMacro}

# If "yes" DELTA electrons will be embedded
#export DELTA=no
#export DELTA_FILE=/lustre/cbm/user/ebelolap/aug11/sep12/${ENERGY}/${FIELDDIR}/deltasource/mc.delta.root

# Create special and enter special workdir for job
# username=$(whoami)
# workdir=/tmp/$username/$JOB_ID.$SGE_TASK_ID
# mkdir -p $workdir
# cd $workdir

export OUT_DIR=${outdir}
# Run the root simulation
root -b -l -q "${macro_dir}/run_sim_position.C(${NEVTS}, ${FLAG})"
root -b -l -q "${macro_dir}/run_reco_position.C(${NEVTS}, ${FLAG})"

# cp -v ${SGE_STDOUT_PATH} ${outdir}/log/${JOB_ID}.${SGE_TASK_ID}.log

export SCRIPT=no
