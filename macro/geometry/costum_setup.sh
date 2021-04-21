#!/bin/bash
# setup-costum.sh
# Author: Eoin Clerkin (FAIR)  2020-11-19  (updated with tof v20b geometry)  2021-04-20
#
# Intended for inspection of geometry setups. At the user risk, it may also work for simulation. Generates a costum-setup file for the CBM experiemnt. User is prompted which subsystem and its configuration to include. The scripts only works for default geometries and is not intended to switch between geometry versions. For detectors which are placed on railsystems the user will be prompted as to specify its distance to the target.
# specifying a variable like PSD_INCLUDE=1 will skip question relating to including the PSD, allowing a user to predefine a configuration skip.
#
# Suggested command to create costum setup file:  sh costum-setup.sh
# Creates the run_transport_costum.C and run_reco_costum.C to use the setup_sis100_auto instead of setup_sis100_electron as default.

[ -z ${MAG_INCLUDE+x} ] && MAG_INCLUDE=1
[ -z ${PIPE_INCLUDE+x} ] && PIPE_INCLUDE=1
[ -z ${PLAT_INCLUDE+x} ] && PLAT_INCLUDE=1

[ -z ${MAG_TAG+x} ] && MAG_TAG="v20a"

[ -z ${PLAT_TAG+x} ] && PLAT_TAG="v13a";

[ -z ${MACRO_DIR+x} ] && MACRO_DIR="${VMCWORKDIR}/macro"
[ -z ${GEOMETRY_DIR+x} ] && GEOMETRY_DIR="${VMCWORKDIR}/geometry"
[ -z ${PARAMETER_DIR+x} ] && PARAMETER_DIR="${VMCWORKDIR}/parameters"

[ -z ${SETUP_FILE+x} ] && SETUP_FILE="${GEOMETRY_DIR}/setup/setup_sis100_auto.C"

[ -z ${VMCWORKDIR+x} ] ||
([ -z ${MACRO_DIR+x} ] && [ -z ${GEOMETRY_DIR+x} ] && [ -z ${SETUP_FILE+x} ]) &&
echo "Have you initiated CbmRoot? i.e. source bin/CbmRootConfig.sh -a\nNeeds VMVWORKDIR defined or MACRO_DIR and GEOMETRY_DIR as variables" &&
exit;

CONFIG_RUN=0;

while test $# -gt 0; do
	case "$1" in
#	-u|--update)
#		echo "*** Updating the run_transport and run_reco macros to use costum setup."
#		CONFIG_RUN="1";
#		;;
	-h|-help|--help|*)
		echo "Autoinstall_framework will install FairSoft, FairRoot and CbmRoot packages."
		head -n 10 setup-costum.sh
		echo "Creates costum setup file."
		exit 123;;
	esac
	shift
done

[ ${CONFIG_RUN} -eq 1 ] && echo "The default macros will be modified to use the costum setup"






# BASH FUNCTION TO LINK THE FILENAME WITH AUTO TO THE DEFAULT VALUE. NEEDS DECT VARIABLE SET TO THE DETECTOR NAME.
link_parameter(){
cd ${PARAMETER_DIR}/${DECT};
eval DEC_TAG='$'`echo ${DECT} | tr [:lower:] [:upper:]`"_TAG";
echo ${DEC_TAG}
for tmpfile in `ls *${DEC_TAG}*`;
do
#	cp -lf ${tmpfile} `echo ${tmpfile} | sed s_${DEC_TAG}_auto_`;
	ln -sf ${tmpfile} `echo ${tmpfile} | sed s_${DEC_TAG}_auto_`;
	echo ${tmpfile}" is linked to "`echo ${tmpfile} | sed s_${DEC_TAG}_auto_`;
done;
cd -
echo "$DECT parameter file linked."
}


















# VMCWORKDIR=/u/eclerkin/cbmroot-installer/computing-cbmroot-master/inst/share/cbmroot

printf "A series of questions will follow which will configure the experimental setup\n"

####################################### MVD Section #################################################################################
[ -z ${MVD_TAG+x} ] &&  MVD_TAG="v20a"			# default version

if [ -z ${MVD_INCLUDE+x} ]; then
	printf "Do you want the MVD detector (Yes/No) ? \n"
	read -r ANSWER && echo $ANSWER  | sed -n '/^\(Y\|y\)/!{q1}' && echo "Yes!" && MVD_INCLUDE=1 ||  MVD_INCLUDE=0;
