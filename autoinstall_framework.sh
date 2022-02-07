#!/bin/bash
# Copyright (C) 2015 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by David Emschermann
#
## semi-automated script installing of FairSoft, FairRoot and CbmRoot
#
# usage:
# $ ./autoinstall_framework.sh
# for installation of all three or
# $ ./autoinstall_framework.sh 0 0 1
# for installation of CbmRoot or
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

main()
{
  exec &> >(tee -a .autoinstall_framework.log) # So the user can check up a complete log if he wishes.

  setup_env_variables

  parse_command_line "$@"

  echo FSOFTVER: $FSOFTVER
  echo FROOTVER: $FROOTVER

  check_prerequisites

  install_fairsoft

  install_fairroot

  install_cbmroot
}

setup_env_variables()
{
  # choose your root version
  export ROOTVER=6

  # put your desired variants here:
  export FSOFTDEV=apr21p2
  export FROOTDEV=v18.6.7

  export FSOFTPRO=jun19p3
  export FROOTPRO=v18.2.1

  export FSOFTOLD=jun19p3
  export FROOTOLD=v18.2.0

  # set default version to pro
  export FSOFTVER=$FSOFTPRO
  export FROOTVER=$FROOTPRO

  # My experience tells me that the default could be 2, even if there is only 1 processor, as 1 processing and 1 queued job normally is optimal Will leave it at 1 though.
  export NUMOFCPU=`[ -f /proc/cpuinfo ] && grep -i processor /proc/cpuinfo | wc -l || echo 1`
  export CBMSRCDIR=`pwd`
}

parse_command_line()
{
  SETUP_FAIRSOFT=0 && SETUP_FAIRROOT=0 && SETUP_CBMROOT=0;
  [ $# -eq "0" ] && SETUP_FAIRSOFT=1 && SETUP_FAIRROOT=1 && SETUP_CBMROOT=1 : echo "Some Error has occurred"  # Default behaviour with no flags, all three should be installed.

  while test $# -gt 0; do
  	case "$1" in
	-s|-fs|-fairsoft|--fairsoft)
		echo "*** FairSoft to be installed"
		SETUP_FAIRSOFT="1";
		shift;;
	-r|-fr|-fairroot|--fairroot)
		echo "*** FairRoot to be installed"
		SETUP_FAIRROOT="1";
		shift;;
	-c|-cr|-cbmroot|--cbmroot)
		echo "*** CbmRoot to be installed"
		SETUP_CBMROOT="1";
		shift;;
	-d|dev|-dev|--dev)
		echo "*** DEV VERSION specified"
		export FSOFTVER=$FSOFTDEV
		export FROOTVER=$FROOTDEV
		shift;;
	-p|pro|-pro|--pro)
		echo "*** PRO VERSION specified"
		export FSOFTVER=$FSOFTPRO
		export FROOTVER=$FROOTPRO
		shift;;
	-o|old|-old|--old)
		echo "*** OLD VERSION specified"
		export FSOFTVER=$FSOFTOLD
		export FROOTVER=$FROOTOLD
		shift;;
	-y|-yes|--yes)
		echo "Will update environment to new build after installation"
		ANSWER="YES";
		shift;;
	-n|-no|--no)
		echo "In case someone wants to put in script."
		ANSWER="NO";
		shift;;
	0|1)
	        # This unusual addition is to continue to provide back compatiability
	        # with previous versions of autoinstall_framework.
	        # It is important to keep this in case someone has hardcoded installation
	        # instructions somewhere and thus this should avoid breaking his program.
		# In particular the old behaviour accepts 1, 2 or 3 numerical arguments whereby
		# argument 1 greater or equal to 1 would install FairSoft
		# argument 2 greater or equal to 1 would install FairRoot
		# argument 3 greater or equal to 1 would install CbmRoot
		# ergo
		# ./autobuild_framework dev 1 0 0
		# would install FairSoft but not FairRoot nor CbmRoot.

		if [ $1 -gt 0 ]; then
		  SETUP_FAIRSOFT="1";
		  echo "FairSoft flaged for install"
		fi

		if [ ! -z $2 ]; then   # This combersome and longform if statement exist due to known issues regarding ampersands within bash cases.
  		  if [ $2 -gt 0 ]; then
		    SETUP_FAIRROOT="1";
		    echo "FairRoot flaged for install"
		  fi
		fi

		if [ ! -z $3 ]; then   # This combersome and longform if statement exist due to known issues regarding ampersands within bash cases.
  		  if [ $3 -gt 0 ]; then
  		    SETUP_CBMROOT="1";
		    echo "CbmRoot flaged for install"
		  fi
		fi

		# To prevent the pausing a script with maybe autoinstall_framework called the old way.
		ANSWER="NO";

		break;;
	-h|-help|--help|*)
		echo "Autoinstall_framework will install FairSoft, FairRoot and CbmRoot packages"
		echo "If no flags are specified, the program will install all three"
		echo "otherwise the user may specify one or more to by calling the corresponding flags"
		echo "-h, --help	Shows this brief help"
		echo "-fs, --fairsoft	Installation of FairSoft"
		echo "-fr, --fairroot	Installation of FairRoot"
		echo "-cr, --cbmroot	Installation of CbmRoot"
		echo "-d, --dev		Runs with dev version"
		echo "-p, --pro		Runs with pro version"
		echo "-o, --old		Runs with old version"
		echo "-y, --yes		Automatically uses new envirnoment configuration post installation"
		echo "-n, --no		Answers no to automatic environment update"
		echo ""
		echo "Example case to install only FairRoot and CbmRoot (and not FairSoft)"
		echo "./autoinstall_framework.sh -fr -cr"
		exit 0;;
    esac
  done
}

