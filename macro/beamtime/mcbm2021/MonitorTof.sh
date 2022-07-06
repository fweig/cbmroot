#!/bin/bash
# Copyright (C) 2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only

# shell script to apply clusterizer calibrations
#SBATCH -J MonitorTof
#SBATCH -D /lustre/cbm/users/nh/CBM/cbmroot/trunk/macro/beamtime/mcbm2021
#SBATCH --time=48:00:00
#SBATCH --mem=4000
#SBATCH --partition=long

CTL_FILE=/tmp/MonitorTofRunning
touch $CTL_FILE
chmod a+w $CTL_FILE

#define paths
# here for cbmin006
source /home/shared/herrmann/cbmroot/build/config.sh
cd /home/shared/herrmann/cbmroot/macro/beamtime/mcbm2021

while [ -e $CTL_FILE ]; do 
  root -l 'MonitorTof.C("","localhost",100,8080,200)' &> MonitorTof.log 
  #sleep 180
done 
