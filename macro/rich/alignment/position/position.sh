#!/bin/bash

XXXX=$(printf "%04d" "$SLURM_ARRAY_TASK_ID")
YYYY=$(printf "%08d" "$SLURM_ARRAY_JOB_ID")
##XXXX=$4

cbmroot_config_path=/lustre/nyx/cbm/users/jbendar/CBMINSTALL_Root5/bin/CbmRootConfig.sh
#output_dir=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position
output_dir=/lustre/nyx/cbm/users/jbendar/Sim_Outputs/Ring_Track_VS_Position/Batch_4
macro_dir=/lustre/nyx/cbm/users/jbendar/CBMINSTALL_Root5/share/cbmroot/macro/rich

# Specify input and output directories
if [ $1 -eq 0 ] ; then
        outdir=${output_dir}/Aligned/
elif [ $1 -eq 1 ] ; then
	outdir=${output_dir}/Misaligned_1mrad_Full/
elif [ $1 -eq 2 ] ; then
        outdir=${output_dir}/Misaligned_2mrad_Full/
elif [ $1 -eq 30 ] ; then
        outdir=${output_dir}/Misaligned_3mrad_XY_Full/
elif [ $1 -eq 31 ] ; then
        outdir=${output_dir}/Misaligned_3mrad_minX_Full/
elif [ $1 -eq 32 ] ; then
        outdir=${output_dir}/Misaligned_3mrad_minY_Full/
elif [ $1 -eq 33 ] ; then
        outdir=${output_dir}/Misaligned_3mrad_minXY_Full/
elif [ $1 -eq 4 ] ; then
	outdir=${output_dir}/Misaligned_4mrad_Full/
elif [ $1 -eq 5 ] ; then
        outdir=${output_dir}/Misaligned_5mrad_Full/
elif [ $1 -eq 6 ] ; then
        outdir=${output_dir}/Misaligned_OFF/
fi
export OUT_DIR=${outdir}

# Needed to run macro via script
export SCRIPT=yes

# Create needed directories
mkdir -p ${outdir}/log

# Create directory for output results
mkdir -p ${outdir}/results/${YYYY}_${XXXX}/
cd ${outdir}/results/${YYYY}_${XXXX}/

# Setup the run environment
source ${cbmroot_config_path}
echo ${VMCWORKDIR}

# This line is needed, otherwise root will crash
export DISPLAY=localhost:0.0

# Geometry setup macro
if [ $1 -eq 0 ] ; then
        setupMacro=setup_align.C
	setupName=setup_align
elif [ $1 -eq 1 ] ; then
	setupMacro=setup_misalign_1mrad.C
	setupName=setup_misalign_1mrad
elif [ $1 -eq 2 ] ; then
        setupMacro=setup_misalign_2mrad.C
	setupName=setup_misalign_2mrad
elif [ $1 -eq 30 ] ; then
        setupMacro=setup_misalign_3mrad_XY.C
	setupName=setup_misalign_3mrad_XY
elif [ $1 -eq 31 ] ; then
        setupMacro=setup_misalign_3mrad_minX.C
	setupName=setup_misalign_3mrad_minX
elif [ $1 -eq 32 ] ; then
        setupMacro=setup_misalign_3mrad_minY.C
	setupName=setup_misalign_3mrad_minY
elif [ $1 -eq 33 ] ; then
        setupMacro=setup_misalign_3mrad_minXY.C
	setupName=setup_misalign_3mrad_minXY
elif [ $1 -eq 4 ] ; then
	setupMacro=setup_misalign_4mrad.C
	setupName=setup_misalign_4mrad
elif [ $1 -eq 5 ] ; then
        setupMacro=setup_misalign_5mrad.C
	setupName=setup_misalign_5mrad
elif [ $1 -eq 6 ] ; then
        setupMacro=setup_misalign_OFF.C
	setupName=setup_misalign_OFF
fi
export GEO_SETUP_FILE=${setupMacro}
export SETUP_NAME=${setupName}
export GEO_FOLDER=1

# Define urqmd and output files
export URQMD_FILE=/lustre/nyx/cbm/prod/gen/urqmd/auau/${3}/centr/urqmd.auau.${3}.centr.${YYYY}_${XXXX}.root
export MC_FILE=${outdir}/${setupName}_mc.${YYYY}_${XXXX}.root
export PAR_FILE=${outdir}/${setupName}_params.${YYYY}_${XXXX}.root
export RECO_FILE=${outdir}/${setupName}_reco.${YYYY}_${XXXX}.root
export ANALYSIS_FILE=${outdir}/${setupName}_analysis.${YYYY}_${XXXX}.root
export LIT_RESULT_DIR=${outdir}/${YYYY}_${XXXX}

#Simulation parameters
#--------------------------------------------------
# set generation of primary electrons
export ELECTRONS=yes
# number of embedded electrons
export NOF_ELECTRONS=1
export NELECTRONS=10
# number of embedded positrons
export NOF_POSITRONS=1
export NPOSITRONS=10
# If "yes" then UrQMD will be used as background
export URQMD=no
# If "yes" PLUTO particles will be embedded
export PLUTO=no
# Collision energy: 25gev or 8gev -> set proper weight into analysis
export ENERGY=${3}

# If "yes" DELTA electrons will be embedded
#export DELTA=no
#export DELTA_FILE=/lustre/cbm/user/ebelolap/aug11/sep12/${ENERGY}/${FIELDDIR}/deltasource/mc.delta.root

# Create special and enter special workdir for job
# username=$(whoami)
# workdir=/tmp/$username/$JOB_ID.$SGE_TASK_ID
# mkdir -p $workdir
# cd $workdir

## echo ${output_dir}
## echo ${macro_dir}
## echo ${outdir}
echo ${setupMacro}
echo ${setupName}

# Run the root simulation
root -b -l -q "${macro_dir}/position/run_sim_position4.C(${2})"
root -b -l -q "${macro_dir}/position/run_reco_position4.C(${2})"
# root -b -l -q "${macro_dir}/position/Compute_distance.C(${2}, ${1})"

# cp -v ${SGE_STDOUT_PATH} ${outdir}/log/${JOB_ID}.${SGE_TASK_ID}.log

export SCRIPT=no
