# Install script for directory: /home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base

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
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/NicaEventAna.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/NicaMemoryMapManager.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/NicaMultiTrackAna.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/NicaTrackAna.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/NicaTwoTrackAna.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/chains/NicaEventAnaChain.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/chains/NicaTrackAnaChain.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/chains/NicaTwoTrackAnaChain.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly/NicaSplitedTrackToStatusTask.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly/NicaBoostTask.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly/NicaMiniEventTask.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly/NicaCopyDataTask.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly/NicaSmearTask.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly/smear_codes/NicaSmearAlgorithm.h"
    "/home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/analysis/base/on_the_fly/smear_codes/NicaTrackSmearMomentumPercent.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/G__NicaAnaDict_rdict.pcm"
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/libNicaAna.rootmap"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaAna.so.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaAna.so.1"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/libNicaAna.so.0"
    "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/libNicaAna.so.1"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaAna.so.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaAna.so.1"
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
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaAna.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaAna.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaAna.so"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib/libNicaAna.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaAna.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaAna.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaAna.so"
         OLD_RPATH "/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libNicaAna.so")
    endif()
  endif()
endif()