check_prerequisites()
{

  nof_missing_packages=0
  missing_packages=""

  year=$(echo $FSOFTVER | cut -c4,5)

  # Array with binaries which are needed for the compilation
  programs=(cmake curl gcc g++ gfortran make patch sed bzip2 unzip gzip tar svn git flex lsb_release wget automake autoconf libtoolize)

  for executable in "${programs[@]}"; do
    check_executable $executable
  done

  check_file_exist /usr/include/sqlite3.h sqlite3-dev
  check_file_exist /usr/include/X11/Xlib.h x11-dev
  check_file_exist /usr/include/X11/Xft/Xft.h xft-dev
  check_file_exist /usr/include/X11/extensions/Xext.h xext-dev
  check_file_exist /usr/include/X11/xpm.h xpm-dev
  check_file_exist /usr/include/GL/glu.h mesa-dev
  check_file_exist /usr/include/curses.h ncurses-dev
  check_file_exist /usr/include/bzlib.h bz2-dev
  check_file_exist /usr/include/openssl/ssl.h ssl-dev
  check_file_exist /usr/include/gsl/gsl_types.h gsl-dev
  check_file_exist /usr/include/tbb/tbb.h tbb-dev

  check_file_exist /usr/bin/curl-config curl-dev
  check_file_exist /usr/bin/xml2-config xml2-dev


  if [ $year -lt 20 ]; then
    check_file_exist /usr/bin/xmkmf xutils-dev
    check_file_exist /usr/bin/krb5-config krb5-dev
  fi

  tmp=$(which python2-config)
  if [ $? -ne 0 ]; then
    tmp=$(which python3-config)
    if [ $? -ne 0 ]; then
      nof_missing_packages=$((nof_missing_packages+1))
      missing_packages="python-dev, $missing_packages"
    fi
  fi

  if [ $nof_missing_packages -ne 0 ]; then
    echo ""
    echo "The following packages are missing and need to be installed"
    echo $missing_packages
    echo ""
    echo "For further information which packages need to be installed"
    echo "check the documentation on the FairSoft GitHub page at"
    if [ $year -lt 20 ]; then
      echo ""
      echo "https://github.com/FairRootGroup/FairSoft/tree/$FSOFTVER"
      echo ""
      echo "For Linux check dependencies.md"
      echo "For macosx check dependencies_macosx.md"
    else
      echo "https://github.com/FairRootGroup/FairSoft/blob/$FSOFTVER/legacy/dependencies.md"
    fi
    echo ""
    echo "and follow the given instructions."
    echo
    # check if sqlite3 is available
    missing=0
    if [[ "$missing_packages" =~ .*"sqlite3".* ]]; then
      missing=$((missing+1))
    fi
    if [[ "$missing_packages" =~ .*"gsl".* ]]; then
      missing=$((missing+1))
    fi
    if [[ "$missing_packages" =~ .*"tbb".* ]]; then
      missing=$((missing+1))
    fi
    if [[ $missing -ne 0 ]];then
      if [[ "$missing_packages" =~ .*"sqlite3".* ]]; then
        echo
        echo "Sqlite3 is not available which may not be part of the above installation instructions"
        echo
        echo "On Debian or Ubuntu, please install as follows:"
        echo "sudo apt install libsqlite3-dev"
        echo
        echo "On OpenSuSE, please install as follows:"
        echo "zypper install libsqlite3-0 sqlite3-devel"
        echo
        echo "On Fedora, please install as follows:"
        echo "dnf install sqlite-devel"
        echo
        echo "afterwards, restart autoinstall_framework.sh"
        echo
      fi
      if [[ "$missing_packages" =~ .*"tbb".* ]]; then
        echo
        echo "The tbb development package is not available which may not be part of the above installation instructions"
        echo
        echo "On Debian or Ubuntu, please install as follows:"
        echo "sudo apt install libtbb-dev"
        echo
        echo "On OpenSuSE, please install as follows:"
        echo "zypper install tbb-devel"
        echo
        echo "On Fedora, please install as follows:"
        echo "dnf install tbb-devel"
        echo
        echo "afterwards, restart autoinstall_framework.sh"
        echo
      fi
      if [[ "$missing_packages" =~ .*"gsl".* ]]; then
        echo
        echo "The gsl development package is not available which may not be part of the above installation instructions"
        echo
        echo "On Debian or Ubuntu, please install as follows:"
        echo "sudo apt install libgsl-dev"
        echo
        echo "On OpenSuSE, please install as follows:"
        echo "zypper install gsl-devel"
        echo
        echo "On Fedora, please install as follows:"
        echo "dnf install gsl-devel"
        echo
        echo "afterwards, restart autoinstall_framework.sh"
        echo
      fi
    fi
    exit
  fi
  echo
  echo "All needed external software is installed"
  echo
}