fi

if [ ${MVD_INCLUDE} -eq 0 ]; then
echo "No!";
fi

if [ ${MVD_INCLUDE} -eq 1 ]; then
#	if [ ! ${MVD_MODE} = "vx" ] && [ ! ${MVD_MODE} = "tr" ]; 
#	then
		printf "Do you want the MVD to be in tracking or vertexing mode [TR or VX]? \n"
		read -r ANSWER && echo $ANSWER  | sed -n '/^\(V\|v\)/!{q1}' && echo "Vertexing mode" && MVD_MODE="vx" || echo "Tracking mode" &&  MVD_MODE="tr";
#	fi
	ROOT_FILE=${GEOMETRY_DIR}/mvd/mvd_${MVD_TAG}_${MVD_MODE}.geo.root
	[ -f ${ROOT_FILE} ] && ln -sf ${ROOT_FILE} ${GEOMETRY_DIR}/mvd/mvd_auto.geo.root || echo "Failure: MVD file failed to link"

DECT="mvd"
link_parameter
fi















####################################### STS Section #################################################################################
[ -z ${STS_TAG+x} ] && STS_TAG="v19a"
if [ -z ${STS_INCLUDE+x} ]; then
	printf "Do you want the STS detector (Yes/No) ? \n"
	read -r ANSWER && echo $ANSWER  | sed -n '/^\(Y\|y\)/!{q1}' && echo "Yes!" && STS_INCLUDE=1 || STS_INCLUDE=0;
fi

[ ${STS_INCLUDE} -eq 0 ] && echo "No!";

if [ ${STS_INCLUDE} -eq 1 ];
then
	ROOT_FILE=${GEOMETRY_DIR}/sts/sts_${STS_TAG}.geo.root
	echo ${ROOT_FILE}
	[ -f ${ROOT_FILE} ] && ln -sf ${ROOT_FILE} ${GEOMETRY_DIR}/sts/sts_auto.geo.root || echo "Failure: STS file failed to link"

DECT="sts"
link_parameter


fi



####################################### RICH Section #################################################################################
[ -z ${RICH_TAG+x} ] && RICH_TAG="v17a_1e"

if [ -z ${RICH_INCLUDE+x} ]; then
	printf "Do you want the RICH detector (Yes/No) ? \n"
	read -r ANSWER && echo $ANSWER  | sed -n '/^\(Y\|y\)/!{q1}' && echo "Yes!" && RICH_INCLUDE=1 || RICH_INCLUDE=0;
fi

echo "INCLUDE RICH" ${RICH_INCLUDE}

if [ ${RICH_INCLUDE} -eq 1 ]; then
	ROOT_FILE=${GEOMETRY_DIR}/rich/rich_${RICH_TAG}.geo.root
	echo ${ROOT_FILE}
	[ -f ${ROOT_FILE} ] && ln -sf ${ROOT_FILE} ${GEOMETRY_DIR}/rich/rich_auto.geo.root || echo "Failure: RICH file failed to link"


# No parameters for rich in directory,
#DECT="rich"
#link_parameter

fi


####################################### MUCH Section #################################################################################
[ -z ${MUCH_TAG+x} ] && MUCH_TAG="v20a";			# default version
MUCH_INCLUDE=0;
FIFTH_ABSORBER="0"
if [ ${RICH_INCLUDE} -eq 0 ]; then
 printf "Do you want the MUCH detector (Yes/No) ? \n"
 read -r ANSWER && echo $ANSWER  | sed -n '/^\(Y\|y\)/!{q1}' && echo "Yes!" && MUCH_INCLUDE=1 || MUCH_INCLUDE=0;
 if [ ${MUCH_INCLUDE} -eq 1 ]; then
 	MAG_TAG="v20b"; # If using the MUCH, the magnet without clamps should be used.
 	MUCH_MODE="jpsi"; # default is J/psi
	FIFTH_ABSORBER="1"; # since J/psi is assumed
 	printf "which configuration of the MUCH is desired?\n [1=J/psi or 2=LMVM or 3=start]\n";
 	read -r ANSWER
 	[ $ANSWER -eq 2 ] && MUCH_MODE="lmvm" && FIFTH_ABSORBER=0;
 	[ $ANSWER -eq 3 ] && MUCH_MODE="lmvm_start" && FIFTH_ABSORBER=0 && MUCH_TAG="v20b";

 	ROOT_FILE=${GEOMETRY_DIR}/much/much_${MUCH_TAG}_sis100_1m_${MUCH_MODE}.geo.root
