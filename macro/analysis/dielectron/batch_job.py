#!/usr/bin/env python3

import os
import sys
import shutil


def main():
    dataDir = sys.argv[1]
    geoSetup = sys.argv[2]
    plutoParticle = sys.argv[3]
    cbmrootConfigPath = "/lustre/nyx/cbm/users/slebedev/cbm/trunk/build/config.sh"
    macroDir = "/lustre/nyx/cbm/users/slebedev/cbm/trunk/cbmroot/macro/analysis/dielectron/"
    nofEvents = 1000
    taskId = os.environ.get('SLURM_ARRAY_TASK_ID')
    jobId = os.environ.get('SLURM_ARRAY_JOB_ID')

    print("dataDir:" + dataDir)

    os.system(("source {}").format(cbmrootConfigPath))

    workDir = dataDir + "/workdir/" + jobId + "_" + taskId + "/"
    if os.path.exists(workDir):
        shutil.rmtree(workDir)
        os.makedirs(workDir)
        os.chdir(workDir)

    plutoFile = getPlutoPath("auau", "8gev", plutoParticle, taskId)
    urqmdFile = "/lustre/nyx/cbm/prod/gen/urqmd/auau/8gev/centr/urqmd.auau.8gev.centr.00001.root"
    plutoFile = "/lustre/nyx/cbm/prod/gen/pluto/erik/cktA/8gev/omega/epem/pluto.auau.8gev.omega.epem.0001.root"
    mcFile = dataDir + "/mc." + taskId + ".root"
    parFile = dataDir + "/param." + taskId + ".root"
    digiFile = dataDir + "/digi." + taskId + ".root"
    recoFile = dataDir + "/reco." + taskId + ".root"
    litQaFile = dataDir + "/litqa." + taskId + ".root"
    analysisFile = dataDir + "/analysis." + taskId + ".root"
    geoSimFile = dataDir + "/geosim." + taskId + ".root"
    os.system(('root -l -b -q {}/run/run_sim.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{},{},\\"{}\\",\\"{}\\",{}\)').format(
        macroDir, urqmdFile, mcFile, parFile, geoSimFile, nofElectrons, nofPositrons, plutoFile, geoSetup, nofEvents))
    os.system(('root -l -b -q {}/run/run_digi.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(
        macroDir, mcFile, parFile, digiFile, nofEvents))
    os.system(('root -l -b -q {}/run/run_reco.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(
        macroDir, mcFile, parFile, digiFile, recoFile, geoSetup, resultDir, nofEvents))
    os.system(('root -l -b -q {}/run/run_qa.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(
        macroDir, mcFile, parFile, digiFile, recoFile, qaFile, geoSetup, resultDir, nofEvents))


def getPlutoPath(colSystem, colEnergy, plutoParticle, index):
    if plutoParticle == "rho0":
        return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/${collEnergy}/rho0/epem/pluto.auau.${collEnergy}.rho0.epem.${XXXX}.root"
    elif plutoParticle == "omegaepem":
        return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/${collEnergy}/omega/epem/pluto.auau.${collEnergy}.omega.epem.${XXXX}.root"
    elif plutoParticle == "omegadalitz":
        return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/${collEnergy}/omega/pi0epem/pluto.auau.${collEnergy}.omega.pi0epem.${XXXX}.root"
    elif plutoParticle == "phi":
        return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/${collEnergy}/phi/epem/pluto.auau.${collEnergy}.phi.epem.${XXXX}.root"
    elif plutoParticle == "pi0":
        return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/${collEnergy}/pi0/gepem/pluto.auau.${collEnergy}.pi0.gepem.${XXXX}.root"
    elif plutoParticle == "inmed":
        return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktRapp/${collEnergy}/rapp.inmed/epem/pluto.auau.${collEnergy}.rapp.inmed.epem.${XXXX}.root"
    elif plutoParticle == "qgp":
        return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktRapp/${collEnergy}/rapp.qgp/epem/pluto.auau.${collEnergy}.rapp.qgp.epem.${XXXX}.root"
    elif plutoParticle == "urqmd":
        return ""


if __name__ == '__main__':
    main()
