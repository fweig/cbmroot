#!/usr/bin/env python3

import os
import sys

taskId = sys.argv[1]
cbmrootConfigPath = "/Users/slebedev/Development/cbm/trunk/build/config.sh"
macroDir = "/Users/slebedev/Development/cbm/trunk/cbmroot/macro/rich/"
nofEvents = 10

os.system(("source {}").format(cbmrootConfigPath))

urqmdFile = "/Users/slebedev/Development/cbm/data/urqmd/auau/10gev/centr/urqmd.auau.10gev.centr.00001.root"
dataDir= "/Users/slebedev/Development/cbm/data/sim/rich/reco/"
mcFile = dataDir + "/mc."+ taskId + ".root"
parFile = dataDir + "/param."+ taskId + ".root"
digiFile = dataDir + "/digi."+ taskId + ".root"
recoFile = dataDir + "/reco."+ taskId + ".root"
qaFile = dataDir + "/qa."+ taskId + ".root"
geoSimFile = dataDir + "/geosim."+ taskId + ".root"
geoSetup = "rich_pipe_v" + taskId
resultDir = "results_rich_pipe/results_" + geoSetup + "/recoqa/"


#geotest
#commandSim=('root -l -b -q {}/geotest/run_sim_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, geoSimFile, geoSetup, nofEvents)
#commandDigi=('root -l -b -q {}/geotest/run_digi_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, nofEvents)
#commandReco=('root -l -b -q {}/geotest/run_reco_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, geoSetup, resultDir, nofEvents)
#commandQa=('root -l -b -q {}/geotest/run_qa_geotest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, qaFile, geoSetup, resultDir, nofEvents)

#urqmdtest
# commandSim=('root -l -b -q {}/geotest/run_sim_urqmdtest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, urqmdFile, mcFile, parFile, geoSimFile, geoSetup, nofEvents)
# commandDigi=('root -l -b -q {}/geotest/run_digi_urqmdtest.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, nofEvents)
# commandReco=('root -l -b -q {}/geotest/run_reco_urqmdtest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, geoSetup, resultDir, nofEvents)
# commandQa=('root -l -b -q {}/geotest/run_qa_urqmdtest.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, qaFile, geoSetup, resultDir, nofEvents)

#recoqa
# commandSim=('root -l -b -q {}/run/run_sim.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{},{},\\"{}\\",\\"{}\\",{}\)').format(macroDir, urqmdFile, mcFile, parFile, geoSimFile, 5, 5, "", geoSetup, nofEvents)
# commandDigi=('root -l -b -q {}/run/run_digi.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, nofEvents)
# commandReco=('root -l -b -q {}/run/run_reco.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, geoSetup, resultDir, nofEvents)
commandQa=('root -l -b -q {}/run/run_qa.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, qaFile, geoSetup, resultDir, nofEvents)

# os.system(commandSim)
# os.system(commandDigi)
# os.system(commandReco)
os.system(commandQa)
