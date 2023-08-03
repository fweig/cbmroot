# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/heinemann/gpuunpacker/cbmroot/build2")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/NicaSubCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/NicaCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/NicaCutContainer.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/NicaCutCollection.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/NicaEventCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/NicaTrackCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/NicaTwoTrackCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/NicaCutsAndMonitors.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/eventcuts/NicaEventImpactParameterCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/eventcuts/NicaEventModuloCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/eventcuts/NicaEventMultiplicityCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/eventcuts/NicaEventPhiCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/eventcuts/NicaEventPtMaxCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/eventcuts/NicaEventVirtualCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/eventcuts/NicaEventComplexCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/eventcuts/NicaEventVertexCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/eventcuts/NicaEventBinningCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/eventcuts/NicaEventPrimaryTracksNoCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/detector/NicaTrackBasicToFCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/detector/NicaTrackToFMass2Cut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/detector/NicaTrackDCACut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/detector/NicaTrackTpcCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/detector/NicaTrackExpCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/detector/NicaTrackTpcHitCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/detector/NicaTrackTpcToFCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/detector/NicaTrackDCAPipeCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/detector/NicaTrackTpcToFThresholdlessCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/detector/NicaTrackChi2Cut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics/NicaTrackBetaCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics/NicaTrackEtaCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics/NicaTrackMassCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics/NicaTrackMtCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics/NicaTrackPCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics/NicaTrackPhiCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics/NicaTrackPtCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics/NicaTrackYCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics/NicaTrackPzCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics/NicaTrackChargeCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/kinematics/NicaTrackBoostedMomentaCuts.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/mc/NicaTrackBasicMCCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/mc/NicaTrackFreezoutCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/mc/NicaTrackMCCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/mc/NicaTrackPdgCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/mc/NicaTrackStartPositionCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/mc/NicaTrackPdgBinCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/resolution/NicaTrackDeltaMomCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/resolution/NicaTrackDeltaMomentumCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/NicaTrackOnlyPrimariesCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/NicaTrackStatusCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/NicaTrackComplexCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/NicaTrackVirtualCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/trackcuts/NicaTrackFlagCuts.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics/NicaTwoTrackEtaCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics/NicaTwoTrackKtCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics/NicaTwoTrackMinvCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics/NicaTwoTrackPhiCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics/NicaTwoTrackPtOrderCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics/NicaTwoTrackDphiDetaCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics/NicaTwoTrackPtSumCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics/NicaTwoTrackRapidityCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics/NicaTwoTrackDeltaEtaCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics/NicaPairDeltaQinvCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics/NicaTwoTrackLCMSCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/kinematics/NicaTwoTrack3DCFCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/NicaTwoTrackComplexCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/NicaTwoTrackFemtoQinvCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/detector/NicaTwoTrackSailorCowboyCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/NicaTwoTrackRadialDistanceCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/NicaTwoTrackDebugCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/mc/NicaTwoTrackFemtoPRFCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/mc/NicaPdgBinPairCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/twotrackcuts/NicaTwoTrackVirtualCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/cutmonitors/NicaCutMonitor.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/cutmonitors/NicaCutMonitorX.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/cutmonitors/NicaCutMonitorXY.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/cutmonitors/NicaCutMonitorXYZ.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/cutmonitors/NicaPropertyMonitorX.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/cutmonitors/NicaPropertyMonitorXY.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/cuts/cutmonitors/NicaPropertyMonitorXYZ.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/G__NicaCutDict_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/libNicaCut.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCut.so.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCut.so.1"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/libNicaCut.so.0"
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/libNicaCut.so.1"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCut.so.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCut.so.1"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib:"
           NEW_RPATH "")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCut.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCut.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCut.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/libNicaCut.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCut.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCut.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCut.so"
         OLD_RPATH "/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaCut.so")
    endif()
  endif()
endif()

