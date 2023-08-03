# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/heinemann/gpuunpacker/cbmroot

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/heinemann/gpuunpacker/cbmroot/build2

# Include any dependencies generated for this target.
include reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/compiler_depend.make

# Include the progress variables for this target.
include reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/progress.make

# Include the compile flags for this target's objects.
include reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/flags.make

reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.o: reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/flags.make
reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.o: ../reco/tracking/global/CbmGlobalTrackingTask.cxx
reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.o: reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.o -MF CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.o.d -o CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.o -c /home/heinemann/gpuunpacker/cbmroot/reco/tracking/global/CbmGlobalTrackingTask.cxx

reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/reco/tracking/global/CbmGlobalTrackingTask.cxx > CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.i

reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/reco/tracking/global/CbmGlobalTrackingTask.cxx -o CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.s

reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.o: reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/flags.make
reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.o: ../reco/tracking/global/CbmGlobalTrackingQa.cxx
reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.o: reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.o -MF CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.o.d -o CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.o -c /home/heinemann/gpuunpacker/cbmroot/reco/tracking/global/CbmGlobalTrackingQa.cxx

reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/reco/tracking/global/CbmGlobalTrackingQa.cxx > CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.i

reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/reco/tracking/global/CbmGlobalTrackingQa.cxx -o CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.s

reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.o: reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/flags.make
reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.o: ../reco/tracking/global/CbmGlobalTrackingTof.cxx
reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.o: reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.o -MF CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.o.d -o CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.o -c /home/heinemann/gpuunpacker/cbmroot/reco/tracking/global/CbmGlobalTrackingTof.cxx

reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/reco/tracking/global/CbmGlobalTrackingTof.cxx > CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.i

reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/reco/tracking/global/CbmGlobalTrackingTof.cxx -o CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.s

# Object files for target CbmGlobalTracking
CbmGlobalTracking_OBJECTS = \
"CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.o" \
"CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.o" \
"CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.o"

# External object files for target CbmGlobalTracking
CbmGlobalTracking_EXTERNAL_OBJECTS = \
"/home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global/CMakeFiles/G__CbmGlobalTracking.dir/G__CbmGlobalTracking.cxx.o"

lib/libCbmGlobalTracking.so: reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTask.cxx.o
lib/libCbmGlobalTracking.so: reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingQa.cxx.o
lib/libCbmGlobalTracking.so: reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/CbmGlobalTrackingTof.cxx.o
lib/libCbmGlobalTracking.so: reco/tracking/global/CMakeFiles/G__CbmGlobalTracking.dir/G__CbmGlobalTracking.cxx.o
lib/libCbmGlobalTracking.so: reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/build.make
lib/libCbmGlobalTracking.so: lib/libLittrack.so
lib/libCbmGlobalTracking.so: lib/libKFParticleInterface.so
lib/libCbmGlobalTracking.so: lib/libKF.so
lib/libCbmGlobalTracking.so: lib/libCbmRecoBase.so
lib/libCbmGlobalTracking.so: lib/libLittrackparallel.so
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libVc.a
lib/libCbmGlobalTracking.so: lib/libCbmMuchSim.so
lib/libCbmGlobalTracking.so: lib/libCbmMuchBase.so
lib/libCbmGlobalTracking.so: lib/libCbmBase.so
lib/libCbmGlobalTracking.so: lib/libCbmData.so
lib/libCbmGlobalTracking.so: external/flesnet-prefix/src/flesnet-build/lib/fles_ipc/libfles_ipc.a
lib/libCbmGlobalTracking.so: external/flesnet-prefix/src/flesnet-build/src/zeromq-build/lib/libzmq.a
lib/libCbmGlobalTracking.so: external/flesnet-prefix/src/flesnet-build/lib/logging/liblogging.a
lib/libCbmGlobalTracking.so: external/flesnet-prefix/src/flesnet-build/lib/monitoring/libmonitoring.a
lib/libCbmGlobalTracking.so: external/flesnet-prefix/src/flesnet-build/_deps/fmtlib-build/libfmt.a
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_log.so.1.78.0
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_chrono.so.1.78.0
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_filesystem.so.1.78.0
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_atomic.so.1.78.0
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_regex.so.1.78.0
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_thread.so.1.78.0
lib/libCbmGlobalTracking.so: lib/libxpu.so
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libBase.so
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libGeoBase.so
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libAlignment.so
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_serialization.so.1.78.0
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libMbsAPI.so
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libRHTTP.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGeomPainter.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libProof.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libVMCLibrary.so
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGdml.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libXMLIO.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libParBase.so
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGeom.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libFairTools.so
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libEG.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libPhysics.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libFairLogger.so.1.11.0
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libfmt.a
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libTree.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf3d.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGpad.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libNet.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libHist.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libMatrix.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libRIO.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libThread.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libMathCore.so.6.26.02
lib/libCbmGlobalTracking.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libCore.so.6.26.02
lib/libCbmGlobalTracking.so: reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX shared library ../../../lib/libCbmGlobalTracking.so"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CbmGlobalTracking.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/build: lib/libCbmGlobalTracking.so
.PHONY : reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/build

reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/clean:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global && $(CMAKE_COMMAND) -P CMakeFiles/CbmGlobalTracking.dir/cmake_clean.cmake
.PHONY : reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/clean

reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/depend:
	cd /home/heinemann/gpuunpacker/cbmroot/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heinemann/gpuunpacker/cbmroot /home/heinemann/gpuunpacker/cbmroot/reco/tracking/global /home/heinemann/gpuunpacker/cbmroot/build2 /home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global /home/heinemann/gpuunpacker/cbmroot/build2/reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : reco/tracking/global/CMakeFiles/CbmGlobalTracking.dir/depend

