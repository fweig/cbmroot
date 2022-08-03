#!/usr/bin/env python3

import os
import sys

runId = sys.argv[1]
cbmrootConfigPath = "/Users/slebedev/Development/cbm/git/build/config.sh"
macroDir = "/Users/slebedev/Development/cbm/git/cbmroot/macro/rich/"
nofEvents = 100

urqmdFile = "/Users/slebedev/Development/cbm/data/urqmd/auau/10gev/centr/urqmd.auau.10gev.centr.00001.root"
dataDir= "/Users/slebedev/Development/cbm/data/sim/rich/reco/"
traFile = dataDir + "/tra."+ runId + ".root"
parFile = dataDir + "/par."+ runId + ".root"
digiFile = dataDir + "/digi."+ runId + ".root"
recoFile = dataDir + "/reco."+ runId + ".root"
qaFile = dataDir + "/qa."+ runId + ".root"
geoSimFile = dataDir + "/geosim."+ runId + ".root"

# geoSetup = "sis100_electron"
geoSetup = "sis100_electron_" + runId
resultDir = "results_recoqa_" + runId + "/"
targetZ = -44.0

traCmd=('root -l -b -q {}/run/run_transport.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{},{},\\"{}\\",\\"{}\\",{},{}\)').format(macroDir, urqmdFile, traFile, parFile, geoSimFile, 5, 5, "", geoSetup, nofEvents, targetZ)
digiCmd=('root -l -b -q {}/run/run_digi.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, traFile, parFile, digiFile, nofEvents)
recoCmd=('root -l -b -q {}/run/run_reco.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, traFile, parFile, digiFile, recoFile, geoSetup, resultDir, nofEvents)
qaCmd=('root -l -b -q {}/run/run_qa.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, traFile, parFile, digiFile, recoFile, qaFile, geoSetup, resultDir, nofEvents)

os.system((". /{} -a; {}").format(cbmrootConfigPath, traCmd))
os.system((". /{} -a; {}").format(cbmrootConfigPath, digiCmd))
os.system((". /{} -a; {}").format(cbmrootConfigPath, recoCmd))
os.system((". /{} -a; {}").format(cbmrootConfigPath, qaCmd))