#	[ $ANSWER -eq 3 ] && ROOT_FILE="much_v20b_sis100_1m_lmvm_start.geo.root"
 	echo ${ROOT_FILE}
 	[ -f ${ROOT_FILE} ] && ln -sf ${ROOT_FILE} ${GEOMETRY_DIR}/much/much_auto.geo.root || echo "Failure: MUCH file failed to link";

 fi
# FIFTH_ABSORBER=0;
# printf "Include room for MUCH's 5th absorber (Yes/No) ? \n"
# read -r ANSWER && echo $ANSWER  | sed -n '/^\(Y\|y\)/!{q1}' && echo "Yes!" && FIFTH_ABSORBER=1 || echo "No!";





DECT="much"
link_parameter



fi



# Switch between different beam pipes depending on whether the MUCH is installed or not.
# Allow override by previous definition.
if [ ${MUCH_INCLUDE} -eq  1 ]; then
	[ -z ${PIPE_TAG+x} ] && PIPE_TAG="v20a_1m"
else
	[ -z ${PIPE_TAG+x} ] && PIPE_TAG="v16b_1e"
fi




####################################### TRD Section #################################################################################


[ -z ${TRD_MODE+x} ] && TRD_MODE="1e";
[ -z ${TRD_TAG+x} ] && TRD_TAG="v20b"; # If not set then specify a default tag.


TRD_INCLUDE=0;
TRD_MIN=4.1;
[ ${MUCH_INCLUDE} -eq 1 ] && TRD_TAG="v20b" && TRD_MODE="1m" && [ ${FIFTH_ABSORBER} -eq 1 ] && TRD_MIN=4.8;
TRD_MAX=12.2;
TRD_DIST=${TRD_MIN};
printf "Do you want to inlcude the TRD (Yes/No) ? \n";
read -r ANSWER && echo $ANSWER  | sed -n '/^\(Y\|y\)/!{q1}' && echo "Yes!" && TRD_INCLUDE=1 || echo "No!";

if [ ${TRD_INCLUDE}  -eq 1 ]
then
	printf "What distance along the beam should the TRD be placed from the target in meters (range %s -> %s ) ? \n" ${TRD_MIN}, ${TRD_MAX};
	read -r ANSWER;
	TRD_DIST=`echo "100 * $ANSWER" | bc -l`

	ROOT_FILE=${GEOMETRY_DIR}/trd/trd_${TRD_TAG}_${TRD_MODE}.geo.root
	KEYNAME="trd_${TRD_TAG}_${TRD_MODE}";

	[ ${MUCH_INCLUDE} -eq 1 ] &&  ROOT_FILE=${GEOMETRY_DIR}/trd/trd_${TRD_TAG}_${TRD_MODE}.geo.root && KEYNAME="trd_${TRD_TAG}_${TRD_MODE}";

root -l << EOT
		TFile file("${ROOT_FILE}")
		file.GetListOfKeys()->Print()
		TGeoVolume* top_vol = (TGeoVolume *) file.Get("${KEYNAME}")
		TGeoTranslation* trd_trans = (TGeoTranslation *) file.Get("trd_trans")
		trd_trans->SetTranslation(0,0,${TRD_DIST})
		file.Close()
		TFile newfile("${GEOMETRY_DIR}/trd/trd_auto_${TRD_MODE}.geo.root","recreate")
		newfile.WriteTObject(top_vol)
		newfile.WriteTObject(trd_trans)
		newfile.Close()
		.q
EOT

DECT="trd"

# TRD_TAG="${TRD_TAG}_${TRD_MODE}"   # Bad hack.
link_parameter


fi



####################################### TOF Section #################################################################################
[ -z ${TOF_TAG+x} ] && TOF_TAG="v20a"
[ -z ${TOF_MODE+x} ] && TOF_MODE="1e"

[ -z ${TOF_INCLUDE+x} ] && TOF_INCLUDE=0

TOF_MIN=6.9
 [ ${MUCH_INCLUDE} -eq 1 ] && TOF_MODE="1m" && [ ${FIFTH_ABSORBER} -eq 1 ] && TOF_MIN=7.7

#`echo "2.5 + (${TRD_DIST}/100)" | bc -l`
TOF_MAX=15.1
TOF_DIST=${TOF_MIN}

