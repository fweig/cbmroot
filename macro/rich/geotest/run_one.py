#!/usr/bin/env python3

import os   
import sys 
import shutil
import argparse

def main():
  runId = sys.argv[1]
  testType = sys.argv[2] # geotest or urqmdtest

  #runId = "0"
  geoSetup = "sis100_electron"
  nEvents = 3000 if testType == "geotest" else 100
  targetZ = -44.0
  energy = "10gev"

  cbmrootConfigPath = "/Users/slebedev/Development/cbm/git/build/config.sh"
  urqmdFile = ("/Users/slebedev/Development/cbm/data/urqmd/auau/{}/mbias/urqmd.auau.{}.mbias.00001.root").format(energy, energy)
  if testType == "geotest":
    urqmdFile = ""

  plutoFile = ""
  dataDir = "/Users/slebedev/Development/cbm/data/sim/rich/" + testType
  resultDir = "results_" + testType + "_" + runId + "_"+ energy +"/" 
  

  traFile = dataDir + "/tra." + runId + ".root"  
  parFile = dataDir + "/par." + runId + ".root"
  digiFile = dataDir + "/digi." + runId + ".root"
  recoFile = dataDir + "/reco." + runId + ".root"
  qaFile = dataDir + "/qa." + runId + ".root"
  geoSimFile = dataDir + "/geosim." + runId + ".root"

  traCmd = ('root -l -b -q run_transport.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{},{}\)').format(urqmdFile, plutoFile, traFile, parFile, geoSimFile, geoSetup, nEvents, targetZ)
  digiCmd = ('root -l -b -q run_digi.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(traFile, parFile, digiFile, nEvents)
  recoCmd = ('root -l -b -q run_reco.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(testType, traFile, parFile, digiFile, recoFile, geoSetup, nEvents)
  qaCmd = ('root -l -b -q run_qa.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(testType, traFile, parFile, digiFile, recoFile, qaFile, geoSetup, resultDir, nEvents)

  os.system((". /{} -a; {}").format(cbmrootConfigPath, traCmd))
  os.system((". /{} -a; {}").format(cbmrootConfigPath, digiCmd))
  os.system((". /{} -a; {}").format(cbmrootConfigPath, recoCmd))
  os.system((". /{} -a; {}").format(cbmrootConfigPath, qaCmd))

# def make_args():
#   parser = argparse.ArgumentParser()
#   parser.add_argument('--urqmdFile', help='urqmdFile')
#   return parser.parse_args()
  
if __name__ == '__main__':
  # import sys
  # args = make_args()
  # main(make_args())
  main()
