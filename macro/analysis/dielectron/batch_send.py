#!/usr/bin/env python3

import os
import shutil

nofJobs = 648
timeLimit="08:00:00"
mirrorRotation=15
#All data in data dir will be removed
removeData = False
dataDir = "/lustre/nyx/cbm/users/slebedev/cbm/data/geoopt_m" + str(mirrorRotation) + "/"
logFile = dataDir + "/log/log_slurm-%A_%a.out"
errorFile = dataDir + "/error/error_slurm-%A_%a.out"
jobName = "RICH" + str(mirrorRotation)

if removeData:
    print("All data in dataDir will be removed. Dir:" + dataDir)
    print("Removing...")
    if os.path.exists(dataDir):
        shutil.rmtree(dataDir)
    os.makedirs(dataDir)

if os.path.exists(os.path.dirname(logFile)):
    shutil.rmtree(os.path.dirname(logFile))
os.makedirs(os.path.dirname(logFile))

if os.path.exists(os.path.dirname(errorFile)):
    shutil.rmtree(os.path.dirname(errorFile))
os.makedirs(os.path.dirname(errorFile))

#-p debug
commandStr=('sbatch --job-name={} --time={} --output={} --error={} --array=1-{} batch_job.py {} {}').format(jobName, timeLimit, logFile, errorFile, nofJobs, dataDir, mirrorRotation)


#print(commandStr)
os.system(commandStr)