printf "Do you want the TOF detector (Yes/No) ? \n"
read -r ANSWER && echo $ANSWER  | sed -n '/^\(Y\|y\)/!{q1}' && echo "Yes!" && TOF_INCLUDE=1 || echo "No!";

if [ ${TOF_INCLUDE} -eq 1 ]; then
	printf "What distance along the beam should the TOF be placed from the target in meters (range %s -> %s ) ? \n" ${TOF_MIN}, ${TOF_MAX};
	read -r ANSWER;
	TOF_DIST=`echo "96 + 100 * $ANSWER" | bc -l`
	ROOT_FILE=${GEOMETRY_DIR}/tof/tof_${TOF_TAG}_${TOF_MODE}.geo.root
  	root -l << EOT
		TFile file("${ROOT_FILE}")
		TGeoVolume* top_vol = (TGeoVolume *) file.Get("tof_${TOF_TAG}_${TOF_MODE}")
		file.GetListOfKeys()->Print()
		//top_vol->GetNodes()->Print()
		//file.ls()
		TGeoTranslation * tof_trans = (TGeoTranslation *) file.Get("")
		tof_trans->Print()
		file.Close()
		tof_trans->SetTranslation(0,0,${TOF_DIST})
		TFile newfile("${GEOMETRY_DIR}/tof/tof_auto_${TOF_MODE}.geo.root","recreate")
		newfile.WriteTObject(top_vol)
		newfile.WriteTObject(tof_trans)
		newfile.Close()
		.q
EOT

DECT="tof"
link_parameter

fi

####################################### BFTC Section #################################################################################

[ -z ${BFTC_INCLUDE+x} ] && BFTC_INCLUDE=1
# Not yet included.
# The current default positions for the BFTC is 9.4m from target and 10m from target when MUCH's 5th absorber is installed.




####################################### PSD Section #################################################################################

PSD_INCLUDE=0
[ -z ${PSD_TAG+x} ] && PSD_TAG="v20a"
[ ${MUCH_INCLUDE} -eq 1 ] && [ ${FIFTH_ABSORBER} -eq 1 ] && PSD_Z_MIN=10.9 || PSD_Z_MIN=10.1
PSD_Z_MAX=17.3; PSD_X_MIN=-0.9; PSD_X_MAX=0.9; PSD_Y_MIN=-0.7; PSD_Y_MAX=0.7;

printf "Do you want the PSD detector (Yes/No) ? \n"
read -r ANSWER && echo $ANSWER  | sed -n '/^\(Y\|y\)/!{q1}' && echo "Yes!" && PSD_INCLUDE=1 || echo "No!";

if [ ${PSD_INCLUDE} -eq 1 ]; then
	printf "Translation of PSD along beam axis (Z = %s -> %s ) ? \n" ${PSD_Z_MIN} ${PSD_Z_MAX};
	read -r ANSWER;
	PSD_Z_DIST=`echo "80 + 100 * $ANSWER" | bc -l`

	printf "Translation of PSD laterially ( X = %s -> %s ) ? \n" ${PSD_X_MIN} ${PSD_X_MAX};
	read -r ANSWER;
	PSD_X_DIST=`echo "100 * $ANSWER" | bc -l`

	printf "Rotation of PSD around the vertical axis ( Rot_Y = -2.5 -> 2.5 ) ? \n";
	read -r ANSWER;
	PSD_Y_ROT=$ANSWER;

# Commented out as the PSD is, although capable, not supposed to move vertically during normal operations of the detector.
#	printf "Translation of PSD vertically ( Y = %s -> %s ) ? \n" ${PSD_Y_MIN} ${PSD_Y_MAX};
#	read -r ANSWER;
#	PSD_Y_DIST=`echo "100 * $ANSWER" | bc -l`
	PSD_Y_DIST=0

	ROOT_FILE=${GEOMETRY_DIR}/psd/psd_${PSD_TAG}.geo.root

root -l << EOT
		TFile file("${ROOT_FILE}")
		TGeoVolume* top_vol = (TGeoVolume *) file.Get("psd_${PSD_TAG}")
		TGeoCombiTrans * psd_combitrans = (TGeoCombiTrans*) file.Get("")
		psd_combitrans->Print()
		file.Close()

		TGeoRotation* rot = new TGeoRotation();
		rot->RotateY(${PSD_Y_ROT})
		rot->Print()

		psd_combitrans = new TGeoCombiTrans(${PSD_X_DIST},${PSD_Y_DIST},${PSD_Z_DIST}, rot);
		psd_combitrans->Print();

		TFile newfile("${GEOMETRY_DIR}/psd/psd_auto.geo.root","recreate")
		newfile.WriteTObject(top_vol)
		newfile.WriteTObject(psd_combitrans)
		newfile.Close()
		.q
