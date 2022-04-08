Installation Instructions for for CbmRoot
=========================================

The easiest way to install CbmRoot is described in Chapter 3 which
introduces a scripts for automatic installation of CbmRoot and its
dependencies. If you are unsure how to install CbmRoot please follow
the instructions in this chapter.

# 1. Dependencies


## 1.1 Compiler

To be able to compile CbmRoot, the used compiler must support the C++17 standard.
A GCC beginning from version 7 and clang beginning from version 6 should be okay.


## 1.2 FairSoft

To install, compile, and run CbmRoot, several external programs are required.
These are for example Root, Geant3 or Geant4 and many others. Installing these
is the first step of the CbmRoot installation. As the individual installation of
each of these programs is very time consuming, the FairRoot team provides a
software bundle known as FairSoft, which contains all the needed packages, and also
provides tools for its automated installation on several platforms. The most up-to-date
information regarding the installation of FairSoft, together with a complete list of
the contained packages, can be found on the project's GitHub page:

  https://github.com/FairRootGroup/FairSoft

This page should be considered the primary source of information regarding FairSoft,
which takes precedence over any others (which may in some cases be outdated).
Additional information, for instance regarding older versions, can be found on
CBM Redmine:

  https://redmine.cbm.gsi.de/projects/cbmroot/wiki/Install_External_Packages

We recommend to always use the latest FairSoft release version. Currently this is
"apr21p2", which is compatible with the latest version of CbmRoot. Different
combinations of older versions of FairSoft, FairRoot and CbmRoot may work in
principle, but are not endorsed or actively supported.

Please make sure SQLite is installed on your system before compiling
FairSoft, as this is required by CbmRoot. Failing to compile FairSoft
with SQLite support will lead to a crash during the CbmRoot compilation
which results in a complete reinstallation of FairSoft and FairRoot after
the package was installed.
There have also been reports about problems with other missing packages.
Mainly these were the Threading Building Blocks (tbb) and the GNU Scientific
Library.
Since these packages may be missing in the instructions from the FairRoot
webpage please install them using the following commands depending on your
OS.

Debian/Ubuntu:

    sudo apt install sqlite3 libsqlite3-dev
    sudo apt install libgsl-dev
    sudo apt install libtbb-dev

OpenSuse:

    sudo zypper install sqlite3 sqlite3-devel
    sudo zypper install gsl-devel
    sudo zypper install tbb-devel

Fedora:

    sudo dnf install sqlite-devel
    sudo dnf install gsl-devel
    sudo dnf install tbb-devel

The complete installation of FairSoft (and likewise FairRoot and CbmRoot) doesn't
need any administrative privileges and an installation into system paths is strongly
discouraged, as this may create conflicts with existing software.
So please make sure that you do the installation as normal user.

For an installation which is to be shared between users on a MacOS or linux system
it is recommended to use the opt/ folder.
Consider also using the auto installer (see section 3).

The compilation of FairSoft is the most time-consuming part of the CbmRoot installation
and can take several hours, depending on your system. If you are working at GSI there are
always ready-to-use versions of FairSoft available. For detailed information please check
the following wiki page

  https://redmine.cbm.gsi.de/projects/cbmroot/wiki/RunCbmGSI

## 1.3 FairRoot

CbmRoot is based on FairRoot, a simulation, reconstruction and analysis
framework based on the ROOT system, which by now is also used by several other experiments
as base for their developments. Beside FairSoft you need also to install FairRoot to be able
to compile CbmRoot.

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

    git clone https://git.cbm.gsi.de/computing/cbmroot

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
a Bash shell, you can  add the following line to .bashrc to further automate
this step:

    source ~/build_cbmroot/config.sh > /dev/null 2>&1

The problem with this automatisation is that it frequently results in
problems if you use more than one combination of FairSoft/FairRoot/Cbmroot.
If this isn't the case for you please proceed but keep in mind to unset this
automatism before installing any new versions of FairSoft, FairRoot.

# 3. Automatic installation

To make the installation more easy we provide a script which automatically
compiles and installs FairSoft, FairRoot, and CbmRoot in one go. The script
always combines the correct FairSoft and FairRoot versions. The full description
of this process is at

  https://redmine.cbm.gsi.de/projects/cbmroot/wiki/InstallCbmRootAuto

To get the script and the needed CbmRoot source code to your local computer
you need to clone the CbmRoot repository from out Git repository using:

    git clone https://git.cbm.gsi.de/computing/cbmroot

The needed skript is then available in the CbmRoot source directory and can
be executed from there by a one-line command

    ./autoinstall_framework.sh --fairsoft --fairroot --cbmroot

The command automatically install the defined production versions of
FairSoft and FairRoot. If you want to check for the avaible options you can
get the complete list with

    ./autoinstall_framework.sh --help

If any problems appear and the script stops with an error message please
redo the installation step by step to see where the problem appears.

    ./autoinstall_framework.sh --fairsoft
    ./autoinstall_framework.sh --fairsoft --fairroot
    ./autoinstall_framework.sh --fairsoft --fairroot --cbmroot

The same script can be used to only install any subset of the three packages. See
the Redmine page or the script itself for further details.

## 3.1 Tested systems

The current version of the script was tested for the following systems and
combinations of FairSoft and FairRoot

| OS            | FairSoft   | FairRoot |
|---------------|------------|----------|
| Ubuntu 20.04  | apr21p2    | v18.6.7  |
| OpenSuse 15.4 | apr21p2    | v18.6.7  |
| Fedora 35     | apr21p2    | v18.6.7  |

# 4. Recommended versions

GCC  : >= 7
  or
Clang: >= 6

FairSoft: apr21p2

FairRoot: v18.6.7

# 5. Further remarks

Compiling CbmRoot with multicore support using "make -j", without
specifying the number of threads, is known to cause MacOS and Ubuntu
20.04 LTS systems to freeze. Please explicitly choose a number of cores
which is suitable for your system.
