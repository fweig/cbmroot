#!/usr/bin/env python3

import os
import sys
import shutil

dataDir = sys.argv[1]
mirrorRotation = sys.argv[2]
cbmrootConfigPath = "/lustre/nyx/cbm/users/slebedev/cbm/trunk/build/config.sh"
macroDir = "/lustre/nyx/cbm/users/slebedev/cbm/trunk/cbmroot/macro/rich/"
taskId = os.environ.get('SLURM_ARRAY_TASK_ID')
jobId = os.environ.get('SLURM_ARRAY_JOB_ID')

print("dataDir:" + dataDir)

os.system(("source {}").format(cbmrootConfigPath))

geoSetup = "mirror" + str(mirrorRotation) + "_" + taskId

workDir = dataDir + "/workdir/" + jobId + "_" + taskId + "/"
if os.path.exists(workDir):
    shutil.rmtree(workDir)
os.makedirs(workDir)
os.chdir(workDir)

runGeotest = False
runUrqmdtest = False
runRecoqaUrqmd = True
runRecoqaBoxGen = True
runGeotestPlutoOmega3_5gev = False
runGeotestPlutoOmega8gev = False

if runGeotest:
    nofEvents = 30000
    plutoFile = ""
    dataDirNew = dataDir + "/geotest_box/"
    if not os.path.exists(dataDirNew): os.makedirs(dataDirNew)
    mcFile = dataDirNew + "/mc."+ taskId + ".root"
    parFile = dataDirNew + "/param."+ taskId + ".root"
    digiFile = dataDirNew + "/digi."+ taskId + ".root"
    recoFile = dataDirNew + "/reco."+ taskId + ".root"
    qaFile = dataDirNew + "/qa."+ taskId + ".root"
    geoSimFile = dataDirNew + "/geosim."+ taskId + ".root"
    resultDir = dataDirNew + "/results/results_" + geoSetup + "/"
    os.system( ('root -l -b -q {}/geotest/run_sim_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, plutoFile, mcFile, parFile, geoSimFile, geoSetup, nofEvents) )
    os.system( ('root -l -b -q {}/geotest/run_digi_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, nofEvents) )
    os.system( ('root -l -b -q {}/geotest/run_reco_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, geoSetup, resultDir, nofEvents) )
    os.system( ('root -l -b -q {}/geotest/run_qa_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, qaFile, geoSetup, resultDir, nofEvents) )

if runUrqmdtest:
    nofEvents = 1000
    dataDirNew = dataDir + "/urqmdtest_8gev/"
    if not os.path.exists(dataDirNew): os.makedirs(dataDirNew)
    urqmdFile = "/lustre/nyx/cbm/prod/gen/urqmd/auau/8gev/centr/urqmd.auau.8gev.centr.00001.root"
    mcFile = dataDirNew + "/mc."+ taskId + ".root"
    parFile = dataDirNew + "/param."+ taskId + ".root"
    digiFile = dataDirNew + "/digi."+ taskId + ".root"
    recoFile = dataDirNew + "/reco."+ taskId + ".root"
    qaFile = dataDirNew + "/qa."+ taskId + ".root"
    geoSimFile = dataDirNew + "/geosim."+ taskId + ".root"
    resultDir = dataDirNew + "/results/results_" + geoSetup + "/"
    os.system( ('root -l -b -q {}/geotest/run_sim_urqmdtest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, urqmdFile, mcFile, parFile, geoSimFile, geoSetup, nofEvents) )
    os.system( ('root -l -b -q {}/geotest/run_digi_urqmdtest.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, nofEvents) )
    os.system( ('root -l -b -q {}/geotest/run_reco_urqmdtest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, geoSetup, resultDir, nofEvents) )
    os.system( ('root -l -b -q {}/geotest/run_qa_urqmdtest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, qaFile, geoSetup, resultDir, nofEvents) )
  
if runRecoqaUrqmd:
    nofEvents = 1000
    dataDirNew = dataDir + "/recoqa_urqmd8gev/"
    if not os.path.exists(dataDirNew): os.makedirs(dataDirNew)
    urqmdFile = "/lustre/nyx/cbm/prod/gen/urqmd/auau/8gev/centr/urqmd.auau.8gev.centr.00001.root"
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
    # os.system( ('root -l -b -q {}/run/run_sim.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{},{},\\"{}\\",\\"{}\\",{}\)').format(macroDir, urqmdFile, mcFile, parFile, geoSimFile, nofElectrons, nofPositrons, plutoFile, geoSetup, nofEvents) )
    # os.system( ('root -l -b -q {}/run/run_digi.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, nofEvents) )
    # os.system( ('root -l -b -q {}/run/run_reco.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, geoSetup, resultDir, nofEvents) )
    os.system( ('root -l -b -q {}/run/run_qa.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, qaFile, geoSetup, resultDir, nofEvents) )

if runRecoqaBoxGen:
    nofEvents = 5000
    urqmdFile = ""
    dataDirNew = dataDir + "/recoqa_box30/"
    if not os.path.exists(dataDirNew): os.makedirs(dataDirNew)
    mcFile = dataDirNew + "/mc."+ taskId + ".root"
    parFile = dataDirNew + "/param."+ taskId + ".root"
    digiFile = dataDirNew + "/digi."+ taskId + ".root"
    recoFile = dataDirNew + "/reco."+ taskId + ".root"
    qaFile = dataDirNew + "/qa."+ taskId + ".root"
    geoSimFile = dataDirNew + "/geosim."+ taskId + ".root"
    resultDir = dataDirNew + "/results/results_" + geoSetup + "/"
    nofElectrons = 15
    nofPositrons = 15
    plutoFile = ""
    # os.system( ('root -l -b -q {}/run/run_sim.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{},{},\\"{}\\",\\"{}\\",{}\)').format(macroDir, urqmdFile, mcFile, parFile, geoSimFile, nofElectrons, nofPositrons, plutoFile, geoSetup, nofEvents) )
    # os.system( ('root -l -b -q {}/run/run_digi.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, nofEvents) )
    # os.system( ('root -l -b -q {}/run/run_reco.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, geoSetup, resultDir, nofEvents) )
    os.system( ('root -l -b -q {}/run/run_qa.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, qaFile, geoSetup, resultDir, nofEvents) )


if runGeotestPlutoOmega3_5gev:
    nofEvents = 1000
    dataDirNew = dataDir + "/geotest_plutoOmega3_5gev/"
    if not os.path.exists(dataDirNew): os.makedirs(dataDirNew)
    plutoFile = "/lustre/nyx/cbm/prod/gen/pluto/erik/cktA/3.5gev/omega/epem/pluto.auau.3.5gev.omega.epem.0001.root"
    mcFile = dataDirNew + "/mc."+ taskId + ".root"
    parFile = dataDirNew + "/param."+ taskId + ".root"
    digiFile = dataDirNew + "/digi."+ taskId + ".root"
    recoFile = dataDirNew + "/reco."+ taskId + ".root"
    qaFile = dataDirNew + "/qa."+ taskId + ".root"
    geoSimFile = dataDirNew + "/geosim."+ taskId + ".root"
    resultDir = dataDirNew + "/results/results_" + geoSetup + "/"
    os.system( ('root -l -b -q {}/geotest/run_sim_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, plutoFile, mcFile, parFile, geoSimFile, geoSetup, nofEvents) )
    os.system( ('root -l -b -q {}/geotest/run_digi_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, nofEvents) )
    os.system( ('root -l -b -q {}/geotest/run_reco_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, geoSetup, resultDir, nofEvents) )
    os.system( ('root -l -b -q {}/geotest/run_qa_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, qaFile, geoSetup, resultDir, nofEvents) )


if runGeotestPlutoOmega8gev:
    nofEvents = 1000
    dataDirNew = dataDir + "/geotest_plutoOmega8gev/"
    if not os.path.exists(dataDirNew): os.makedirs(dataDirNew)
    plutoFile = "/lustre/nyx/cbm/prod/gen/pluto/erik/cktA/8gev/omega/epem/pluto.auau.8gev.omega.epem.0001.root"
    mcFile = dataDirNew + "/mc."+ taskId + ".root"
    parFile = dataDirNew + "/param."+ taskId + ".root"
    digiFile = dataDirNew + "/digi."+ taskId + ".root"
    recoFile = dataDirNew + "/reco."+ taskId + ".root"
    qaFile = dataDirNew + "/qa."+ taskId + ".root"
    geoSimFile = dataDirNew + "/geosim."+ taskId + ".root"
    resultDir = dataDirNew + "/results/results_" + geoSetup + "/"
    os.system( ('root -l -b -q {}/geotest/run_sim_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, plutoFile, mcFile, parFile, geoSimFile, geoSetup, nofEvents) )
    os.system( ('root -l -b -q {}/geotest/run_digi_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, nofEvents) )
    os.system( ('root -l -b -q {}/geotest/run_reco_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, geoSetup, resultDir, nofEvents) )
    os.system( ('root -l -b -q {}/geotest/run_qa_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, qaFile, geoSetup, resultDir, nofEvents) )
