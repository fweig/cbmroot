Installation Instructions for for CbmRoot 
=========================================

# 1. Dependencies

## 1.1 Compiler

To be able to compile CbmRoot a compiler with full C++11 support is needed. 
GCC since version 4.8 and clang since version 3.4 are known to work without
problems. 

## 1.2 FairSoft

To install, compile, and run CbmRoot several external programs are required. 
This are for example Root, Geant3 or Geant4. The complete list can be found 
on the FairRoot webpage (fairroot.gsi.de). Since most of these programs can't 
be installed using the package manager of the used Linux distribution, or at 
least can't be installed in the required version, the FairRoot team provides 
a GIT repository with all needed packages and some scripts to automatically 
build and install these so called external packages.

Detailed instructions how to get and compile FairSoft (aka external packages) 
can be found at the following webpage

  https://redmine.cbm.gsi.de/projects/cbmroot/wiki/Install_External_Packages

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

If you are working at GSI there are always ready-to-use versions of 
FairSoft available. For detailed information please check the following 
wiki page

  https://redmine.cbm.gsi.de/projects/cbmroot/wiki/RunCbmGSI

## 1.3 FairRoot

CbmRoot is based on FairRoot, a package which meanwhile serves several
experiments as base for their developments. Beside FairSoft you need also
to install FairRoot to be able to compile CbmRoot 

Detailed instructions how to get and compile FairRoot can be found at the 
following webpage

  https://redmine.cbm.gsi.de/projects/cbmroot/wiki/InstallFairRoot

# 2. Recommended versions

GCC  : >= 4.8
  or
Clang: >= 3.4

FairSoft: jun19p2

FairRoot: v18.2.1

# 3. Automatic installation

To make the installation more easy we provide a script which automatically
gets and installs FairSoft, FairRoot, and CbmRoot in one go. The script
always take the correct FairSoft and FairRoot versions. 
The full description of this process is at 

  https://redmine.cbm.gsi.de/projects/cbmroot/wiki/InstallCbmRootAuto

The one line command which has to be executed inside the CbmRoot source
directory is

  ./autoinstall_framework.sh 1 1 1

# 4. Further remarks

Compiling CbmRoot with multicore support using "make -j", without 
specifying the number of threads is known to cause MacOS and Ubuntu 
20.04 LTS systems to freeze. Please explicitly choose a number of cores 
which is suitable for your system. 
