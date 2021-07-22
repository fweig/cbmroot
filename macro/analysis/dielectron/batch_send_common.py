#!/usr/bin/env python3

import os
import shutil

def main():
    nofJobs = 100
    timeLimit="08:00:00"
    geoSetup="sis100_electron"
    plutoParticles = ["rho0", "wdalitz", "w", "phi", "qgp_epem"]
    
    recoDataDirIn = "/lustre/nyx/cbm/pwg/common/mc/cbmsim/apr20_fr_18.2.1_fs_jun19p1/urqmd_pluto_"
    dataDirOut = "/lustre/nyx/cbm/users/criesen/cbm/data/lmvm_CommonData/"
    
    jobName = "LMVM"

    #All data in data dir will be removed
    removeData = False
    if removeData:
        print("All data in dataDirOut will be removed. Dir:" + dataDirOut)
        print("Removing...")
        if os.path.exists(dataDirOut):
            shutil.rmtree(dataDirOut)
    os.makedirs(dataDirOut,exist_ok=True)
        
    for plutoParticle in plutoParticles:
        recoDataDirInPluto  = recoDataDirIn + plutoParticle + "/"           
        dataDirOutPluto = dataDirOut + plutoParticle + "/"
        logFile = dataDirOutPluto + "/log/log_slurm-%A_%a.out"
        errorFile = dataDirOutPluto + "/error/error_slurm-%A_%a.out"
        workDir = dataDirOutPluto + "/workdir/"        
        makeLogErrorDirs(logFile, errorFile, workDir)

        #- p debug
        commandStr=('sbatch --job-name={} --time={} --output={} --error={} --array=1-{} -- ./batch_job_common.py {} {} {} {}').format(jobName, timeLimit, logFile, errorFile, nofJobs, recoDataDirInPluto, dataDirOutPluto, geoSetup, plutoParticle)

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
