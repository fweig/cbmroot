#!/bin/sh
# Copyright (C) 2018 Justus-Liebig-Universitaet Giessen, Giessen
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Gregor Pitsch


#inmed, qgp, rho0, omegaepem, omegadalitz, phi

nofJobs=200
#dataDir=/lustre/nyx/cbm/users/gpitsch/CbmRoot/data/lmvm/auautest/
dataDir=/lustre/nyx/cbm/users/gpitsch/CbmRoot/data/lmvm/agag/august18_2kk_field60/
#dataDir=/lustre/nyx/cbm/users/gpitsch/CbmRoot/data/lmvm/august18_1kk_field50/
#set full path to geo setup macro
geoSetupMacroPath=/lustre/nyx/cbm/users/gpitsch/CbmRoot/trunk/macro/analysis/dielectron/geosetup/diel_setup_sis100_field60.C
collEnergy=4.5gev

#for plutoParticle in inmed; do
for plutoParticle in inmed omegaepem omegadalitz phi; do
if [ ${plutoParticle} = "inmed" ] ; then
nofJobs=400
else
nofJobs=200
fi
outdir=${dataDir}/${collEnergy}/${plutoParticle}
logFile=${outdir}/log/log_slurm-%A_%a.out
errorFile=${outdir}/error/error_slurm-%A_%a.out
jobName=${collEnergy}_${plutoParticle}
#directories must be created by user
mkdir -p $(dirname "${logFile}")
mkdir -p $(dirname "${errorFile}")

sbatch --job-name=${jobName} --time=8:00:00 --workdir=${outdir} --output=${logFile} --error=${errorFile} --array=1-${nofJobs} sim_agag.sh ${outdir} ${collEnergy} ${plutoParticle} ${geoSetupMacroPath}

done
