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
include core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/compiler_depend.make

# Include the progress variables for this target.
include core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/progress.make

# Include the compile flags for this target's objects.
include core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/flags.make

core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.o: core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/flags.make
core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.o: ../core/data/test/psd/_GTestCbmPsdDigi.cxx
core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.o: core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/psd && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.o -MF CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.o.d -o CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.o -c /home/heinemann/gpuunpacker/cbmroot/core/data/test/psd/_GTestCbmPsdDigi.cxx

core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/psd && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/core/data/test/psd/_GTestCbmPsdDigi.cxx > CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.i

core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/psd && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/core/data/test/psd/_GTestCbmPsdDigi.cxx -o CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.s

# Object files for target _GTestCbmPsdDigi
_GTestCbmPsdDigi_OBJECTS = \
"CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.o"

# External object files for target _GTestCbmPsdDigi
_GTestCbmPsdDigi_EXTERNAL_OBJECTS =

bin/_GTestCbmPsdDigi: core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/_GTestCbmPsdDigi.cxx.o
bin/_GTestCbmPsdDigi: core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/build.make
bin/_GTestCbmPsdDigi: lib/libCbmData.so
bin/_GTestCbmPsdDigi: lib/libgtest.a
bin/_GTestCbmPsdDigi: lib/libgtest_main.a
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libBase.so
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libAlignment.so
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libParBase.so
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libFairTools.so
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libGeoBase.so
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairroot_v18.6.9/install/lib/libMbsAPI.so
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libRHTTP.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGeomPainter.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libProof.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGdml.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libXMLIO.so.6.26.02
bin/_GTestCbmPsdDigi: external/flesnet-prefix/src/flesnet-build/lib/fles_ipc/libfles_ipc.a
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_serialization.so.1.78.0
bin/_GTestCbmPsdDigi: external/flesnet-prefix/src/flesnet-build/src/zeromq-build/lib/libzmq.a
bin/_GTestCbmPsdDigi: external/flesnet-prefix/src/flesnet-build/lib/logging/liblogging.a
bin/_GTestCbmPsdDigi: external/flesnet-prefix/src/flesnet-build/lib/monitoring/libmonitoring.a
bin/_GTestCbmPsdDigi: external/flesnet-prefix/src/flesnet-build/_deps/fmtlib-build/libfmt.a
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_log.so.1.78.0
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_chrono.so.1.78.0
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_filesystem.so.1.78.0
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_atomic.so.1.78.0
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_regex.so.1.78.0
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_thread.so.1.78.0
bin/_GTestCbmPsdDigi: lib/libxpu.so
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libVMCLibrary.so
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libEG.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libPhysics.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf3d.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGpad.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGeom.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libHist.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libMatrix.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libTree.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libMathCore.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libNet.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libRIO.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libThread.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libCore.so.6.26.02
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libFairLogger.so.1.11.0
bin/_GTestCbmPsdDigi: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libfmt.a
bin/_GTestCbmPsdDigi: core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../../bin/_GTestCbmPsdDigi"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/psd && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/_GTestCbmPsdDigi.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/build: bin/_GTestCbmPsdDigi
.PHONY : core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/build

core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/clean:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/psd && $(CMAKE_COMMAND) -P CMakeFiles/_GTestCbmPsdDigi.dir/cmake_clean.cmake
.PHONY : core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/clean

core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/depend:
	cd /home/heinemann/gpuunpacker/cbmroot/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heinemann/gpuunpacker/cbmroot /home/heinemann/gpuunpacker/cbmroot/core/data/test/psd /home/heinemann/gpuunpacker/cbmroot/build2 /home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/psd /home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : core/data/test/psd/CMakeFiles/_GTestCbmPsdDigi.dir/depend

