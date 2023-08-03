#!/bin/bash

# Setup the needed environment
export LD_LIBRARY_PATH=/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:$DYLD_LIBRARY_PATH
export PATH=/home/cbmdata/_deps/fairsoft_apr22/install//bin:$PATH:/home/cbmdata/_deps/fairsoft_apr22/install/lib/Geant4-11.0.1
export ROOTSYS=
export ROOTEXE=/home/cbmdata/_deps/fairsoft_apr22/install/bin/root
export ROOT_INCLUDE_PATH="/home/cbmdata/_deps/fairsoft_apr22/install/include/TGeant3:"
export VMCWORKDIR=/home/heinemann/gpuunpacker/cbmroot
export Geant4VMC_MACRO_DIR=
export USE_VGM=1
export BOOST=

####################### Create the data set variables for Geant4 #############
export G4NEUTRONHPDATA=/home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4NDL4.6
export G4LEDATA=/home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4EMLOW8.0
export G4LEVELGAMMADATA=/home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/PhotonEvaporation5.7
export G4RADIOACTIVEDATA=/home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/RadioactiveDecay5.6
export G4PARTICLEXSDATA=/home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4PARTICLEXS4.0
export G4PIIDATA=/home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4PII1.3
export G4REALSURFACEDATA=/home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/RealSurface2.2
export G4SAIDXSDATA=/home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4SAIDDATA2.0
export G4ABLADATA=/home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4ABLA3.1
export G4INCLDATA=/home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4INCL1.0
export G4ENSDFSTATEDATA=/home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4ENSDFSTATE2.3
##############################################################################


export G3SYS=""
export PYTHIA8DATA=""
if [ -e /home/cbmdata/_deps/fairsoft_apr22/install/lib/Geant4-11.0.1/env.sh ]; then
  . /home/cbmdata/_deps/fairsoft_apr22/install/lib/Geant4-11.0.1/env.sh
fi

cd /home/heinemann/gpuunpacker/cbmroot/build2/macro/

source  /home/heinemann/gpuunpacker/cbmroot/scripts/checklibs.sh
