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
include MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/compiler_depend.make

# Include the progress variables for this target.
include MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/progress.make

# Include the compile flags for this target's objects.
include MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/flags.make

MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.o: MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/flags.make
MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.o: ../MQ/mcbm/CbmDeviceMcbmEventBuilderWin.cxx
MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.o: MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/mcbm && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.o -MF CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.o.d -o CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.o -c /home/heinemann/gpuunpacker/cbmroot/MQ/mcbm/CbmDeviceMcbmEventBuilderWin.cxx

MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/mcbm && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/MQ/mcbm/CbmDeviceMcbmEventBuilderWin.cxx > CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.i

MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/mcbm && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/MQ/mcbm/CbmDeviceMcbmEventBuilderWin.cxx -o CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.s

MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.o: MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/flags.make
MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.o: ../MQ/mcbm/runMcbmEvtBuilderWin.cxx
MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.o: MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/mcbm && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.o -MF CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.o.d -o CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.o -c /home/heinemann/gpuunpacker/cbmroot/MQ/mcbm/runMcbmEvtBuilderWin.cxx

MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/mcbm && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/MQ/mcbm/runMcbmEvtBuilderWin.cxx > CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.i

MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/mcbm && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/MQ/mcbm/runMcbmEvtBuilderWin.cxx -o CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.s

# Object files for target McbmEventBuilderWin
McbmEventBuilderWin_OBJECTS = \
"CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.o" \
"CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.o"

# External object files for target McbmEventBuilderWin
McbmEventBuilderWin_EXTERNAL_OBJECTS =

bin/MQ/mcbm/McbmEventBuilderWin: MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/CbmDeviceMcbmEventBuilderWin.cxx.o
bin/MQ/mcbm/McbmEventBuilderWin: MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/runMcbmEvtBuilderWin.cxx.o
bin/MQ/mcbm/McbmEventBuilderWin: MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/build.make
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmFlibMcbm2018.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmMQBase.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmBmonReco.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmMuchReco.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmPsdReco.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmRichReco.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmTofReco.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmTrdReco.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmEventBuilder.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libflib_dpb.so
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libFairMQ.so.1.4.50
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_container.so.1.78.0
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libFairMQStateMachine.so.1.4.50
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libFairMQTools.so.1.4.50
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmPsdBase.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libLittrack.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libLittrackparallel.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libL1.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmRecoSts.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmFlibFlesTools.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libAlgo.so
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_program_options.so.1.78.0
bin/MQ/mcbm/McbmEventBuilderWin: lib/libOnlineData.so
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGenVector.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: external/flesnet-prefix/src/flesnet-build/lib/logging/liblogging.a
bin/MQ/mcbm/McbmEventBuilderWin: lib/libKF.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmSimSteer.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmGeoSetup.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmRichBase.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmSimBase.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmTrdBase.so
bin/MQ/mcbm/McbmEventBuilderWin: /usr/lib/gcc/x86_64-linux-gnu/11/libgomp.so
bin/MQ/mcbm/McbmEventBuilderWin: /usr/lib/x86_64-linux-gnu/libpthread.a
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmRecoBase.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmTofBase.so
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libMinuit.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmMuchBase.so
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libVc.a
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmQaBase.so
bin/MQ/mcbm/McbmEventBuilderWin: external/yaml-cpp-prefix/src/yaml-cpp-build/libyaml-cpp.a
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmStsBase.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmBase.so
bin/MQ/mcbm/McbmEventBuilderWin: lib/libCbmData.so
bin/MQ/mcbm/McbmEventBuilderWin: external/flesnet-prefix/src/flesnet-build/lib/fles_ipc/libfles_ipc.a
bin/MQ/mcbm/McbmEventBuilderWin: external/flesnet-prefix/src/flesnet-build/src/zeromq-build/lib/libzmq.a
bin/MQ/mcbm/McbmEventBuilderWin: external/flesnet-prefix/src/flesnet-build/lib/logging/liblogging.a
bin/MQ/mcbm/McbmEventBuilderWin: external/flesnet-prefix/src/flesnet-build/lib/monitoring/libmonitoring.a
bin/MQ/mcbm/McbmEventBuilderWin: external/flesnet-prefix/src/flesnet-build/_deps/fmtlib-build/libfmt.a
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_log.so.1.78.0
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_chrono.so.1.78.0
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_regex.so.1.78.0
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_thread.so.1.78.0
bin/MQ/mcbm/McbmEventBuilderWin: lib/libxpu.so
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libBase.so
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libParBase.so
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libAlignment.so
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libFairTools.so
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_serialization.so.1.78.0
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libMbsAPI.so
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGeomPainter.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libProof.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libVMCLibrary.so
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGdml.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libXMLIO.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libGeoBase.so
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_filesystem.so.1.78.0
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_atomic.so.1.78.0
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGeom.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libFairLogger.so.1.11.0
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libfmt.a
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libMLP.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libTreePlayer.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libMultiProc.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libRHTTP.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libEG.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libPhysics.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf3d.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGpad.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libHist.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libMatrix.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libTree.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libNet.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libRIO.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libThread.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libMathCore.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libCore.so.6.26.02
bin/MQ/mcbm/McbmEventBuilderWin: MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../bin/MQ/mcbm/McbmEventBuilderWin"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/mcbm && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/McbmEventBuilderWin.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/build: bin/MQ/mcbm/McbmEventBuilderWin
.PHONY : MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/build

MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/clean:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/mcbm && $(CMAKE_COMMAND) -P CMakeFiles/McbmEventBuilderWin.dir/cmake_clean.cmake
.PHONY : MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/clean

MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/depend:
	cd /home/heinemann/gpuunpacker/cbmroot/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heinemann/gpuunpacker/cbmroot /home/heinemann/gpuunpacker/cbmroot/MQ/mcbm /home/heinemann/gpuunpacker/cbmroot/build2 /home/heinemann/gpuunpacker/cbmroot/build2/MQ/mcbm /home/heinemann/gpuunpacker/cbmroot/build2/MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : MQ/mcbm/CMakeFiles/McbmEventBuilderWin.dir/depend

