#!/usr/bin/env python3

import os
import sys
import shutil

dataDir = sys.argv[1]
geoSetup = sys.argv[2]
cbmrootConfigPath = "/lustre/nyx/cbm/users/slebedev/cbm/trunk/build/config.sh"
macroDir = "/lustre/nyx/cbm/users/slebedev/cbm/trunk/cbmroot/macro/rich/"
taskId = os.environ.get('SLURM_ARRAY_TASK_ID')
jobId = os.environ.get('SLURM_ARRAY_JOB_ID')

print("dataDir:" + dataDir)

os.system(("source {}").format(cbmrootConfigPath))

workDir = dataDir + "/workdir/" + jobId + "_" + taskId + "/"
if os.path.exists(workDir):
    shutil.rmtree(workDir)
os.makedirs(workDir)
os.chdir(workDir)


runRecoqaUrqmd = True

if runRecoqaUrqmd:
    nofEvents = 1000
    dataDirNew = dataDir + "/recoqa_urqmd8gev/"
    if not os.path.exists(dataDirNew): os.makedirs(dataDirNew)
    urqmdFile = "/lustre/nyx/cbm/prod/gen/urqmd/auau/8gev/centr/urqmd.auau.8gev.centr." + str(taskId).zfill(5) + ".root"
    mcFile = dataDirNew + "/mc."+ taskId + ".root"
    parFile = dataDirNew + "/param."+ taskId + ".root"
    digiFile = dataDirNew + "/digi."+ taskId + ".root"
    recoFile = dataDirNew + "/reco."+ taskId + ".root"
    qaFile = dataDirNew + "/qa."+ taskId + ".root"
    geoSimFile = dataDirNew + "/geosim."+ taskId + ".root"
    resultDir = dataDirNew + "/results/results_" + geoSetup + "/"
    nofElectrons = 5
    nofPositrons = 5
    plutoFile = ""
    os.system( ('root -l -b -q {}/run/run_sim.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{},{},\\"{}\\",\\"{}\\",{}\)').format(macroDir, urqmdFile, mcFile, parFile, geoSimFile, nofElectrons, nofPositrons, plutoFile, geoSetup, nofEvents) )
    os.system( ('root -l -b -q {}/run/run_digi.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, nofEvents) )
    os.system( ('root -l -b -q {}/run/run_reco.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, geoSetup, resultDir, nofEvents) )
    os.system( ('root -l -b -q {}/run/run_qa.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, qaFile, geoSetup, resultDir, nofEvents) )