EOT


fi


################################################# GENERATING THE SETUP FILE ###########################################################################
cat > ${SETUP_FILE} << EOT
// auto-generated by script

void setup_sis100_auto()
{
  TString magnetGeoTag = "${MAG_TAG}";
  TString pipeGeoTag   = "${PIPE_TAG}";
  TString platGeoTag   = "${PLAT_TAG}"
  TString mvdGeoTag    = "auto";
  TString stsGeoTag    = "auto";
  TString richGeoTag   = "auto";
  TString muchGeoTag   = "auto";
  TString trdGeoTag    = "auto_${TRD_MODE}";
  TString tofGeoTag    = "auto_${TOF_MODE}";
  TString psdGeoTag    = "auto";

  TString fieldTag      = "v18a";
  Double_t fieldZ       = 40.;            // field centre z position
  Double_t fieldScale   =  1.;            // field scaling factor

  CbmSetup* setup = CbmSetup::Instance();
  if ( ! setup->IsEmpty() ) {
  	std::cout << "-W- setup_sis100_costum: overwriting existing setup" << setup->GetTitle() << std::endl;
  	setup->Clear();
  }
  setup->SetTitle("SIS100 - Costum Setup");
EOT

[ ${MAG_INCLUDE} -eq 1 ] && echo "  setup->SetModule(ECbmModuleId::kMagnet, magnetGeoTag);" >> ${SETUP_FILE}
[ ${PIPE_INCLUDE} -eq 1 ] && echo "  setup->SetModule(ECbmModuleId::kPipe, pipeGeoTag);" >> ${SETUP_FILE}
[ ${MVD_INCLUDE} -eq 1 ] && echo "  setup->SetModule(ECbmModuleId::kMvd, mvdGeoTag);" >> ${SETUP_FILE}
[ ${STS_INCLUDE} -eq 1 ] && echo "  setup->SetModule(ECbmModuleId::kSts, stsGeoTag);" >> ${SETUP_FILE}
[ ${RICH_INCLUDE} -eq 1 ] && echo "  setup->SetModule(ECbmModuleId::kRich, richGeoTag);" >> ${SETUP_FILE}
[ ${MUCH_INCLUDE} -eq 1 ] && echo "  setup->SetModule(ECbmModuleId::kMuch, muchGeoTag);" >> ${SETUP_FILE}
[ ${TRD_INCLUDE} -eq 1 ] && echo "  setup->SetModule(ECbmModuleId::kTrd, trdGeoTag);" >> ${SETUP_FILE}
[ ${TOF_INCLUDE} -eq 1 ] && echo "  setup->SetModule(ECbmModuleId::kTof, tofGeoTag);" >> ${SETUP_FILE}
[ ${PSD_INCLUDE} -eq 1 ] && echo "  setup->SetModule(ECbmModuleId::kPsd, psdGeoTag);" >> ${SETUP_FILE}
[ ${PLAT_INCLUDE} -eq 1 ] && echo "  setup->SetModule(ECbmModuleId::kPlatform, platGeoTag);" >> ${SETUP_FILE}

echo "  setup->SetField(fieldTag, fieldScale, 0., 0., fieldZ);" >> ${SETUP_FILE}
echo "}" >> ${SETUP_FILE}

echo "costum setup file \"${SETUP_FILE}\" generated"



# modifies run_transport.C to use the costum setup file.
# [ ${CONFIG_RUN} -eq 1 ] &&

sed -e 's|const char\* setupName = "sis100_electron"|const char* setupName = "sis100_auto"|' -e 's|void run_transport(|void costum_transport(|' ${MACRO_DIR}/run/run_transport.C > ${PWD}/costum_transport.C

sed -e 's|void run_digi(|void costum_digi(|' ${MACRO_DIR}/run/run_digi.C > ${PWD}/costum_digi.C

sed -e 's|const char\* setupName = "sis100_electron"|const char* setupName = "sis100_auto"|' -e 's|void run_reco(|void costum_reco(|' ${MACRO_DIR}/run/run_reco.C > ${PWD}/costum_reco.C

echo "Program ended successfully"

return 1;

