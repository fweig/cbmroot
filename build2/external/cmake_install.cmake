# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/external

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/home/heinemann/gpuunpacker/cbmroot/external/cppzmq/zmq.hpp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/lib/fles_ipc/libfles_ipc.a"
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/lib/shm_ipc/libshm_ipc.a"
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/lib/libzmq.a"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/ArchiveDescriptor.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/InputArchive.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/InputArchiveLoop.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/InputArchiveSequence.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/MergingSource.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/Microslice.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/MicrosliceDescriptor.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/MicrosliceInputArchive.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/MicrosliceOutputArchive.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/MicrosliceSource.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/MicrosliceView.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/OutputArchive.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/OutputArchiveSequence.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/Sink.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/Source.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/StorableMicroslice.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/StorableTimeslice.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/System.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/Timeslice.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimesliceAutoSource.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimesliceCompletion.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimesliceComponentDescriptor.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimesliceDescriptor.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimesliceInputArchive.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimesliceMultiInputArchive.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimesliceMultiSubscriber.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimesliceOutputArchive.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimeslicePublisher.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimesliceReceiver.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimesliceShmWorkItem.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimesliceSource.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimesliceSubscriber.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimesliceView.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/TimesliceWorkItem.hpp"
    "/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc/Utility.hpp"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/heinemann/gpuunpacker/cbmroot/build2/external/flib_dpb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/heinemann/gpuunpacker/cbmroot/build2/external/GSL/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/heinemann/gpuunpacker/cbmroot/build2/external/xpu/cmake_install.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libKFParticle.so"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libKFParticle.rootmap"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libKFParticle_rdict.pcm"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/KFParticle" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/include/KFParticleBase.h"
    "/home/heinemann/gpuunpacker/cbmroot/build2/include/KFParticle.h"
    "/home/heinemann/gpuunpacker/cbmroot/build2/include/KFVertex.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/KFParticlePerformance" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/include/KFMCParticle.h"
    "/home/heinemann/gpuunpacker/cbmroot/build2/include/KFPartEfficiencies.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/KFParticleTest" TYPE FILE FILES "/home/heinemann/gpuunpacker/cbmroot/build2/include/KFParticleTest.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/include/KFPVertex.h"
    "/home/heinemann/gpuunpacker/cbmroot/build2/include/KFMCCounter.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaAna.so"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaAna.so.0"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaAna.so.1"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/G__NicaAnaDict_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaAna.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaCut.so"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaCut.so.0"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaCut.so.1"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/G__NicaCutDict_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaCut.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaDataFormat.so"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaDataFormat.so.0"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaDataFormat.so.1"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/G__NicaDataFormatDict_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaDataFormat.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFeatures.so"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFeatures.so.0"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFeatures.so.1"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/G__NicaFeaturesDict_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFeatures.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFemto.so"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFemto.so.0"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFemto.so.1"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/G__NicaFemtoDict_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFemto.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFlow.so"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFlow.so.0"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFlow.so.1"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/G__NicaFlowDict_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFlow.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFluct.so"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFluct.so.0"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFluct.so.1"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/G__NicaFluctDict_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaFluct.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaGen.so"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaGen.so.0"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaGen.so.1"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/G__NicaGenDict_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaGen.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaSpectra.so"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaSpectra.so.0"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaSpectra.so.1"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/G__NicaSpectraDict_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libNicaSpectra.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/bin/nica-report"
    "/home/heinemann/gpuunpacker/cbmroot/build2/bin/nica-merger"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share" TYPE DIRECTORY FILES
    "/home/heinemann/gpuunpacker/cbmroot/external/jsroot"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/nicafemto_plus"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libAnalysisTreeBase.so"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libAnalysisTreeBase_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libAnalysisTreeBase.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libAnalysisTreeInfra.so"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libAnalysisTreeInfra_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libAnalysisTreeInfra.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/include/AnalysisTree")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libAnalysisTreeQA.so"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libAnalysisTreeQA_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/lib/libAnalysisTreeQA.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/include/AnalysisTreeQA")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro" TYPE FILE FILES "/home/heinemann/gpuunpacker/cbmroot/macro/analysis/common/qa/run_analysistree_qa.C")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/heinemann/gpuunpacker/cbmroot/build2/external/bba/cmake_install.cmake")
endif()

