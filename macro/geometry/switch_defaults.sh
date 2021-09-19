#4f2efee3174a5defe5f16!/bin/bash 
# Copyright (C) 2021 Facility for AntiProton and Ion Research in Europe, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Eoin Clerkin

echo "programme starts"

if false; then
if [ -z ${VMWORKDIR} ];
then
cat << EOT
BUILT AND CONFIGURATION CBMROOT ENIVORNMENT NOT DETECTED\n
HAVE YOU
. build/config.sh -a
SCRIPTS REQUIRES A RUNNING SYSTEM.
EOT
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
    APR20)  echo "You specified APR20"; GEOMETRY_TAG=${APR20_GEOTAG}; PARAMETER_TAG=${APR20_PARATAG}  ;;
    APR21)  echo "You specified APR21"; GEOMETRY_TAG=${APR21_GEOTAG}; PARAMETER_TAG=${APR21_PARATAG} ;;
    TEST)   echo "You specified TEST "; GEOMETRY_TAG=${TEST_GEOTAG};  PARAMETER_TAG=${TEST_PARATAG} ;;
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
	


# Geometry Repostiory
	cd ${VMCWORKDIR}/geometry
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
	cd ${VMCWORKDIR}/parameters
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
You may now run the standard CBMROOT macros, e.g.

root -l run_tra_file.C
root -l run_digi.C
root -l run_reco.C

and the specified default geometries will be used. To revert these changes, you may re-run this script or rebuild the CBMROOT framework.

EOT


echo "Program Ended"
