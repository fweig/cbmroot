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
CMAKE_SOURCE_DIR = /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build

# Include any dependencies generated for this target.
include CMakeFiles/inproc_lat.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/inproc_lat.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/inproc_lat.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/inproc_lat.dir/flags.make

CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.o: CMakeFiles/inproc_lat.dir/flags.make
CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.o: /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq/perf/inproc_lat.cpp
CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.o: CMakeFiles/inproc_lat.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.o -MF CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.o.d -o CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.o -c /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq/perf/inproc_lat.cpp

CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq/perf/inproc_lat.cpp > CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.i

CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq/perf/inproc_lat.cpp -o CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.s

# Object files for target inproc_lat
inproc_lat_OBJECTS = \
"CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.o"

# External object files for target inproc_lat
inproc_lat_EXTERNAL_OBJECTS =

bin/inproc_lat: CMakeFiles/inproc_lat.dir/perf/inproc_lat.cpp.o
bin/inproc_lat: CMakeFiles/inproc_lat.dir/build.make
bin/inproc_lat: lib/libzmq.so.5.2.4
bin/inproc_lat: CMakeFiles/inproc_lat.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/inproc_lat"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/inproc_lat.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/inproc_lat.dir/build: bin/inproc_lat
.PHONY : CMakeFiles/inproc_lat.dir/build

CMakeFiles/inproc_lat.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/inproc_lat.dir/cmake_clean.cmake
.PHONY : CMakeFiles/inproc_lat.dir/clean

CMakeFiles/inproc_lat.dir/depend:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/CMakeFiles/inproc_lat.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/inproc_lat.dir/depend

