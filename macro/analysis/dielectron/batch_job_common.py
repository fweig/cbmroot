#!/ usr / bin / env python3

import os #operating system interfaces import sys #provides access to some variables used or maintained by the interpreter and to functions that interact strongly with the interpreter import shutil #offers a number of high - level operations on files and collections of files


  def main() : recoDataDirIn = sys.argv[1] dataDirOut = sys.argv[2] geoSetup = sys.argv[3] plutoParticle = sys.argv[4]

                                                                                                                    cbmrootConfigPath = "/lustre/nyx/cbm/users/criesen/build/config.sh" macroDir = "/lustre/nyx/cbm/users/criesen/cbmroot/macro/analysis/dielectron/" nofEvents = 100

                                                                                                                    taskId = os.environ.get('SLURM_ARRAY_TASK_ID')

                                                                                                                                              jobId = os.environ.get('SLURM_ARRAY_JOB_ID') colEnergy = "12gev" colEnergyFile = "12agev" colSystem = "auau" print("recoDataDirIn:" + recoDataDirIn) print("dataDirOut:" + dataDirOut)

                                                                                                                                                                                                                                                                                                           os.system(("source {}").format(cbmrootConfigPath))

                                                                                                                                                                                                                                                                                                                        workDirOut = dataDirOut + "/workdir/" + jobId + "_" + taskId + "/" if os.path.exists(workDirOut) : shutil.rmtree(workDirOut) os.makedirs(workDirOut) os.chdir(workDirOut)

#offset describes the name of the folder with lowest number - 1 in dataIn directory
                                                                                                                                                                                                                                                                                                                                                                                                                                                                   if plutoParticle == "inmed_had_epem" : offset = 5000 offsetId = offset + int(taskId) if plutoParticle == "w" : offset = 0 offsetId = offset + int(taskId) if plutoParticle == "wdalitz" : offset = 1000 offsetId = offset + int(taskId) if plutoParticle == "phi" : offset = 3000 offsetId = offset + int(taskId) if plutoParticle == "qgp_epem" : offset = 7500 offsetId = offset + int(taskId)

#unlike others, paths of w and wdalitz input do not contain '/TGeant4/'
                                                                                                                                                                                                                                                                                                                                                                                                                                                                     if plutoParticle == "w" or plutoParticle == "wdalitz" : mcFile = recoDataDirIn + colSystem + "/" + colEnergyFile + "/centr_0_10/sis100_electron_target_25_mkm/" + str(offsetId) + "/" + str(offsetId) + ".event.tra.root" parFile = recoDataDirIn + colSystem + "/" + colEnergyFile + "/centr_0_10/sis100_electron_target_25_mkm/" + str(offsetId) + "/" + str(offsetId) + ".event.par.root" digiFile = recoDataDirIn + colSystem + "/" + colEnergyFile + "/centr_0_10/sis100_electron_target_25_mkm/" + str(offsetId) + "/" + str(offsetId) + ".event.raw.root" recoFile = recoDataDirIn + colSystem + "/" + colEnergyFile + "/centr_0_10/sis100_electron_target_25_mkm/" + str(offsetId) + "/" + str(offsetId) + ".event.rec.root" else : mcFile = recoDataDirIn + colSystem + "/" + colEnergyFile + "/centr_0_10/sis100_electron_target_25_mkm/TGeant4/" + str(offsetId) + "/" + str(offsetId) + ".event.tra.root" parFile = recoDataDirIn + colSystem + "/" + colEnergyFile + "/centr_0_10/sis100_electron_target_25_mkm/TGeant4/" + str(offsetId) + "/" + str(offsetId) + ".event.par.root" digiFile = recoDataDirIn + colSystem + "/" + colEnergyFile + "/centr_0_10/sis100_electron_target_25_mkm/TGeant4/" + str(offsetId) + "/" + str(offsetId) + ".event.raw.root" recoFile = recoDataDirIn + colSystem + "/" + colEnergyFile + "/centr_0_10/sis100_electron_target_25_mkm/TGeant4/" + str(offsetId) + "/" + str(offsetId) + ".event.rec.root"

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                litQaFile = dataDirOut + "/litqa." + taskId + ".root" analysisFile = dataDirOut + "/analysis." + taskId + ".root"
#geoSimFile = dataDir + "/geosim." + taskId + ".root"##needed ? ##

#it is assumed to take mc, digi, par and reco files from common folder, so only litqa and ana have to be run
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                os.system(('root -l -b -q {}/run_litqa.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, litQaFile, geoSetup, nofEvents)) os.system(('root -l -b -q {}/run_analysis.C\(\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",\\"{}\\",{}\)').format(macroDir, mcFile, parFile, digiFile, recoFile, analysisFile, plutoParticle, colSystem, colEnergy, geoSetup, nofEvents))


#def getPlutoPath(colSystem, colEnergy, plutoParticle, taskId):
#if plutoParticle == "rho0":
#return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/" + colEnergy + "/rho0/epem/pluto.auau." + colEnergy + ".rho0.epem." \
  + str(taskId).zfill(4) + ".root"
#elif plutoParticle == "omegaepem":
#return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/" + colEnergy + "/omega/epem/pluto.auau." + colEnergy                \
  + ".omega.epem." + str(taskId).zfill(4) + ".root"
#elif plutoParticle == "omegadalitz":
#return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/" + colEnergy + "/omega/pi0epem/pluto.auau." + colEnergy             \
  + ".omega.pi0epem." + str(taskId).zfill(4) + ".root"
#elif plutoParticle == "phi":
#return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/" + colEnergy + "/phi/epem/pluto.auau." + colEnergy + ".phi.epem."   \
  + str(taskId).zfill(4) + ".root"
#elif plutoParticle == "pi0":
#return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktA/" + colEnergy + "/pi0/gepem/pluto.auau." + colEnergy + ".pi0.gepem." \
  + str(taskId).zfill(4) + ".root"
#elif plutoParticle == "inmed":
#return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktRapp/" + colEnergy + "/rapp.inmed/epem/pluto.auau." + colEnergy        \
  + ".rapp.inmed.epem." + str(taskId).zfill(4) + ".root"
#elif plutoParticle == "qgp":
#return "/lustre/nyx/cbm/prod/gen/pluto/auau/cktRapp/" + colEnergy + "/rapp.qgp/epem/pluto.auau." + colEnergy          \
  + ".rapp.qgp.epem." + str(taskId).zfill(4) + ".root"
#elif plutoParticle == "urqmd":
#return ""


if __name__ == '__main__':
    main()
