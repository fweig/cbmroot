# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/external/KFParticle

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
    set(CMAKE_INSTALL_CONFIG_NAME "RELWITHDEBINFO")
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
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFParticleBase.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFParticle.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFVertex.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticlePerformance/KFPartEfficiencies.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticlePerformance/KFMCParticle.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticleTest/KFParticleTest.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFParticleTopoReconstructor.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFParticlePVReconstructor.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFPVertex.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFPTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFParticleFinder.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFParticleSIMD.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFParticleBaseSIMD.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFParticleDef.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFPSimdAllocator.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFPTrackVector.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFPEmcCluster.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFParticleField.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFPTrackVector.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticle/KFParticleDatabase.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticlePerformance/KFTopoPerformance.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticlePerformance/KFParticlePerformanceBase.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticlePerformance/KFMCCounter.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticlePerformance/KFPVEfficiencies.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticlePerformance/KFMCVertex.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticlePerformance/KFMCTrack.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/KFParticle/KFParticlePerformance/KFPartMatch.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE OPTIONAL FILES "/home/heinemann/gpuunpacker/cbmroot/build2/external/KFPARTICLE-prefix/src/KFPARTICLE-build/libKFParticle_rdict.pcm")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "/home/heinemann/gpuunpacker/cbmroot/build2/external/KFPARTICLE-prefix/src/KFPARTICLE-build/libKFParticle.rootmap")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libKFParticle.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libKFParticle.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libKFParticle.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/external/KFPARTICLE-prefix/src/KFPARTICLE-build/libKFParticle.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libKFParticle.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libKFParticle.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libKFParticle.so"
         OLD_RPATH "/home/cbmdata/_deps/fairsoft_apr22/install/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libKFParticle.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/heinemann/gpuunpacker/cbmroot/build2/external/KFPARTICLE-prefix/src/KFPARTICLE-build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
