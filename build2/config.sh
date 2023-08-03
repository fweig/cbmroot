#!/bin/bash

export Linux_Flavour_="Ubuntu 22.04.2 LTS"
export System_="x86_64"
. /home/heinemann/gpuunpacker/cbmroot/build2/check_system.sh
if [ $same_system -eq 1 ]; then
	export SIMPATH="/home/cbmdata/_deps/fairsoft_apr22/install/"
	export ROOTSYS="/home/cbmdata/_deps/fairsoft_apr22/install/"
	export FAIRROOTPATH="/home/cbmdata/_deps/fairroot_v18.6.9/install/"
	if (true); then
		export Geant4_INCLUDE_DIRS="/home/cbmdata/_deps/fairsoft_apr22/install/include/Geant4"
		export Geant4VMC_INCLUDE_DIRS="/home/cbmdata/_deps/fairsoft_apr22/install/include/geant4vmc:/home/cbmdata/_deps/fairsoft_apr22/install/include/g4root"
		export Geant4VMC_LIBRARY_DIR=""
		export Geant4VMC_MACRO_DIR=""
		export PYTHIA6_LIBRARY_DIR=""
		export Geant3_INCLUDE_DIRS="/home/cbmdata/_deps/fairsoft_apr22/install/include/TGeant3"
		export G3SYS=""
		export Geant3_LIBRARY_DIR=""
		export USE_VGM="1"
		export PYTHIA8DATA=""

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

	fi
	export Geant3_INCLUDE_DIRS="/home/cbmdata/_deps/fairsoft_apr22/install/include/TGeant3"
	export ROOT_LIBRARY_DIR="/home/cbmdata/_deps/fairsoft_apr22/install/lib"
	export ROOT_LIBRARIES="/home/cbmdata/_deps/fairsoft_apr22/install/lib/libCore.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libImt.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libRIO.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libNet.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libHist.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf3d.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libGpad.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libROOTDataFrame.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libTree.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libTreePlayer.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libRint.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libPostscript.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libMatrix.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libPhysics.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libMathCore.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libThread.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libMultiProc.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libROOTVecOps.so"
	export ROOT_INCLUDE_DIR=""
	export ROOT_INCLUDE_PATH="/home/cbmdata/_deps/fairsoft_apr22/install/include/TGeant3:/home/cbmdata/_deps/fairroot_v18.6.9/install/include"
	export VMCWORKDIR="/home/heinemann/gpuunpacker/cbmroot"
	export FAIRLIBDIR="/home/heinemann/gpuunpacker/cbmroot/build2/lib"
	export PYTHONPATH="/home/heinemann/gpuunpacker/cbmroot/python:/home/cbmdata/_deps/fairsoft_apr22/install//lib:/home/cbmdata/_deps/fairsoft_apr22/install//lib/root:/home/cbmdata/_deps/fairsoft_apr22/install//lib/Geant4:/home/cbmdata/_deps/fairsoft_apr22/install//lib/g4py"
	if [[ -z $OMP_NUM_THREADS ]]; then
		export OMP_NUM_THREADS=32
	fi

	case $1 in
		-a | --append )
			export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:""
			export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib"
			export PATH=$PATH:"/home/cbmdata/_deps/fairsoft_apr22/install/bin"
			;;
		-p | --prepend )
			export DYLD_LIBRARY_PATH="":$DYLD_LIBRARY_PATH
			export LD_LIBRARY_PATH="/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib":$LD_LIBRARY_PATH
			export PATH="/home/cbmdata/_deps/fairsoft_apr22/install/bin":$PATH
			;;
		-o | --overwrite )
			for rc in /etc/profile $HOME/.bash_profiles; do echo "sourced "; [ -f "" ] && source ""; done;
			export DYLD_LIBRARY_PATH=""
			export LD_LIBRARY_PATH="/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib"
                        export PATH=$PATH:"/home/cbmdata/_deps/fairsoft_apr22/install/bin"
			;;
		* )
                        echo "To append use flag \"-a\". To overwrite use \"-o\". To prepend use \"-p\"."
                        echo "Appended LD_LIBRARY_PATH and PATH by default."
                        export DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:""
                        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib"
                        export PATH=$PATH:"/home/cbmdata/_deps/fairsoft_apr22/install/bin"
			;;
	esac
 	echo "Configured CBMROOT build 96cadf152fb5978efd9295734b58236d9cf6e8de (Tue Jul 11 09:35:10 2023 +0000)"
fi
