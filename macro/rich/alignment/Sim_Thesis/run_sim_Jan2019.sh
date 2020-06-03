#!/bin/sh

#Macro will be running via script
export SCRIPT=yes

#Build directory of CBMROOT
export MY_BUILD_DIR=/data/ROOT6/build

export TRACKINGTYPE=branch

#Simulation parameters
#--------------------------------------------------
# number of embedded electrons
export NELECTRONS=5

# number of embedded positrons
export NPOSITRONS=5

# If "yes" than UrQMD will be used as background
export URQMD=yes

# If "yes" than primary electrons will be generated
export ELECTRONS=yes

# If "yes" PLUTO particles will be embedded
export PLUTO=no

# Inputs
#export IN_FILE=/data/ROOT6/trunk/input/urqmd/urqmd.auau.10gev.centr.00011.root

#number of events for each thread
export NEVENTS=1000

#for Z in 16; do
for Z in 01 02 03 04 05 06 07 08; do
#for Z in 09 10 11 12 13 14 15 16; do

	export COR=Uncorrected
	export COR1=Uncorrected
	export IN_FILE=/data/ROOT6/trunk/input/urqmd/urqmd.auau.10gev.centr.000${Z}.root

#	export SETUP_NAME=setup_align
#	export SETUP_NAME=setup_misalign_gauss_sigma_1
#	export SETUP_NAME=setup_misalign_gauss_sigma_3
	export SETUP_NAME=setup_misalign_gauss_sigma_2
        export GEO_SETUP_FILE=/macro/rich/alignment/misalignment_correction/Sim_Thesis/geosetup/${SETUP_NAME}.C

#	export DIR=/data/Sim_Outputs/Sim_Thesis/AuAu_10AGeV_Jan2019/Aligned
#	export DIR=/data/Sim_Outputs/Sim_Thesis/AuAu_10AGeV_Jan2019/1mrad
#	export DIR=/data/Sim_Outputs/Sim_Thesis/AuAu_10AGeV_Jan2019/3mrad
	export DIR=/data/Sim_Outputs/Sim_Thesis/AuAu_10AGeV_Jan2019/2mrad
	export DIR2=${DIR}/${COR1}/${Z}

	export MC_FILE=${DIR}/${COR}/mc.${Z}.root
	export PAR_FILE=$DIR/${COR}/param.${Z}.root
	export RECO_FILE=$DIR/${COR1}/reco.${Z}.root
	export GEOSIM_FILE=$DIR/${COR}/geosim.${Z}.root
        export LIT_RESULT_DIR=${DIR}/${COR}/${Z}/
	        mkdir -p ${LIT_RESULT_DIR}

   xterm -fa 'Monospace' -fs 11 -hold -e ". ./sim_Jan2019.sh"&
# bsub -q batch -J mc.$Z.$XXXX.run -o $DIR/log/$XXXX.log -N sh ./sim.sh

done
export SCRIPT=no
