Installation Instructions for for CbmRoot 
=========================================

# 1. Dependencies

## 1.1 Compiler

To be able to compile CbmRoot, a compiler with full C++11 support is needed. 
GCC since version 4.8 and clang since version 3.4 are known to work without
problems. 

## 1.2 FairSoft

To install, compile, and run CbmRoot, several external programs are required. 
These are for example Root, Geant3 or Geant4 and many others. Installing these
is the first step of the CbmRoot installation. As the individual installation of 
each of these programs is very time consuming, the FairRoot team provides a 
software bundle known as FairSoft, which contains all the needed packages, and also 
provides tools for its automated installation on several platforms. The most up-to-date 
information regarding the installation of FairSoft, together with a complete list of 
the contained packages, can be found on the project's GitHub page:

  https://github.com/FAirRootgroup/FairSoft

This page should be considered the primary source of information regarding FairSoft,
which takes precedence over any others (which may in some cases be outdated). 
Additional information, for instance regarding older versions, can be found on 
Cbm Redmine:

  https://redmine.cbm.gsi.de/projects/cbmroot/wiki/Install_External_Packages

We recommend to always use the latest FairSoft relase version. Currently this is
"apr21p2", which is compatible with the latest version of CbmRoot. Different
combinations of older versions of FairSoft, FairRoot and CbmRoot may work in 
principle, but are not endorsed or actively supported. 

Please make sure SQLite is installed on your system before compiling 
FairSoft, as this is required by CbmRoot. Failing compile FairSoft
with SQLite support will lead to a crash during the CbmRoot compilation. 
To install SQLite on Debian/Ubuntu and OpenSuse use

Debian/Ubuntu:

    sudo apt install sqlite3 libsqlite3-dev

OpenSuse:

    sudo zypper install sqlite3 sqlite3-devel

It is currently not recommended to install FairSoft (and likewise FairRoot and
CbmRoot) with administrator privileges, as this may create conflicts with
existing software. For an installation which is to be shared between users
on a MacOS or linux system it is recommended to use the opt/ folder.
Consider also using the auto installer (see section 3). 

The compilation of FairSoft is the most time-consuming part of the CbmRoot installation
and can take several hours, depending on your system. If you are working at GSI there are 
always ready-to-use versions of FairSoft available. For detailed information please check 
the following wiki page

  https://redmine.cbm.gsi.de/projects/cbmroot/wiki/RunCbmGSI

## 1.3 FairRoot

CbmRoot is based on FairRoot, a simulation, reconstruction and analysis 
framework based on the ROOT system, which by now is used by several
experiments as base for their developments. Beside FairSoft you need also
to install FairRoot to be able to compile CbmRoot.

Detailed instructions how to get and compile FairRoot can be found on
the project's GitHub page at:

  https://github.com/FairRootGroup/FairRoot

We again recommend this official repository as the primary source of
information, and recommend using the latest release version. 

Other information can be found at: 

  https://redmine.cbm.gsi.de/projects/cbmroot/wiki/InstallFairRoot

In order to compile FairRoot, you must set the environment variable SIMPATH
to contain the path to your FairSoft installation, i.e.:

    export SIMPATH=[path_to_fairsoft]

Not setting this variable (or FAIRROOTPATH, defined below) properly is a common
source of problems. 

# 2. Installing CbmRoot

After installing FairSoft and FairRoot, to install CbmRoot first
create a local clone of the Git repository using: 
 
    git clone git@git.cbm.gsi.de:computing/cbmroot.git

Subsequently, create a build directory:

    mkdir build_cbmroot
  
To proceed, you must set the environment variables SIMPATH and FAIRROOTPATH,
to the folders of your FairSoft and FairRoot installations, i.e. 

    export SIMPATH=[path_to_fairsoft]
    export FAIRROOTPATH=[path_to_fairroot install]

From your build directory, then call

    cmake [path_to_cbmroot]
    make -j[number_of_cores]

After your build is complete, before running any part of CbmRoot from a shell, 
a large number of environment variables must be set, which contain various
library and include paths. This is done automatically by executing the "config.sh"
or "config.csh" script from your CbmRoot build directory. If you are using
a Bash shell, we recommend adding the following line to .bashrc to further automate
this step:

    source ~/build_cbmroot/config.sh > /dev/null 2>&1

# 3. Automatic installation

To make the installation more easy we provide a script which automatically
compiles and installs FairSoft, FairRoot, and CbmRoot in one go. The script
always combines the correct FairSoft and FairRoot versions. The full description 
of this process is at 

  https://redmine.cbm.gsi.de/projects/cbmroot/wiki/InstallCbmRootAuto

The one-line command which has to be executed inside the CbmRoot source
directory is

    ./autoinstall_framework.sh 1 1 1

The same scipt can be used to only install any subset of the three packages. See
the Redmine page or the script itself for further details. 

**(The auto-install script currently doesn't work with the latest version of FairSoft)**

# 4. Recommended versions

GCC  : >= 4.8
  or
Clang: >= 3.4

FairSoft: apr21p2

FairRoot: v18.6.6

# 5. Further remarks

Compiling CbmRoot with multicore support using "make -j", without 
specifying the number of threads, is known to cause MacOS and Ubuntu 
20.04 LTS systems to freeze. Please explicitly choose a number of cores 
which is suitable for your system. 
