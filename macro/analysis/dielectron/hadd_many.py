import os
import shutil

def main():
  plutoParticles =["inmed", "omegadalitz", "omegaepem", "phi", "qgp"]
  dataDir = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm"
  
  for plutoParticle in plutoParticles:
    dataDirPluto = dataDir + "/" + plutoParticle
    
    #inFilesLitQa = dataDirPluto + "/litqa.*.root"
    #outFileLitQa = dataDirPluto + "/litqa.all.root"
    #if os.path.exists(outFileLitQa):
    #  os.remove(outFileLitQa)
    #os.system(('hadd -j -T -f {} {}').format(outFileLitQa, inFilesLitQa))
    
    inFilesAna = dataDirPluto + "/analysis.*.root"
    outFileAna = dataDirPluto + "/analysis.all.root"
    if os.path.exists(outFileAna):
      os.remove(outFileAna)
    os.system(('hadd -j -T -f {} {}').format(outFileAna, inFilesAna))
    
if __name__ == '__main__':
  main()
