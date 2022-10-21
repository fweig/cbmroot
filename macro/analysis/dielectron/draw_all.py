#!/usr/bin/env python3

import os, shutil

def main():
  plutoParticles = ["inmed", "omegadalitz", "omegaepem", "phi", "qgp"]

  # paths on lustre
  macroDir = "/lustre/cbm/users/criesen/cbmroot/macro/analysis/dielectron/"
  dataDir  = "/lustre/cbm/users/criesen/data/lmvm/"

  # local paths
  #macroDir = "/home/aghoehne/soft/cbm/cbmroot/macro/analysis/dielectron/"
  #dataDir  = "/home/aghoehne/soft/cbm/data/output/"
  
  resultDir = dataDir + "results/"

  useMvd = False

  if os.path.exists(resultDir):
    shutil.rmtree(resultDir)
  os.mkdir(resultDir)
  
  for plutoParticle in plutoParticles:
    resultDirPP = resultDir + plutoParticle

    resultDirAna = resultDirPP + "/lmvm/"
    inRootAna = dataDir + plutoParticle + "/analysis.all.root"
    #os.system(('root -l -b -q {}/draw_analysis.C\(\\"{}\\",\\"{}\\",\\"{}\\"\)').format(macroDir, inRootAna, resultDirAna, useMvd))
    
    resultDirLitqa = resultDirPP + "/litqa/"
    inRootLitqa = dataDir + plutoParticle + "/litqa.all.root"
    #os.system(('root -l -b -q {}/draw_litqa.C\(\\"{}\\",\\"{}\\"\)').format(macroDir, inRootLitqa, resultDirLitqa))
    

  allInmed  = dataDir + "/inmed/analysis.all.root"
  allQgp    = dataDir + "/qgp/analysis.all.root"
  allOmega  = dataDir + "/omegaepem/analysis.all.root"
  allPhi    = dataDir + "/phi/analysis.all.root"
  allOmegaD = dataDir + "/omegadalitz/analysis.all.root"
  resultDirAll = resultDir + "/all/"

  os.system(('root -l -b -q {}/draw_analysis_all.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\"\)').format(macroDir, allInmed, allQgp, allOmega, allPhi, allOmegaD, resultDirAll, useMvd))

if __name__ == '__main__':
  main()
