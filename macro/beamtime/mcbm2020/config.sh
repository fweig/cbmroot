#!/bin/bash
# Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Pierre-Alain Loizeau


export Linux_Flavour_="Ubuntu 18.04.2 LTS"
export System_="x86_64"
. /opt/cbmsoft/cbmsrc/build/check_system.sh
if [ $same_system -eq 1 ]; then
	export SIMPATH="/opt/fairsoft/jun19p1/"
	export ROOTSYS="/opt/fairsoft/jun19p1"
	export FAIRROOTPATH="/opt/fairroot/v18.2.1_fairsoft_jun19p1/"
	if (true); then
		export Geant4_INCLUDE_DIRS=""
		export Geant4VMC_INCLUDE_DIRS=""
		export Geant4VMC_LIBRARY_DIR=""
		export Geant4VMC_MACRO_DIR=""
		export PLUTO_LIBRARY_DIR=""
		export PLUTO_INCLUDE_DIR=""
		export PYTHIA6_LIBRARY_DIR=""
		export Geant3_INCLUDE_DIRS=""
		export G3SYS=""
		export Geant3_LIBRARY_DIR=""
		export USE_VGM="1"
		export PYTHIA8DATA="/opt/fairsoft/jun19p1//share/pythia8/xmldoc"
		export CLASSPATH=""

		####################### Create the data set variables for Geant4 #############
export G4NEUTRONHPDATA=/opt/fairsoft/jun19p1/share/Geant4-10.5.1/data/G4NDL4.5
export G4LEDATA=/opt/fairsoft/jun19p1/share/Geant4-10.5.1/data/G4EMLOW7.7
export G4LEVELGAMMADATA=/opt/fairsoft/jun19p1/share/Geant4-10.5.1/data/PhotonEvaporation5.3
export G4RADIOACTIVEDATA=/opt/fairsoft/jun19p1/share/Geant4-10.5.1/data/RadioactiveDecay5.3
export G4PARTICLEXSDATA=/opt/fairsoft/jun19p1/share/Geant4-10.5.1/data/G4PARTICLEXS1.1
export G4PIIDATA=/opt/fairsoft/jun19p1/share/Geant4-10.5.1/data/G4PII1.3
export G4REALSURFACEDATA=/opt/fairsoft/jun19p1/share/Geant4-10.5.1/data/RealSurface2.1.1
export G4SAIDXSDATA=/opt/fairsoft/jun19p1/share/Geant4-10.5.1/data/G4SAIDDATA2.0
export G4ABLADATA=/opt/fairsoft/jun19p1/share/Geant4-10.5.1/data/G4ABLA3.1
export G4INCLDATA=/opt/fairsoft/jun19p1/share/Geant4-10.5.1/data/G4INCL1.0
export G4ENSDFSTATEDATA=/opt/fairsoft/jun19p1/share/Geant4-10.5.1/data/G4ENSDFSTATE2.2
##############################################################################

	fi
	export Geant3_INCLUDE_DIRS=""
	export ROOT_LIBRARY_DIR="/opt/fairsoft/jun19p1/lib/root"
	export ROOT_LIBRARIES="-L/opt/fairsoft/jun19p1/lib/root -lGui -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lROOTDataFrame -pthread -Wl,-rpath,/opt/fairsoft/jun19p1/lib/root -lm -ldl -rdynamic"
	export ROOT_INCLUDE_DIR="/opt/fairsoft/jun19p1/include/root6"
	export ROOT_INCLUDE_PATH=":/opt/fairroot/v18.2.1_fairsoft_jun19p1/include:/opt/fairsoft/jun19p1/include/TGeant3"
	export VMCWORKDIR="/opt/cbmsoft/cbmsrc"
	export FAIRLIBDIR="/opt/cbmsoft/cbmsrc/build/lib"
	export PYTHONPATH="/opt/cbmsoft/cbmsrc/python:/opt/fairsoft/jun19p1//lib:/opt/fairsoft/jun19p1//lib/root:/opt/fairsoft/jun19p1//lib/Geant4:/opt/fairsoft/jun19p1//lib/g4py"
	case $1 in
		-a | --append )
			export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:""
			export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"/opt/cbmsoft/cbmsrc/build/lib:/opt/fairsoft/jun19p1/lib:/opt/fairroot/v18.2.1_fairsoft_jun19p1/lib:/opt/fairsoft/jun19p1/lib/root"
			export PATH=$PATH:"/opt/fairsoft/jun19p1/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/opt/fles/bin:/opt/spm"
			;;
		-p | --prepend )
			export DYLD_LIBRARY_PATH="":$DYLD_LIBRARY_PATH
			export LD_LIBRARY_PATH="/opt/cbmsoft/cbmsrc/build/lib:/opt/fairsoft/jun19p1/lib:/opt/fairroot/v18.2.1_fairsoft_jun19p1/lib:/opt/fairsoft/jun19p1/lib/root":$LD_LIBRARY_PATH
			export PATH="/opt/fairsoft/jun19p1/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/opt/fles/bin:/opt/spm":$PATH
			;;
		* )
			export DYLD_LIBRARY_PATH=""
			export LD_LIBRARY_PATH="/opt/cbmsoft/cbmsrc/build/lib:/opt/fairsoft/jun19p1/lib:/opt/fairroot/v18.2.1_fairsoft_jun19p1/lib:/opt/fairsoft/jun19p1/lib/root"
			export PATH="/opt/fairsoft/jun19p1/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/opt/fles/bin:/opt/spm"
			;;
	esac
fi
