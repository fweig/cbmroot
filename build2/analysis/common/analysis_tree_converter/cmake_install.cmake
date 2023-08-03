# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter

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
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmAnalysisTreeInterface_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmAnalysisTreeInterface.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmAnalysisTreeInterface.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmAnalysisTreeInterface.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmAnalysisTreeInterface.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmAnalysisTreeInterface.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmAnalysisTreeInterface.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmAnalysisTreeInterface.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmAnalysisTreeInterface.so"
         OLD_RPATH "/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmAnalysisTreeInterface.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter/CbmConverterManager.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter/CbmConverterTask.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter/CbmSimEventHeaderConverter.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter/CbmRecEventHeaderConverter.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter/CbmStsTracksConverter.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter/CbmSimTracksConverter.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter/CbmPsdModulesConverter.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter/CbmTofHitsConverter.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter/CbmTrdTracksConverter.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter/CbmRichRingsConverter.h"
    "/home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter/CbmMatchEvents.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmAnalysisTreeInterface.rootmap"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmAnalysisTreeInterface_rdict.pcm"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/analysis_tree" TYPE FILE FILES "/home/heinemann/gpuunpacker/cbmroot/analysis/common/analysis_tree_converter/../../../macro/analysis/common/analysis_tree_converter/run_analysis_tree_maker.C")
endif()

