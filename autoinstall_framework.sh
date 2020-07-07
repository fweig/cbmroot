#!/bin/bash
#
## semi-automated script installing of FairSoft, FairRoot and CbmRoot
#
# usage:
# $ ./autoinstall_framework.sh
# for installation of all three or
# $ ./autoinstall_framework.sh --help
# to see a help file with possible user flags.
#
# 11.06.2020 - modification for robustness and to bring up to line with standard by Eoin Clerkin
# 31.08.2018 - re-introduce old version
# 24.05.2018 - switch to oct17p1 as dev version
# 31.01.2017 - make ROOT6 the default
# 17.12.2015 - split fairsoft directory into src and install
# 01.12.2015 - add selection of root version
# 17.07.2015 - introduce option to compile dev settings
# 02.06.2015 - introduce parameters for individual package selection
# 13.03.2015 - initial version
# by David Emschermann

exec &> >(tee -a .autoinstall_framework.log) # So the user can check up a complete log if he wishes.


# choose your root version
export ROOTVER=6

# put your desired variants here:
export FSOFTDEV=jun19p2
export FROOTDEV=v18.4.0

export FSOFTPRO=jun19p2
export FROOTPRO=v18.2.1

export FSOFTOLD=jun19p1
export FROOTOLD=v18.2.0

# set default version to pro
export FSOFTVER=$FSOFTPRO
export FROOTVER=$FROOTPRO


SETUP_FAIRSOFT=0 && SETUP_FAIRROOT=0 && SETUP_CBMROOT=0;
[ $# -eq "0" ] && SETUP_FAIRSOFT=1 && SETUP_FAIRROOT=1 && SETUP_CBMROOT=1 : echo "Some Error has occurred"  # Default behaviour with no flags, all three should be installed.

while test $# -gt 0; do
	case "$1" in
	-y|-yes|--yes)
		echo "Will update environment to new build after installation"
		ANSWER="YES";
		shift;;
	-n|-no|--no)
		echo "In case someone wants to put in script."
		ANSWER="NO";
		shift;;
	-fs|-s|-fairsof|--fairsoft)
		echo "*** FAIRSOFT to be installed"
		SETUP_FAIRSOFT="1";
		shift;;
	-fr|-r|-fairroot|--fairroot)
		echo "*** FAIRROOT to be installed"
		SETUP_FAIRROOT="1";
		shift;;
	-cr|-c|-cbmroot|--cbmroot)
		echo "*** CBMROOT to be installed"
		SETUP_CBMROOT="1";
		shift;;
	-d|--dev|-dev|dev)
		echo "*** DEV VERSION specified"
		export FSOFTVER=$FSOFTDEV
		export FROOTVER=$FROOTDEV
		shift;;
	-o|--old|-old|old)
		echo "*** OLD VERSION specified"
		export FSOFTVER=$FSOFTOLD
		export FROOTVER=$FROOTOLD
		shift;;
	0|1)
		# This unusual addition is to continue to provide back compatiability with previous versions of autoinstall_framework.
		# It is important to keep this in case someone has hardcoded installation instructions somewhere and thus this should avoid breaking his program.
		# In particular the old behaviour except 1, 2, or 3 numerical arguments whereby
		# argument 1 greater or equal to 1 would install FAIR SOFT
		# argument 2 greater or equal to 1 would install FAIR ROOT
		# argument 3 greater or equal to 1 would install CBM ROOT
		# ergo
		# ./autobuild_framework dev 1 0 0
		# would install FAIR SOFT but not FAIR ROOT nor CBM ROOT.

		if [ $1 -gt 0 ];
		then
		SETUP_FAIRSOFT="1";
		echo "FAIR SOFT flaged for install"
		fi

		if [ ! -z $2 ];   # This combersome and longform if statement exist due to known issues regarding ampersands within bash cases.
		then
		if [ $2 -gt 0 ];
		then
		SETUP_FAIRROOT="1";
		echo "FAIR ROOT flaged for install"
		fi
		fi

		if [ ! -z $3 ];   # This combersome and longform if statement exist due to known issues regarding ampersands within bash cases.
		then
		if [ $3 -gt 0 ];
		then
		SETUP_CBMROOT="1";
		echo "CBMROOT flaged for install"
		fi
		fi

		# To prevent the pausing a script with maybe autoinstall_framework called the old way.
		ANSWER="NO";

		break;;
	-h|-help|--help|*)
		echo "Autoinstall_framework will install FairSoft, FairSoft, and CbmRoot packages"
		echo "If no flags are specified, the program will install all three"
		echo "otherwise the user may specify one or more to by calling the corresponding flags"
		echo "-h, --help	shows this brief help"
		echo "-d, --dev		Runs with dev"
		echo "-o, --old		Runs a old version"
		echo "-fs, --fairsoft	Installation of FairSoft"
		echo "-fr, --fairroot	Installation of FairRoot"
		echo "-cr, --cbmroot	Installation of CbmRoot"
		echo "-y, --yes		Automatically uses new envirnoment configuration post installation"
		echo "-n, --no		Answers No to automatic environment update"
		echo ""
		echo "Example case to install only FairRoot and CbmRoot (and not FairSoft)"
		echo "./autoinstall_framework.sh -fr -cr"
		exit 0;;
  esac
done

# My experience tells me that the default could be 2, even if there is only 1 processor, as 1 processing and 1 queued job normally is optimal Will leave it at 1 though.
export NUMOFCPU=`[ -f /proc/cpuinfo ] && grep -i processor /proc/cpuinfo | wc -l || echo 1`
export CBMSRCDIR=`pwd`

