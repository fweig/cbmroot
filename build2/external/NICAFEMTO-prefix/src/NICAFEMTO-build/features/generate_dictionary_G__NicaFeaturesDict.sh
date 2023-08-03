#!/bin/bash

# This macro is used to generate the ROOT dictionaries
# To use the ctest launchers one needs some environment variables which
# are only present when running CMake. To have the same environment at
# the time the dictionary is build this script is used which is build
# at the time cmake runs.

# Setup the needed environment
export LD_LIBRARY_PATH=/home/cbmdata/_deps/fairsoft_apr22/install/lib::/usr/local/cuda/lib64::/home/heinemann/gpuunpacker/cbmroot/build/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:/home/heinemann/gpuunpacker/cbmroot/build/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib
export DYLD_LIBRARY_PATH=/home/cbmdata/_deps/fairsoft_apr22/install/lib::/usr/local/cuda/lib64::/home/heinemann/gpuunpacker/cbmroot/build/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:/home/heinemann/gpuunpacker/cbmroot/build/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib
export ROOTSYS=

/home/cbmdata/_deps/fairsoft_apr22/install/bin/rootcling -f /home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/features/G__NicaFeaturesDict.cxx -rmf /home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/libNicaFeatures.rootmap -rml NicaFeatures.so  -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/hist -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/io/html -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/std -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/minimizer -I/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/minimizer -I/home/cbmdata/_deps/fairsoft_apr22/install/include/root -I/home/cbmdata/_deps/fairroot_v18.6.9/install/include -I/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc -I/home/cbmdata/_deps/fairsoft_apr22/install/include/root -I/home/cbmdata/_deps/fairsoft_apr22/install/include NicaCout.h NicaArray.h NicaOptions.h NicaParameter.h NicaStd.h NicaObjectMatrix.h NicaHelix.h NicaHelixBase.h NicaLink.h NicaConst.h NicaXMLNode.h NicaJobs.h NicaPack.h NicaPackage.h NicaPackage2HTML.h NicaAnaFile.h NicaHtmlCore.h NicaPackageSql.h NicaHtmlDiv.h NicaHtmlFile.h NicaHtmlObject.h NicaHtmlTable.h NicaDividedHisto.h NicaHistogramManager.h NicaMultiGraph.h NicaSplines.h NicaChiSqMap2D.h NicaStdHist.h NicaStdString.h NicaStdMath.h NicaFitParam.h NicaMinimizer.h NicaFeaturesLinkDef.h
