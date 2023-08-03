# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/core/qa

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
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmQaBase_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmQaBase.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmQaBase.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmQaBase.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmQaBase.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmQaBase.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmQaBase.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmQaBase.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmQaBase.so"
         OLD_RPATH "/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libCbmQaBase.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaConstants.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaCanvas.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaEff.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaPie.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaHist.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaTable.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaTask.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaIO.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/checker/CbmQaCheckerCore.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/checker/CbmQaCheckerFileHandler.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/checker/CbmQaCheckerHist1DHandler.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/checker/CbmQaCheckerHist2DHandler.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/checker/CbmQaCheckerObjectHandler.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/checker/CbmQaCheckerProfile1DHandler.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/checker/CbmQaCheckerObjectDB.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmQaBase.rootmap"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmQaBase_rdict.pcm"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaTask.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaCanvas.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaTable.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaHist.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaEff.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaPie.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaConstants.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/CbmQaCmpDrawer.h"
    "/home/heinemann/gpuunpacker/cbmroot/core/qa/checker/CbmQaCheckerTypedefs.h"
    )
endif()