#-------------------------------------

echo FSOFTVER: $FSOFTVER
echo FROOTVER: $FROOTVER

#-----------------------------------------------------------------------------------------------------------------
#
##   FairSoft
#

if [ $SETUP_FAIRSOFT -ge 1 ]; then
  echo "Setting up Fairsoft ..."

  # check if sqlite3 is available
  if [ -f /usr/include/sqlite3.h ] ; then
   echo
   echo "Sqlite3 is available"
   echo
  else
   echo
   echo "Sqlite3 is not available"
   echo
   echo "On Debian, please install as follows:"
   echo "sudo apt install libsqlite3-dev"
   echo
   echo "On OpenSuSE, please install as follows:"
   echo "zypper install libsqlite3-0 sqlite3-devel"
   echo
   echo "afterwards, restart autoinstall_framework.sh"
   echo
   sleep 5
   exit
  fi

  cd ..
  git clone https://github.com/FairRootGroup/FairSoft fairsoft_src_${FSOFTVER}_root${ROOTVER}
  cd fairsoft_src_${FSOFTVER}_root${ROOTVER}
  git tag -l
  git checkout -b $FSOFTVER $FSOFTVER

  if [ $ROOTVER -eq 6 ]; then
    sed s/build_root6=no/build_root6=yes/ automatic.conf > automatic1_root.conf
  else 
    cp automatic.conf automatic1_root.conf
  fi
  FSOFTINSTALLPATH=`pwd | sed s/fairsoft_src_/fairsoft_/`
  sed /SIMPATH_INSTALL/d automatic1_root.conf > automatic2_path.conf
  echo "  SIMPATH_INSTALL=$FSOFTINSTALLPATH/installation" >> automatic2_path.conf
  sed s/compiler=/compiler=gcc/ automatic2_path.conf > automatic3_gcc.conf
  ./configure.sh automatic3_gcc.conf

  cd $CBMSRCDIR
  echo done installing FairSoft
fi

#-----------------------------------------------------------------------------------------------------------------
#
##   FairRoot
#

if [ $SETUP_FAIRROOT -ge 1 ]; then
  echo "Setting up Fairroot ..."

  # set SIMPATH
  cd ..
  echo "SIMPATH	before: $SIMPATH"
  cd fairsoft_${FSOFTVER}_root${ROOTVER}/installation/
  export SIMPATH=`pwd`
  echo "SIMPATH	now   : $SIMPATH"
  cd $CBMSRCDIR

  echo PATH=$SIMPATH/bin:$PATH
  export PATH=$SIMPATH/bin:$PATH

  cd ..
  git clone https://github.com/FairRootGroup/FairRoot.git fairroot_src_$FROOTVER-fairsoft_${FSOFTVER}_root${ROOTVER}
  cd fairroot_src_$FROOTVER-fairsoft_${FSOFTVER}_root${ROOTVER}
  git tag -l
  git checkout -b $FROOTVER $FROOTVER
  mkdir build
  cd build
  cmake \
    -DCMAKE_CXX_COMPILER=$($SIMPATH/bin/fairsoft-config --cxx) \
    -DCMAKE_C_COMPILER=$($SIMPATH/bin/fairsoft-config --cc) \
    -DCMAKE_INSTALL_PREFIX=../../fairroot_$FROOTVER-fairsoft_${FSOFTVER}_root${ROOTVER} \
    ..
  nice make install -j$NUMOFCPU

  cd $CBMSRCDIR
  echo done installing FairRoot
fi

#-----------------------------------------------------------------------------------------------------------------
#
##   CbmRoot
#

if [ ${SETUP_CBMROOT} -eq "1" ]; 
then
  echo "Setting up Cbmroot ..."

  # set SIMPATH
  cd ..
  echo "SIMPATH	before: $SIMPATH"
  cd fairsoft_${FSOFTVER}_root${ROOTVER}/installation/
  export SIMPATH=`pwd`
  echo "SIMPATH	now   : $SIMPATH"
  cd $CBMSRCDIR

  # set FAIRROOTPATH
  cd ..
  cd fairroot_$FROOTVER-fairsoft_${FSOFTVER}_root${ROOTVER}
  export FAIRROOTPATH=`pwd`
  echo "FAIRROOTPATH: $FAIRROOTPATH"
  cd $CBMSRCDIR

  echo PATH=$SIMPATH/bin:$PATH
  export PATH=$SIMPATH/bin:$PATH

  cd ..

  cd $CBMSRCDIR
  mkdir build
  cd build
  cmake \
    -DCMAKE_CXX_COMPILER=$($SIMPATH/bin/fairsoft-config --cxx) \
    -DCMAKE_C_COMPILER=$($SIMPATH/bin/fairsoft-config --cc) \
    ..
  nice make -j$NUMOFCPU
  cd ..

echo "done installing CbmRoot"

[ -z $ANSWER ] && (
cat << EOT
Since the system is now installed. Shall I switch to the new environment?
source build/config.sh
Reply Yes or Y for confirmation ????
EOT
) && read ANSWER

if (echo "$ANSWER" | sed -n '/^\(Y\|y\)/!{q10}')
then
  echo "A yes detected."
  echo "Environmental variables and paths updated"
  source build/config.sh;
#  export SIMPATH=$SIMPATH;
#  export FAIRROOTPATH=$FAIRROOTPATH;
fi


fi


echo "Installation Complete"

#####################################################################################

