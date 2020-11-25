#!/usr/bin/env python3

import os, shutil

def main():
    plutoParticles = ["inmed", "phi", "omegaepem", "omegadalitz", "qgp"]
    dataDir = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/"
    dataDirHistos = dataDir + "histograms/"
    macroDir = "/lustre/cbm/users/criesen/cbmroot/macro/analysis/dielectron"
    
    if os.path.exists(dataDirHistos):
        shutil.rmtree(dataDirHistos)
    os.mkdir(dataDirHistos)
    
    for plutoParticle in plutoParticles:
        
        outFile = dataDirHistos + plutoParticle

        inFilesAna = dataDir + plutoParticle + "/analysis.all.root"
        uMvd = False
        drawSig = True
        os.system(('root -l -b -q {}/draw_analysis.C\(\\"{}\\",\\"{}\\"\)').format(macroDir, inFilesAna, outFile, uMvd, drawSig))

        inFilesLitqa = dataDir + plutoParticle + "/litqa.all.root"
        os.system(('root -l -b -q {}/draw_litqa.C\(\\"{}\\",\\"{}\\"\)').format(macroDir, inFilesLitqa, outFile))
    
 
    print("======================================================================================= DRAW ALL ==================================================================================")
    os.system(('root -l -b -q {}/draw_analysis_all.C').format(macroDir))    # default values of macro will be taken (see there)
    
if __name__ == '__main__':
    main()