check_executable()
{
  # check if executable is available
  # pass result into a temporary variable to have a quite mode
  tmp=$(which $1)
  if [ $? -ne 0 ]; then
    nof_missing_packages=$((nof_missing_packages+1))
    missing_packages="$1, $missing_packages"
  fi
}

check_file_exist()
{
  file=$1
  package=$2

  if [ ! -f $file ] ; then
    nof_missing_packages=$((nof_missing_packages+1))
    missing_packages="$package, $missing_packages"
  fi
}


install_fairsoft()
{
  if [ $SETUP_FAIRSOFT -ge 1 ]; then
    echo "Setting up FairSoft ..."

    cd ..
    git clone https://github.com/FairRootGroup/FairSoft fairsoft_src_${FSOFTVER}_root${ROOTVER}
    cd fairsoft_src_${FSOFTVER}_root${ROOTVER}
    git tag -l
    git checkout -b tag_$FSOFTVER $FSOFTVER

    year=$(echo $FSOFTVER | cut -c4,5)
    if [ $year -lt 20 ]; then
      install_fairsoft_before_2020
    else
      install_fairsoft_since_2020
    fi

    cd $CBMSRCDIR
    echo "done installing FairSoft"
  fi
}

install_fairsoft_before_2020()
{
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
  result=$?
  if [ $result -ne 0 ]; then
    echo
    echo "Something went wrong with the installation."
    echo "Please check the output above."
    exit
  fi

  cd $CBMSRCDIR
}


install_fairsoft_since_2020()
{
  FSOFTINSTALLPATH=`pwd | sed s/fairsoft_src_/fairsoft_/`

  cmake -S $PWD -B $PWD/build -C $PWD/FairSoftConfig.cmake -DCMAKE_INSTALL_PREFIX=$FSOFTINSTALLPATH/installation
  if [ $? -ne 0 ]; then
    echo "Something went wrong with the FairSoft configuration"
    exit
  fi

  cmake --build $PWD/build -j$NUMOFCPU
  if [ $? -ne 0 ]; then
    echo "Something went wrong with the FairSoft build"
    exit
  fi
}

install_fairroot()
{
  if [ $SETUP_FAIRROOT -ge 1 ]; then
    echo "Setting up FairRoot ..."
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
    git checkout -b tag_$FROOTVER $FROOTVER
    mkdir -p build
    cd build
    cmake \
      -DCMAKE_CXX_COMPILER=$($SIMPATH/bin/fairsoft-config --cxx) \
      -DCMAKE_C_COMPILER=$($SIMPATH/bin/fairsoft-config --cc) \
      -DCMAKE_INSTALL_PREFIX=../../fairroot_$FROOTVER-fairsoft_${FSOFTVER}_root${ROOTVER} \
      ..
    nice make install -j$NUMOFCPU

    cd $CBMSRCDIR
    echo "done installing FairRoot"
  fi
}

install_cbmroot()
{
  if [ ${SETUP_CBMROOT} -eq "1" ]; then
    echo "Setting up CbmRoot ..."

    cd ..
    echo "SIMPATH	before: $SIMPATH"
    cd fairsoft_${FSOFTVER}_root${ROOTVER}/installation/
    export SIMPATH=`pwd`
    echo "SIMPATH	now   : $SIMPATH"
    cd $CBMSRCDIR

    cd ..
    cd fairroot_$FROOTVER-fairsoft_${FSOFTVER}_root${ROOTVER}
    export FAIRROOTPATH=`pwd`
    echo "FAIRROOTPATH: $FAIRROOTPATH"
    cd $CBMSRCDIR

    echo PATH=$SIMPATH/bin:$PATH
    export PATH=$SIMPATH/bin:$PATH

    cd ..

    cd $CBMSRCDIR
    mkdir -p build
    cd build
    cmake \
      -DCMAKE_CXX_COMPILER=$($SIMPATH/bin/fairsoft-config --cxx) \
      -DCMAKE_C_COMPILER=$($SIMPATH/bin/fairsoft-config --cc) \
      ..

    nice make -j$NUMOFCPU
    if [ $? -ne 0 ]; then
      echo "Something went wrong with the CbmRoot installation"
      exit
    fi

    cd ..

    echo "done installing CbmRoot"

    [ -z $ANSWER ] && (
  cat << EOT
Since the system is now installed. Shall I switch to the new environment?
source build/config.sh
Reply Yes or Y for confirmation ????
EOT
    ) && read ANSWER

    if (echo "$ANSWER" | sed -n '/^\(Y\|y\)/!{q10}'); then
      echo "A yes detected."
      source build/config.sh;
      echo "Environmental variables and paths updated"
    fi
  fi
}

#####################################################################################

main "$@"
