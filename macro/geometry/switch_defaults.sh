#4f2efee3174a5defe5f16!/bin/bash 
# Copyright (C) 2021 Facility for AntiProton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Eoin Clerkin

echo "programme starts"

# Detecting the GEOMETRY and PARAMETER GIT DIRECTORIES
if [ -d ${PWD}/../../geometry/.git -a -d ${PWD}/../../parameters/.git ]; then
GEO_SRC_DIR="$PWD/../../geometry" 
PAR_SRC_DIR="$PWD/../../parameters" 
cd $PWD/../run
RUN_SRC_DIR=$PWD
cd -
else
	if [ -z ${VMWORKDIR} -a -d ${VMCWORKDIR}/geometry/.git  -a -d ${VMCWORKDIR}/parameters/.git ]; then
	PAR_SRC_DIR="${VMCWORKDIR}/parameters"	
	GEO_SRC_DIR="${VMCWORKDIR}/geometry"
	RUN_SRC_DIR="${VMCWORKDIR}/macro/run"
	else
	echo "Geometry or Parameter Git Source Directory not detected"
	exit
	fi
fi

# TAG FOR THE OFFICIAL APR 2019 CBMROOT SW Release
APR20_GEOTAG="b085952c8a097bc4b614e5d956285bdf20ef5670"
APR20_PARATAG="fc62e1c9f2a67109e45389ed93537a7d08973ce2"

# TAG FOR THE OFFICIAL APR 2020 CBMROOT SW Release
APR21_GEOTAG="875439f929577d678b86bbc243b8f99a094fd31c"
APR21_PARATAG="fc62e1c9f2a67109e45389ed93537a7d08973ce2"

# TAG FOR NEW EXPERIMENTAL GEOMETRIES. NEXT SOFTWARE RELEASE CANDIDATES
TEST_GEOTAG="114c7830dc6b8460a8527844aa253ec0c33b414d"
TEST_PARATAG="21bed5c5e746efdb63b696f4053b9e5f65f940a8"

# GEOMETRY AMD PARAMETER GIT REPOSITORY
GIT_GEOREPO="https://git.cbm.gsi.de/e.clerkin/cbmroot_geometry.git"
GIT_PARAREPO="https://git.cbm.gsi.de/e.clerkin/cbmroot_parameter.git"

##########1234567890##########1234567890##########1234567890##########1234567890##########
cat << EOT
*******************************************************************************************
WARNING THIS SCRIPT WILL MODIFY CBMROOT SIMULATION ENVIRNOMENT. PLEASE READ BELOW CAREFULLY
*******************************************************************************************

This script switches between official and trial versions of the CBMROOT geometries. This 
is intended for use by a knowledgable user, who will remember to swith back to the official
geometry relase (currently APR21) once the specific use case has ended. Current options 
include

APR20 - (previous 2020 default geometries. Run old defaults with the new CBMROOT software.)
APR21 - (current 2021 default geometries. This is the official release geometries.
TEST - (Geoemtries shift such that the center of the magnet is the origin of the CBM exp.)

Please choose an option, type exact name [APR20,APR21,TEST] and press return.
EOT
read RELEASE

case "${RELEASE}" in
    APR20)  echo "You specified APR20"; GEOMETRY_TAG=${APR20_GEOTAG}; PARAMETER_TAG=${APR20_PARATAG}; TARGET_Z_POS=0 ;;
    APR21)  echo "You specified APR21"; GEOMETRY_TAG=${APR21_GEOTAG}; PARAMETER_TAG=${APR21_PARATAG}; TARGET_Z_POS=0 ;;
    TEST)   echo "You specified ORIGIN_SHIFT "; GEOMETRY_TAG=${TEST_GEOTAG};  PARAMETER_TAG=${TEST_PARATAG}; TARGET_Z_POS=-40 ;;
    *)      echo "$ANSWER is an unknown option"; exit 1 ;;
esac





cat << EOT
Have you read above, and understand that you will need to rerun the script to revert the changes?
TYPE Yes for confirmation ????
EOT

read CONFIRMATION

if (echo "${CONFIRMATION}" | sed -n '/^Yes$/!{q10}')
then
	echo "Yes typed!"
	
	sed -i 's/Double_t[[:space:]]targetZpos.*$/Double_t targetZpos = '${TARGET_Z_POS}'/' ${RUN_SRC_DIR}/run_tra_file.C;	

# Geometry Repostiory
	cd ${GEO_SRC_DIR}
		git checkout ${GEOMETRY_TAG}
		if [ $? -ne 0 ]; then
			git fetch ${GIT_GEOREPO} ${GEOMETRY_TAG}
			git checkout ${GEOMETRY_TAG}
			if [ $? -ne 0 ]; then
				echo "[FAIL] TO SWITCH TO SPECIFIED GEOMETRIES:"	
				exit 101;
			fi
		fi
	
	cd $OLDPWD
# Parameter Repository	
	cd ${PAR_SRC_DIR}
		git checkout ${PARAMETER_TAG}
		if [ $? -ne 0 ]; then
			git fetch ${GIT_PARAREPO} ${PARAMETER_TAG}
			git checkout ${PARAMETER_TAG}
			if [ $? -ne 0 ]; then
				echo "[FAIL] TO SWITCH TO SPECIFIED PARAMETERS:"	
				exit 102;
			fi
		fi

	cd $OLDPWD
else
  echo "No is assumed!"
  echo "Positive answer has been restricted to \"Yes\" only, with upper-case Y followed by lower-case e and s, no extra characters permitted. "
  echo "No changed to geometry and parameter repositoroes made"
fi

cat << EOT
The position of the target is at ${TARGET_Z_POS} cm from the origin.
You may now run the standard CBMROOT macros, e.g.

root -l -q ${RUN_SRC_DIR}/run_tra_file.C
root -l -q ${RUN_SRC_DIR}/run_digi.C
root -l -q ${RUN_SRC_DIR}/run_reco.C

and the specified default geometries will be used. To revert these changes, you may re-run this script or rebuild the CBMROOT framework.

EOT

echo "Program Ended"
