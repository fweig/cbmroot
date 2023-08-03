# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018

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
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmFlibMcbm2018_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmFlibMcbm2018.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmFlibMcbm2018.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmFlibMcbm2018.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmFlibMcbm2018.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmFlibMcbm2018.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmFlibMcbm2018.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmFlibMcbm2018.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmFlibMcbm2018.so"
         OLD_RPATH "/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmFlibMcbm2018.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/CbmMcbmUnpack.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/CbmMcbm2018Source.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/CbmMcbm2018MsCrcCheck.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018RawConverterSdpb.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018RawConverterGdpb.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmCriGet4RawPrint.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerAlgoSts.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerTaskSts.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerAlgoMuch.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerTaskMuch.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerAlgoTof.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerTaskTof.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerAlgoRich.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerTaskRich.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerTaskRich2020.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerAlgoRich2020.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerUtilRich2020.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerAlgoPsd.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerTaskPsd.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerAlgoTrdR.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/unpacker/CbmMcbm2018UnpackerTaskTrdR.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmCheckDataFormatGdpb2018.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorDataRates.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorAlgoT0.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorTaskT0.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorMcbmRate.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorMcbmSync.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorMcbmPulser.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorSts.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorMuch.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorMuchLite.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorAlgoMuchLite.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorTaskMuchLite.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorTof.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorTofPulser.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorAlgoTof.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorTaskTof.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorAlgoTofPulser.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorTaskTofPulser.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018TofTestFee.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018TofFeeThr.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorAlgoPsd.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/monitor/CbmMcbm2018MonitorTaskPsd.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmCheckDigisNbCorr.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmCheckTiming.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmCheckEvents.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmMcbm2018EventBuilder.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmMcbm2019CheckPulser.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmMcbm2019CheckDigisMuch.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmMcbm2019CheckDigisSts.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmMcbm2019CheckDtInDet.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmMcbm2019CheckTimingPairs.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmMcbm2019TimeWinEventBuilderAlgo.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmMcbm2019TimeWinEventBuilderTask.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmMcbmCheckTimingAlgo.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmMcbmCheckTimingTask.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmMcbmSpillFindAlgo.h"
    "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/tasks/CbmMcbmSpillFindTask.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmFlibMcbm2018.rootmap"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmFlibMcbm2018_rdict.pcm"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/home/heinemann/gpuunpacker/cbmroot/fles/mcbm2018/commonMQ/CbmStar2019Algo.h")
endif()

