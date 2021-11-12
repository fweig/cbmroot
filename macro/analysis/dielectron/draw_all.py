#!/usr/bin/env python3

import os, shutil

def main():
  plutoParticles = ["inmed", "omegadalitz", "omegaepem", "phi", "qgp"]
  dataDir = "/lustre/nyx/cbm/users/criesen/data/lmvm/"
  dataDirOut = dataDir + "/results/"
  macroDir = "/lustre/cbm/users/criesen/cbmroot/macro/analysis/dielectron/"
  useMvd = True
  drawSig = True
  
  if os.path.exists(dataDirOut):
    shutil.rmtree(dataDirOut)
  os.mkdir(dataDirOut)

  
  for plutoParticle in plutoParticles:
    resultDir = dataDirOut + plutoParticle
    resultDirAna = resultDir + "/lmvm/"
    inRootAna = dataDir + plutoParticle + "/analysis.all.root"
    os.system(('root -l -b -q {}/draw_analysis.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\"\)').format(macroDir, inRootAna, resultDirAna, useMvd, drawSig))
    
    resultDirLitqa = resultDir + "/litqa/"
    inRootLitqa = dataDir + plutoParticle + "/litqa.all.root"
    os.system(('root -l -b -q {}/draw_litqa.C\(\\"{}\\",\\"{}\\"\)').format(macroDir, inRootLitqa, resultDirLitqa))
    

  allInmed  = dataDir + "/inmed/analysis.all.root"
  allQgp = dataDir + "/qgp/analysis.all.root"
  allOmega = dataDir + "/omegaepem/analysis.all.root"
  allPhi = dataDir + "/phi/analysis.all.root"
  allOmegaD = dataDir + "/omegadalitz/analysis.all.root"
  resultDirAll = dataDirOut + "/lmvm_all/"
  os.system(('root -l -b -q {}/draw_analysis_all.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\"\)').format(macroDir, allInmed, allQgp, allOmega, allPhi, allOmegaD, resultDirAll, useMvd))

if __name__ == '__main__':
  main()
