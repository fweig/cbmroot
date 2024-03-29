#!/bin/bash
# Copyright (C) 2007 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
# SPDX-License-Identifier: GPL-3.0-only
# First commited by Florian Uhlig


function print_example(){
echo "##################################################################"
echo "# To set the required parameters as source and the build         #"
echo "# directory for ctest, the linux flavour and the SIMPATH         #"
echo "# put the export commands below to a separate file which is read #"
echo "# during execution and which is defined on the command line.     #"
echo "# Set all parameters according to your needs.                    #"
echo "# LINUX_FLAVOUR should be set to the distribution you are using  #"
echo "# eg Debian, SuSe etc.                                           #"
echo "# For example                                                    #"
echo "#!/bin/bash                                                      #"
echo "#export LINUX_FLAVOUR=Wheezy64                                   #"
echo "#export FAIRSOFT_VERSION=mar15                                   #"
echo "#export SIMPATH=<path_to_installation_of_external_packages>      #"
echo "#export FAIRROOT_VERSION=v15-03                                  #"
echo "#export FAIRROOTPATH=<path_to_installation_of_external_packages> #"
echo "#export BUILDDIR=/tmp/fairroot/build_cbm_\${FAIRSOFT_VERSION}    #"
echo "#export SOURCEDIR=/home/uhlig/SVN/ctest/cbmroot                  #"
echo "##################################################################"
}

#if test  "x$SIMPATH" = "x" ; then
#  echo ""                                                             
#  echo "-- Error -- You don't set the needed variables in this shell script."
#  echo "-- Error -- Please edit the script and do so."
#  echo ""
#  exit 1
#fi

if [ "$#" -lt "2" ]; then
  echo ""
  echo "-- Error -- Please start script with two parameters"
  echo "-- Error -- The first parameter is the ctest model."
  echo "-- Error -- Possible arguments are Continuous, Nightly, Experimental or Weekly."
  echo "-- Error -- The second parameter is the file containg the"
  echo "-- Error -- Information about the setup at the client" 
  echo "-- Error -- installation (see example below)."
  echo ""
  print_example
  exit 1
fi

# test if a ctest model is either Experimental or Nightly
if [ "$1" == "Experimental" -o "$1" == "Nightly" -o "$1" == "Continuous" -o "$1" == "Profile" -o "$1" == "Weekly" -o "$1" == "MergeRequest" ]; then
  echo ""
else
  echo "-- Error -- This ctest model is not supported."
  echo "-- Error -- Possible arguments are Nightly, Experimental, Continuous, Profile or MergeRequest."
  exit 1
fi 

# test if the input file exists and execute it
if [ -e "$2" ];then
  source $2
else
  echo "-- Error -- Input file does not exist."
  echo "-- Error -- Please choose existing input file."
  exit 1  
fi

# set the ctest model to command line parameter
#export ctest_model=$1
ctest_model=$1

# test for architecture
arch=$(uname -s | tr '[A-Z]' '[a-z]')
chip=$(uname -m | tr '[A-Z]' '[a-z]')

# extract information about the system and the machine and set
# environment variables used by ctest
SYSTEM=$arch-$chip
if test -z $CXX ; then
  COMPILER=gcc;
  GCC_VERSION=$(gcc -dumpversion)
else
  COMPILER=$CXX;
  GCC_VERSION=$($CXX -dumpversion)
fi

if [ "${ctest_model}" == "Weekly" ]; then
  export LABEL1=${LINUX_FLAVOUR}-$SYSTEM-$COMPILER$GCC_VERSION-${ctest_model}-fairsoft_$FAIRSOFT_VERSION-fairroot_$FAIRROOT_VERSION
elif [ "${ctest_model}" == "MergeRequest" ]; then
  export LABEL1=${LINUX_FLAVOUR}-$SYSTEM-$COMPILER$GCC_VERSION-fairsoft_$FAIRSOFT_VERSION-fairroot_${FAIRROOT_VERSION}_MR-$CI_MERGE_REQUEST_IID
else
  export LABEL1=${LINUX_FLAVOUR}-$SYSTEM-$COMPILER$GCC_VERSION-fairsoft_$FAIRSOFT_VERSION-fairroot_$FAIRROOT_VERSION
fi
export LABEL=$(echo $LABEL1 | sed -e 's#/#_#g')

# get the number of processors
# and information about the host
if [ "$arch" = "linux" ];
then
  if [ -z $NCPU ]; then
    export number_of_processors=$(cat /proc/cpuinfo | grep processor | wc -l)
  else
    export number_of_processors=$NCPU
  fi  
  if [ -z $SITE ]; then
    export SITE=$(hostname -f)
    if [ -z $SITE ]; then
      export SITE=$(uname -n)
    fi  
  fi
elif [ "$arch" = "darwin" ];
then
  if [ -z $NCPU ]; then
    export number_of_processors=$(sysctl -n hw.ncpu)
  else
    export number_of_processors=$NCPU
  fi  
  if [ -z $SITE ]; then
    export SITE=$(hostname -s)
  fi
fi

echo "************************"
date
echo "LABEL: " $LABEL
echo "SITE: " $SITE
echo "Model: " ${ctest_model}
echo "Nr. of processes: " $number_of_processors
echo "************************"

cd $SOURCEDIR

ctest -S $SOURCEDIR/CbmRoot_test.cmake -V --VV -DCBM_TEST_MODEL=${ctest_model}
#ctest -S $SOURCEDIR/CbmRoot_test.cmake -V --VV
