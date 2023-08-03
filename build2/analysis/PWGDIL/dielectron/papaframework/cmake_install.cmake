# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "FullWarnings")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibrariesx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmPapa_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmPapa.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmPapa.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmPapa.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmPapa.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmPapa.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmPapa.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmPapa.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmPapa.so"
         OLD_RPATH "/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmPapa.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/AnalysisCuts.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/AnalysisFilter.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisEvent.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisPair.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisPairKF.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisPairLV.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisHelper.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisStyler.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisCutQa.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisVarCuts.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisObjectCuts.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisPairLegCuts.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisCutCombi.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisCutGroup.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisVarManager.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisMetaData.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/AnalysisTaskMultiPairAnalysis.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysis.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisHF.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisHistos.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisMC.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisSignalMC.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisTrackRotator.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisMixedEvent.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisMixingHandler.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisFunction.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisSignalExt.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/PairAnalysisSpectrum.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmPapa.rootmap"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmPapa_rdict.pcm"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/papa" TYPE FILE FILES "/home/heinemann/gpuunpacker/cbmroot/analysis/PWGDIL/dielectron/papaframework/configs/Config_trd_PidLI.C")
endif()

