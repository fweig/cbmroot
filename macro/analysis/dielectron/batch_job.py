#!/usr/bin/env python3

import os     
import sys    
import shutil

def main():
  dataDir = sys.argv[1]
  geoSetup = sys.argv[2]
  plutoParticle = sys.argv[3]
  cbmrootConfigPath = "/lustre/nyx/cbm/users/criesen/build/config.sh"
  macroDir = "/lustre/nyx/cbm/users/criesen/cbmroot/macro/analysis/dielectron/"
  nofEvents = 1000

  taskId = os.environ.get('SLURM_ARRAY_TASK_ID')
  jobId = os.environ.get('SLURM_ARRAY_JOB_ID')

  colEnergy = "8gev"
  colSystem = "auau"

  print("dataDir:" + dataDir)
  os.system(("source {}").format(cbmrootConfigPath))

  workDir = dataDir + "/workdir/" + jobId + "_" + taskId + "/"
  if os.path.exists(workDir):
    shutil.rmtree(workDir)
  os.makedirs(workDir)
  os.chdir(workDir)

  plutoFile = getPlutoPath(colSystem, colEnergy, plutoParticle, taskId)

  urqmdFile = "/lustre/nyx/cbm/prod/gen/urqmd/auau/" + colEnergy + "/centr/urqmd.auau."+ colEnergy + ".centr." + str(taskId).zfill(5) + ".root"

  traFile = dataDir + "/tra." + taskId + ".root"
  parFile = dataDir + "/param." + taskId + ".root"
  digiFile = dataDir + "/digi." + taskId + ".root"
  recoFile = dataDir + "/reco." + taskId + ".root"
  litQaFile = dataDir + "/litqa." + taskId + ".root"
  analysisFile = dataDir + "/analysis." + taskId + ".root"
  geoSimFile = dataDir + "/geosim." + taskId + ".root"

  traCmd = ('root -l -b -q {}/run_transport.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, urqmdFile, plutoFile, traFile, parFile, geoSimFile, geoSetup, nofEvents)
  digiCmd = ('root -l -b -q {}/run_digi.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, traFile, parFile, digiFile, nofEvents)
  recoCmd = ('root -l -b -q {}/run_reco.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, traFile, parFile, digiFile, recoFile, geoSetup, nofEvents)
  qaCmd = ('root -l -b -q {}/run_litqa.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, traFile, parFile, digiFile, recoFile, litQaFile, geoSetup, nofEvents)
  anaCmd = ('root -l -b -q {}/run_analysis.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, traFile, parFile, digiFile, recoFile, analysisFile, plutoParticle, colSystem, colEnergy, geoSetup, nofEvents)

  #os.system((". /{} -a; {}").format(cbmrootConfigPath, traCmd))
  #os.system((". /{} -a; {}").format(cbmrootConfigPath, digiCmd))
  #os.system((". /{} -a; {}").format(cbmrootConfigPath, recoCmd))
  #os.system((". /{} -a; {}").format(cbmrootConfigPath, qaCmd))
  os.system((". /{} -a; {}").format(cbmrootConfigPath, anaCmd))

def getPlutoPath(colSystem, colEnergy, plutoParticle, taskId):
  if plutoParticle == "rho0":
    return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/" + colEnergy + "/rho0/epem/pluto.auau." + colEnergy + ".rho0.epem." + str(taskId).zfill(4) + ".root"
  elif plutoParticle == "omegaepem":
    return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/" + colEnergy + "/omega/epem/pluto.auau." + colEnergy + ".omega.epem." + str(taskId).zfill(4) + ".root"
  elif plutoParticle == "omegadalitz":
    return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/" + colEnergy + "/omega/pi0epem/pluto.auau." + colEnergy + ".omega.pi0epem." + str(taskId).zfill(4) + ".root"
  elif plutoParticle == "phi":
    return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/" + colEnergy + "/phi/epem/pluto.auau." + colEnergy + ".phi.epem." + str(taskId).zfill(4) + ".root"
  elif plutoParticle == "pi0":
    return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/" + colEnergy + "/pi0/gepem/pluto.auau." + colEnergy + ".pi0.gepem." + str(taskId).zfill(4) + ".root"
  elif plutoParticle == "inmed":
    return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktRapp/" + colEnergy + "/rapp.inmed/epem/pluto.auau." + colEnergy + ".rapp.inmed.epem." + str(taskId).zfill(4) + ".root"
  elif plutoParticle == "qgp":
    return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktRapp/" + colEnergy + "/rapp.qgp/epem/pluto.auau." + colEnergy + ".rapp.qgp.epem." + str(taskId).zfill(4) + ".root"
  elif plutoParticle == "urqmd":
    return ""

if __name__ == '__main__':
  main()
