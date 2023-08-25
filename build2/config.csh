#!/bin/csh

setenv Linux_Flavour_ "Ubuntu 22.04.3 LTS"
setenv System_ "x86_64"
. /home/heinemann/gpuunpacker/cbmroot/build2/check_system.csh
if ( "$same_system" == "1" ) then
	setenv SIMPATH "/home/cbmdata/_deps/fairsoft_apr22/install/"
	setenv ROOTSYS "/home/cbmdata/_deps/fairsoft_apr22/install/"
	setenv FAIRROOTPATH "/home/cbmdata/_deps/fairroot_v18.6.9/install/"
	if ( true == true ) then
		setenv Geant4_INCLUDE_DIRS "/home/cbmdata/_deps/fairsoft_apr22/install/include/Geant4"
		setenv Geant4VMC_INCLUDE_DIRS "/home/cbmdata/_deps/fairsoft_apr22/install/include/geant4vmc:/home/cbmdata/_deps/fairsoft_apr22/install/include/g4root"
		setenv Geant4VMC_LIBRARY_DIR ""
		setenv Geant4VMC_MACRO_DIR ""
		setenv PYTHIA6_LIBRARY_DIR ""
		setenv Geant3_INCLUDE_DIRS "/home/cbmdata/_deps/fairsoft_apr22/install/include/TGeant3"
		setenv G3SYS ""
		setenv Geant3_LIBRARY_DIR ""
		setenv USE_VGM "1"
		setenv PYTHIA8DATA ""

		####################### Create the data set variables for Geant4 #############
setenv G4NEUTRONHPDATA  /home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4NDL4.6
setenv G4LEDATA  /home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4EMLOW8.0
setenv G4LEVELGAMMADATA  /home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/PhotonEvaporation5.7
setenv G4RADIOACTIVEDATA  /home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/RadioactiveDecay5.6
setenv G4PARTICLEXSDATA  /home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4PARTICLEXS4.0
setenv G4PIIDATA  /home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4PII1.3
setenv G4REALSURFACEDATA  /home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/RealSurface2.2
setenv G4SAIDXSDATA  /home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4SAIDDATA2.0
setenv G4ABLADATA  /home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4ABLA3.1
setenv G4INCLDATA  /home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4INCL1.0
setenv G4ENSDFSTATEDATA  /home/cbmdata/_deps/fairsoft_apr22/install/share/Geant4-11.0.1/data/G4ENSDFSTATE2.3
##############################################################################


	endif
	setenv Geant3_INCLUDE_DIRS "/home/cbmdata/_deps/fairsoft_apr22/install/include/TGeant3"
	setenv ROOT_LIBRARY_DIR "/home/cbmdata/_deps/fairsoft_apr22/install/lib"
	setenv ROOT_LIBRARIES "/home/cbmdata/_deps/fairsoft_apr22/install/lib/libCore.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libImt.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libRIO.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libNet.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libHist.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf3d.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libGpad.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libROOTDataFrame.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libTree.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libTreePlayer.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libRint.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libPostscript.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libMatrix.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libPhysics.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libMathCore.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libThread.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libMultiProc.so:/home/cbmdata/_deps/fairsoft_apr22/install/lib/libROOTVecOps.so"
	setenv ROOT_INCLUDE_DIR ""
	setenv ROOT_INCLUDE_PATH "/home/cbmdata/_deps/fairsoft_apr22/install/include/TGeant3:/home/cbmdata/_deps/fairroot_v18.6.9/install/include"
	setenv VMCWORKDIR "/home/heinemann/gpuunpacker/cbmroot"
	setenv FAIRLIBDIR "/home/heinemann/gpuunpacker/cbmroot/build2/lib"
	setenv PYTHONPATH "/home/heinemann/gpuunpacker/cbmroot/python:/home/cbmdata/_deps/fairsoft_apr22/install//lib:/home/cbmdata/_deps/fairsoft_apr22/install//lib/root:/home/cbmdata/_deps/fairsoft_apr22/install//lib/Geant4:/home/cbmdata/_deps/fairsoft_apr22/install//lib/g4py"
	if (! $?OMP_NUM_THREADS) then
		setenv OMP_NUM_THREADS 32
	else
		if ("$OMP_NUM_THREADS" == "")  then
			setenv OMP_NUM_THREADS 32
		endif
	endif
	switch( $1 )
		case '-a' :
			setenv DYLD_LIBRARY_PATH $DYLD_LIBRARY_PATH:""
			setenv LD_LIBRARY_PATH $LD_LIBRARY_PATH:"/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib"
			setenv PATH $PATH:"/home/cbmdata/_deps/fairsoft_apr22/install/bin"
			breaksw
		case '-p' :
			setenv DYLD_LIBRARY_PATH="":$DYLD_LIBRARY_PATH
			setenv LD_LIBRARY_PATH="/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib":$LD_LIBRARY_PATH
			setenv PATH="/home/cbmdata/_deps/fairsoft_apr22/install/bin":$PATH
			echo "hello"
			breaksw
		case * :
		        echo "Default was changed"
		        echo "Append new paths to LD_LIBRARY_PATH and PATH instead of overwritting"
                        setenv DYLD_LIBRARY_PATH $DYLD_LIBRARY_PATH:""
                        setenv LD_LIBRARY_PATH $LD_LIBRARY_PATH:"/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib"
                        setenv PATH $PATH:"/home/cbmdata/_deps/fairsoft_apr22/install/bin"
                        breaksw
	endsw
endif
