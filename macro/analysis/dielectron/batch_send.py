#!/usr/bin/env python3

import os
import shutil

def main():
    nofJobs = 10
    timeLimit="08:00:00"
    geoSetup="sis100_electron"
    plutoParticles = ["inmed", "omegadalitz", "phi", "omegaepem", "qgp"];

    #ADDED
    mirrorRotation=15
    dataDir = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm/"
    jobName = "RICH" + str(mirrorRotation)

    #All data in data dir will be removed
    removeData = False
    #jobName = "LMVM"
    if removeData:
        print("All data in dataDir will be removed. Dir:" + dataDir)
        print("Removing...")
        if os.path.exists(dataDir):
            shutil.rmtree(dataDir)
    os.makedirs(dataDir,exist_ok=True)
        
    for plutoParticle in plutoParticles:
        dataDirPluto = dataDir + plutoParticle
        logFile = dataDirPluto + "/log/log_slurm-%A_%a.out"
        errorFile = dataDirPluto + "/error/error_slurm-%A_%a.out"
        workDir = dataDirPluto + "/workdir/"        
        makeLogErrorDirs(logFile, errorFile, workDir)
        
        #-p debug
        commandStr=('sbatch --job-name={} --time={} --output={} --error={} --array=1-{} batch_job.py {} {} {}').format(jobName, timeLimit, logFile, errorFile, nofJobs, dataDirPluto, geoSetup, plutoParticle)

        os.system(commandStr)


def makeLogErrorDirs(logFile, errorFile, workDir):
    if os.path.exists(os.path.dirname(logFile)):
        shutil.rmtree(os.path.dirname(logFile))
    os.makedirs(os.path.dirname(logFile),exist_ok=True)

    if os.path.exists(os.path.dirname(errorFile)):
        shutil.rmtree(os.path.dirname(errorFile))
    os.makedirs(os.path.dirname(errorFile),exist_ok=True)
    
    if os.path.exists(os.path.dirname(workDir)):
        shutil.rmtree(os.path.dirname(workDir))
    os.makedirs(os.path.dirname(workDir),exist_ok=True)

if __name__ == '__main__':
    main()
