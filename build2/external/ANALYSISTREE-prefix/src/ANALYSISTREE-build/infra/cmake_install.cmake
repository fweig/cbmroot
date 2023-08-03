# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra

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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libAnalysisTreeInfra.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libAnalysisTreeInfra.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libAnalysisTreeInfra.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREE-prefix/src/ANALYSISTREE-build/infra/libAnalysisTreeInfra.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libAnalysisTreeInfra.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libAnalysisTreeInfra.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libAnalysisTreeInfra.so"
         OLD_RPATH "/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREE-prefix/src/ANALYSISTREE-build/core:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libAnalysisTreeInfra.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/AnalysisTree" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/SimpleCut.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/Cuts.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/Field.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/Variable.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/Task.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/AnalysisTask.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/TaskManager.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/PlainTreeFiller.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/Chain.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/ChainDrawHelper.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/Branch.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/BranchChannel.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/AnalysisEntry.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/VariantMagic.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/ToyMC.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/Utils.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTree/infra/BranchHashHelper.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE OPTIONAL FILES "/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREE-prefix/src/ANALYSISTREE-build/infra/libAnalysisTreeInfra_rdict.pcm")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE OPTIONAL FILES "/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREE-prefix/src/ANALYSISTREE-build/infra/libAnalysisTreeInfra.rootmap")
endif()

