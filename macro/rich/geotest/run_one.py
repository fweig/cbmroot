#!/usr/bin/env python3

import os   
import sys 
import shutil
import argparse

def main():
  runId = sys.argv[1]
  energy = sys.argv[2]

  cbmrootConfigPath = "/Users/slebedev/Development/cbm/git/build/config.sh"
  urqmdFile = ("/Users/slebedev/Development/cbm/data/urqmd/auau/{}/mbias/urqmd.auau.{}.mbias.00001.root").format(energy, energy)
  plutoFile = ""
  dataDir = "/Users/slebedev/Development/cbm/data/sim/rich/geotest/"
  resultDir = "results_" + runId + "_"+ energy +"/" 
  
  #runId = "0"
  geoSetup = "sis100_electron"
  nEvents = 1000
  testType = "urqmdTest" # urqmdTest or geoTest

  traFile = dataDir + "/tra." + runId + ".root"  
  parFile = dataDir + "/param." + runId + ".root"
  digiFile = dataDir + "/digi." + runId + ".root"
  recoFile = dataDir + "/reco." + runId + ".root"
  qaFile = dataDir + "/qa." + runId + ".root"
  geoSimFile = dataDir + "/geosim." + runId + ".root"

  traCmd = ('root -l -b -q run_transport.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(urqmdFile, plutoFile, traFile, parFile, geoSimFile, geoSetup, nEvents)
  digiCmd = ('root -l -b -q run_digi.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(traFile, parFile, digiFile, nEvents)
  recoCmd = ('root -l -b -q run_reco.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(testType, traFile, parFile, digiFile, recoFile, geoSetup, nEvents)
  qaCmd = ('root -l -b -q run_qa.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(testType, traFile, parFile, digiFile, recoFile, qaFile, geoSetup, resultDir, nEvents)

  # os.system((". /{} -a; {}").format(cbmrootConfigPath, traCmd))
  # os.system((". /{} -a; {}").format(cbmrootConfigPath, digiCmd))
  # os.system((". /{} -a; {}").format(cbmrootConfigPath, recoCmd))
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
