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
include MQ/parmq/CMakeFiles/parmq-server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include MQ/parmq/CMakeFiles/parmq-server.dir/compiler_depend.make

# Include the progress variables for this target.
include MQ/parmq/CMakeFiles/parmq-server.dir/progress.make

# Include the compile flags for this target's objects.
include MQ/parmq/CMakeFiles/parmq-server.dir/flags.make

MQ/parmq/CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.o: MQ/parmq/CMakeFiles/parmq-server.dir/flags.make
MQ/parmq/CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.o: ../MQ/parmq/ParameterMQServer.cxx
MQ/parmq/CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.o: MQ/parmq/CMakeFiles/parmq-server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object MQ/parmq/CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/parmq && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT MQ/parmq/CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.o -MF CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.o.d -o CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.o -c /home/heinemann/gpuunpacker/cbmroot/MQ/parmq/ParameterMQServer.cxx

MQ/parmq/CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/parmq && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/MQ/parmq/ParameterMQServer.cxx > CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.i

MQ/parmq/CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/parmq && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/MQ/parmq/ParameterMQServer.cxx -o CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.s

MQ/parmq/CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.o: MQ/parmq/CMakeFiles/parmq-server.dir/flags.make
MQ/parmq/CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.o: ../MQ/parmq/runParameterMQServer.cxx
MQ/parmq/CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.o: MQ/parmq/CMakeFiles/parmq-server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object MQ/parmq/CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/parmq && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT MQ/parmq/CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.o -MF CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.o.d -o CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.o -c /home/heinemann/gpuunpacker/cbmroot/MQ/parmq/runParameterMQServer.cxx

MQ/parmq/CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/parmq && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/MQ/parmq/runParameterMQServer.cxx > CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.i

MQ/parmq/CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/parmq && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/MQ/parmq/runParameterMQServer.cxx -o CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.s

# Object files for target parmq-server
parmq__server_OBJECTS = \
"CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.o" \
"CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.o"

# External object files for target parmq-server
parmq__server_EXTERNAL_OBJECTS =

bin/MQ/parmq/parmq-server: MQ/parmq/CMakeFiles/parmq-server.dir/ParameterMQServer.cxx.o
bin/MQ/parmq/parmq-server: MQ/parmq/CMakeFiles/parmq-server.dir/runParameterMQServer.cxx.o
bin/MQ/parmq/parmq-server: MQ/parmq/CMakeFiles/parmq-server.dir/build.make
bin/MQ/parmq/parmq-server: lib/libCbmMQBase.so
bin/MQ/parmq/parmq-server: lib/libCbmSimSteer.so
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libFairMQ.so.1.4.50
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_container.so.1.78.0
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_program_options.so.1.78.0
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libFairMQStateMachine.so.1.4.50
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libFairMQTools.so.1.4.50
bin/MQ/parmq/parmq-server: lib/libCbmGeoSetup.so
bin/MQ/parmq/parmq-server: lib/libCbmSimBase.so
bin/MQ/parmq/parmq-server: lib/libCbmData.so
bin/MQ/parmq/parmq-server: external/flesnet-prefix/src/flesnet-build/lib/fles_ipc/libfles_ipc.a
bin/MQ/parmq/parmq-server: external/flesnet-prefix/src/flesnet-build/src/zeromq-build/lib/libzmq.a
bin/MQ/parmq/parmq-server: external/flesnet-prefix/src/flesnet-build/lib/logging/liblogging.a
bin/MQ/parmq/parmq-server: external/flesnet-prefix/src/flesnet-build/lib/monitoring/libmonitoring.a
bin/MQ/parmq/parmq-server: external/flesnet-prefix/src/flesnet-build/_deps/fmtlib-build/libfmt.a
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_log.so.1.78.0
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_regex.so.1.78.0
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_chrono.so.1.78.0
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_thread.so.1.78.0
bin/MQ/parmq/parmq-server: lib/libxpu.so
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_filesystem.so.1.78.0
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_atomic.so.1.78.0
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libBase.so
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libParBase.so
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libAlignment.so
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libFairTools.so
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libFairLogger.so.1.11.0
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libfmt.a
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libGeoBase.so
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_serialization.so.1.78.0
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libMbsAPI.so
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libRHTTP.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGeomPainter.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libProof.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGdml.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libXMLIO.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libVMCLibrary.so
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libTree.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libNet.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGeom.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libEG.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libPhysics.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf3d.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGpad.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libHist.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libRIO.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libThread.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libMatrix.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libMathCore.so.6.26.02
bin/MQ/parmq/parmq-server: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libCore.so.6.26.02
bin/MQ/parmq/parmq-server: MQ/parmq/CMakeFiles/parmq-server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../bin/MQ/parmq/parmq-server"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/parmq && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/parmq-server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
MQ/parmq/CMakeFiles/parmq-server.dir/build: bin/MQ/parmq/parmq-server
.PHONY : MQ/parmq/CMakeFiles/parmq-server.dir/build

MQ/parmq/CMakeFiles/parmq-server.dir/clean:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/MQ/parmq && $(CMAKE_COMMAND) -P CMakeFiles/parmq-server.dir/cmake_clean.cmake
.PHONY : MQ/parmq/CMakeFiles/parmq-server.dir/clean

MQ/parmq/CMakeFiles/parmq-server.dir/depend:
	cd /home/heinemann/gpuunpacker/cbmroot/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heinemann/gpuunpacker/cbmroot /home/heinemann/gpuunpacker/cbmroot/MQ/parmq /home/heinemann/gpuunpacker/cbmroot/build2 /home/heinemann/gpuunpacker/cbmroot/build2/MQ/parmq /home/heinemann/gpuunpacker/cbmroot/build2/MQ/parmq/CMakeFiles/parmq-server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : MQ/parmq/CMakeFiles/parmq-server.dir/depend

