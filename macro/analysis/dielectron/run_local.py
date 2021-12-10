#!/usr/bin/env python3

import os   
import sys 
import shutil

def main():

  cbmrootConfigPath = "/home/aghoehne/soft/cbm/build/config.sh"
  urqmdFile = "/home/aghoehne/soft/cbm/data/input/urqmd.auau.8gev.centr.00001.root"
  plutoFile = "/home/aghoehne/soft/cbm/data/input/pluto.auau.8gev.phi.epem.0001.root"
  dataDir = "/home/aghoehne/soft/cbm/data/output/phi"
  
  runId = "1"
  geoSetup = "sis100_electron"
  nEvents = 10
  colEnergy = "8gev"
  colSystem = "auau"
  plutoParticle = "phi"

  traFile = dataDir + "/tra." + runId + ".root"  
  parFile = dataDir + "/param." + runId + ".root"
  digiFile = dataDir + "/digi." + runId + ".root"
  recoFile = dataDir + "/reco." + runId + ".root"
  litQaFile = dataDir + "/litqa." + runId + ".root"
  analysisFile = dataDir + "/analysis." + runId + ".root"
  geoSimFile = dataDir + "/geosim." + runId + ".root"

  traCmd = ('root -l -b -q run_transport.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(urqmdFile, plutoFile, traFile, parFile, geoSimFile, geoSetup, nEvents)
  digiCmd = ('root -l -b -q run_digi.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(traFile, parFile, digiFile, nEvents)
  recoCmd = ('root -l -b -q run_reco.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(traFile, parFile, digiFile, recoFile, geoSetup, nEvents)
  qaCmd = ('root -l -b -q run_litqa.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(traFile, parFile, digiFile, recoFile, litQaFile, geoSetup, nEvents)
  anaCmd = ('root -l -b -q run_analysis.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(traFile, parFile, digiFile, recoFile, analysisFile, plutoParticle, colSystem, colEnergy, geoSetup, nEvents)

  
  #os.system((". /{} -a; {}").format(cbmrootConfigPath, traCmd))
  #os.system((". /{} -a; {}").format(cbmrootConfigPath, digiCmd))
  #os.system((". /{} -a; {}").format(cbmrootConfigPath, recoCmd))
  os.system((". /{} -a; {}").format(cbmrootConfigPath, qaCmd))
  os.system((". /{} -a; {}").format(cbmrootConfigPath, anaCmd))
  
if __name__ == '__main__':
  main()
