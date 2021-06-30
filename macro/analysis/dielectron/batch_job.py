#!/usr/bin/env python3

import os     #operating system interfaces
import sys    #provides access to some variables used or maintained by the interpreter and to functions that interact strongly with the interpreter
import shutil #offers a number of high level operations on files and collections of files

def main():
  dataDir = sys.argv[1]
  geoSetup = sys.argv[2]
  plutoParticle = sys.argv[3]
  cbmrootConfigPath = "/lustre/nyx/cbm/users/criesen/build/config.sh"
  macroDir = "/lustre/nyx/cbm/users/criesen/cbmroot/macro/analysis/dielectron/"
  nofEvents = 100

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

  #plutoFile = ""
  plutoFile = getPlutoPath(colSystem, colEnergy, plutoParticle, taskId) #this one was activated before
  #plutoFile = getPlutoPath("auau", "8gev", plutoParticle, taskId)

  urqmdFile = "/lustre/nyx/cbm/prod/gen/urqmd/auau/" + colEnergy + "/centr/urqmd.auau.8gev.centr." + str(taskId).zfill(5) + ".root"
  #urqmdFile = "/lustre/nyx/cbm/prod/gen/urqmd/auau/8gev/centr/urqmd.auau.8gev.centr.00001.root"

  mcFile = dataDir + "/mc." + taskId + ".root"
  parFile = dataDir + "/param." + taskId + ".root"
  digiFile = dataDir + "/digi." + taskId + ".root"
  recoFile = dataDir + "/reco." + taskId + ".root"
  litQaFile = dataDir + "/litqa." + taskId + ".root"
  analysisFile = dataDir + "/analysis." + taskId + ".root"
  geoSimFile = dataDir + "/geosim." + taskId + ".root"
  print("Here: in batchJobPy")
  #os.system(('root -l -b -q {}/run_sim.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, urqmdFile, plutoFile, mcFile, parFile, geoSimFile, geoSetup, nofEvents))
  #os.system(('root -l -b -q {}/run_digi.C\(\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, nofEvents))
  #os.system(('root -l -b -q {}/run_reco.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, geoSetup, nofEvents))
  #os.system(('root -l -b -q {}/run_litqa.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, litQaFile, geoSetup, nofEvents))
  os.system(('root -l -b -q {}/run_analysis.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, analysisFile, plutoParticle, colSystem, colEnergy, geoSetup, nofEvents))

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
