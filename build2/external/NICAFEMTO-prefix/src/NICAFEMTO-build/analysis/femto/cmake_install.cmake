# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemtoConst.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/ana/NicaFemtoBasicAna.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/ana/NicaFemtoFreezoutsAna.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/ana/NicaFemtoDumpPairAna.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/ana/NicaFemtoEventBinsAna.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemtoDPhiDEta.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemto1DCF.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemto3DCF.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemto3DCFQinv.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemtoFastCut.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemtoFreezoutGenerator.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemtoPairKinematics.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemtoPair.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemtoMiniPair.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemtoSourceModel.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemtoConst.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/source_models/NicaFemtoSourceModelExp.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/source_models/NicaFemtoSourceModelGauss.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/source_models/NicaFemtoSourceModelGausMt.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/NicaCorrFit.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/NicaCorrFitFunc.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/NicaCorrFitMath1DCF.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/NicaCorrFitMath3DCF.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/NicaCorrFit1DCF.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/NicaCorrFit3DCF.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/NicaCorrFitKisiel.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/NicaCorrFitWielanek.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/NicaCorrFitGUI.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/NicaCorrFitHDFunc.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerators/NicaFemto1DCFAnaMap.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerators/NicaFemto1DCFAnaMapMC.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerators/NicaWielanekMapGenerator.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerators/NicaFemto1DCFAnaMapPairsDumped.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions/NicaCorrFit1DCFSquare.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions/NicaCorrFit1DCFLorentz.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions/NicaCorrFit3DCFSquare.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions/NicaCorrFit3DCFLorentz.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions/NicaCorrFit1DCFExp.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions/NicaCorrFit1DCFGauss.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions/NicaCorrFit1DCFPoly.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions/NicaCorrFit3DCFGauss.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions/NicaCorrFit3DCFGauss2.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions/NicaCorrFit3DCFGaussCross.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions/NicaCorrFit1DCFCumac.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions/NicaCorrFit3DCFBowlerSinyukov.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/fittingfunctions/NicaCorrFit3DCFBowlerSinyukovExtra.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/weights/NicaFemtoFsiParsed.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/weights/NicaFemtoWeightGeneratorLednicky2.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/weights/NicaFemtoWeightGenerator.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/weights/NicaFemtoWeightGeneratorBasic.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/weights/NicaFemtoWeightGeneratorLednicky.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/weights/NicaFemtoWeightGeneratorLednickyResidual.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/weights/NicaFemtoWeightGeneratorSupressed.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemtoCorrFunc.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/corrfunc/NicaFemtoCorrFuncPt.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/corrfunc/NicaFemtoCorrFuncKt.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/corrfunc/NicaFemtoCorrFuncAzimuthalKt.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/corrfunc/NicaFemtoCorrFuncSimple.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/imaging/NicaFemtoImaging.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/imaging/NicaFemtoImaging1D.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/imaging/NicaFemtoImaging3D.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemtoSHCF.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/base/NicaFemtoYlm.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/NicaCorrFitSHCF.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/NicaCorrFitSmear1DCF.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerator_multidim/NicaCorrFitDatabase.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerator_multidim/NicaCorrFitDumpedPairAna.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerator_multidim/NicaCorrFitInfo.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerator_multidim/NicaCorrFit3DCFMultiDim.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerator_multidim/NicaCorrFitFunctor.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerator_multidim/NicaCorrFitPairGeneratorYPt.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/femto/corrfit/mapgenerator_multidim/NicaCorrFitMapRebin.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/G__NicaFemtoDict_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/libNicaFemto.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaFemto.so.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaFemto.so.1"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/libNicaFemto.so.0"
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/libNicaFemto.so.1"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaFemto.so.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaFemto.so.1"
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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaFemto.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaFemto.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaFemto.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/libNicaFemto.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaFemto.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaFemto.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaFemto.so"
         OLD_RPATH "/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaFemto.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/nica-corrfit" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/nica-corrfit")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/nica-corrfit"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/bin/nica-corrfit")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/nica-corrfit" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/nica-corrfit")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/nica-corrfit"
         OLD_RPATH "/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/nica-corrfit")
    endif()
  endif()
endif()

