#!/bin/sh

# get parameters of the script
outdir=${1}
collEnergy=${2}
plutoParticle=${3}
geoSetupMacroPath=${4}
#plutoEnergy=8gev
#cbmrootConfigPath=/lustre/nyx/cbm/users/slebedev/cbm/trunk/build/config.sh
#macro_dir=/lustre/nyx/cbm/users/slebedev/cbm/trunk/cbmroot/macro/analysis/dielectron/

cbmrootConfigPath=/lustre/nyx/cbm/users/gpitsch/CbmRoot/trunk/build/config.sh
macro_dir=/lustre/nyx/cbm/users/gpitsch/CbmRoot/trunk/macro/analysis/dielectron/

XXXXX=$(printf "%05u" "$SLURM_ARRAY_TASK_ID")
XXXX=$(printf "%04u" "$SLURM_ARRAY_TASK_ID")

# Needed to run macro via script
export SCRIPT=yes

# Number of events to run
nevents=10000

# setup the run environment
source ${cbmrootConfigPath}
 
# This line is needed, otherwise root will crash
export DISPLAY=localhost:0.0

#export URQMD_FILE=/lustre/nyx/cbm/prod/gen/urqmd/auau/${collEnergy}/centr/urqmd.auau.${collEnergy}.centr.${XXXXX}.root

#auau simulation
if [ ${plutoParticle} = "omegaepem" ] ; then
   mcFilesDir="omega_epem"
   YYYYY=${XXXXX}
elif [ ${plutoParticle} = "omegadalitz" ] ; then
   mcFilesDir="omega_pi0epem"
   YYYYY=$(printf "%05u" $(($SLURM_ARRAY_TASK_ID + 200)))
elif [ ${plutoParticle} = "phi" ] ; then
   mcFilesDir="phi_epem"
   YYYYY=$(printf "%05u" $(($SLURM_ARRAY_TASK_ID + 400)))
elif [ ${plutoParticle} = "inmed" ] ; then
   mcFilesDir="inmed_epem"
      YYYYY=$(printf "%05u" $(($SLURM_ARRAY_TASK_ID + 600)))
fi   
#export MC_FILE=${outdir}/mc.auau.${collEnergy}.centr.${XXXXX}.root
#export PAR_FILE=${outdir}/params.auau.${collEnergy}.centr.${XXXXX}.root
#export RECO_FILE=${outdir}/reco.auau.${collEnergy}.centr.${XXXXX}.root
#export LITQA_FILE=${outdir}/litqa.auau.${collEnergy}.centr.${XXXXX}.root
#export ANALYSIS_FILE=${outdir}/analysis.auau.${collEnergy}.centr.${XXXXX}.root

#agag analysis


export MC_FILE=/lustre/nyx/cbm/prod/mc/r13109/${mcFilesDir}/sis100_electron.${YYYYY}.tra.root
export PAR_FILE=/lustre/nyx/cbm/users/gpitsch/CbmRoot/data/lmvm/testUhlig/${mcFilesDir}/sis100_electron.${YYYYY}.par.root
export RECO_FILE=${outdir}/reco.agag.${collEnergy}.mbias.${XXXXX}.root
export LITQA_FILE=${outdir}/litqa.agag.${collEnergy}.mbias.${XXXXX}.root
export ANALYSIS_FILE=${outdir}/analysis.agag.${collEnergy}.mbias.${XXXXX}.root

echo $SLURM_ARRAY_TASK_ID
echo $MC_FILE
echo $PAR_FILE
echo $RECO_FILE
echo $LITQA_FILE
echo $ANALYSIS_FILE

export RESULT_DIR=

#Simulation parameters
#--------------------------------------------------
# number of embedded electrons
export NOF_ELECTRONS=0
# number of embedded positrons
export NOF_POSITRONS=0
# If "yes" than UrQMD will be used as background
export URQMD=yes
# If "yes" PLUTO particles will be embedded
export PLUTO=yes
#Collision energy: set proper weight into analysis
export ENERGY=${collEnergy}
#export ENERGY=3.5gev #for agag analysis as long no multiplicities available

#Geometry setup macro
export GEO_SETUP_FILE=${geoSetupMacroPath}

export SETUP_FUNCT="do_setup()"

export PLUTO_PARTICLE=${plutoParticle}
export PLUTO=yes

if [ ${plutoParticle} = "rho0" ] ; then 
   export PLUTO_FILE=/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/${collEnergy}/rho0/epem/pluto.auau.${collEnergy}.rho0.epem.${XXXX}.root
elif [ ${plutoParticle} = "omegaepem" ] ; then 
   export PLUTO_FILE=/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/${collEnergy}/omega/epem/pluto.auau.${collEnergy}.omega.epem.${XXXX}.root
elif [ ${plutoParticle} = "omegadalitz" ] ; then 
   export PLUTO_FILE=/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/${collEnergy}/omega/pi0epem/pluto.auau.${collEnergy}.omega.pi0epem.${XXXX}.root
elif [ ${plutoParticle} = "phi" ] ; then 
   export PLUTO_FILE=/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/${collEnergy}/phi/epem/pluto.auau.${collEnergy}.phi.epem.${XXXX}.root
elif [ ${plutoParticle} = "pi0" ] ; then 
   export PLUTO_FILE=/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/${collEnergy}/pi0/gepem/pluto.auau.${collEnergy}.pi0.gepem.${XXXX}.root
elif [ ${plutoParticle} = "inmed" ] ; then 
   export PLUTO_FILE=/lustre/nyx/cbm/prod/gen/pluto/auau/cktRapp/${collEnergy}/rapp.inmed/epem/pluto.auau.${collEnergy}.rapp.inmed.epem.${XXXX}.root
elif [ ${plutoParticle} = "qgp" ] ; then 
   export PLUTO_FILE=/lustre/nyx/cbm/prod/gen/pluto/auau/cktRapp/${collEnergy}/rapp.qgp/epem/pluto.auau.${collEnergy}.rapp.qgp.epem.${XXXX}.root
elif [ ${plutoParticle} = "urqmd" ] ; then 
   export PLUTO=no
fi 

# run the root simulation
#root -b -l -q "${macro_dir}/run_sim.C(${nevents})"
#root -b -l -q "${macro_dir}/run_reco.C(${nevents})"
root -b -l -q "${macro_dir}/run_litqa.C(${nevents})"
root -b -l -q "${macro_dir}/run_analysis.C(${nevents})"

export SCRIPT=no
