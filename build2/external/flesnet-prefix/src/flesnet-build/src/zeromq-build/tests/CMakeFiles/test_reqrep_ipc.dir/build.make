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
include tests/CMakeFiles/test_reqrep_ipc.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/test_reqrep_ipc.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test_reqrep_ipc.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test_reqrep_ipc.dir/flags.make

tests/CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.o: tests/CMakeFiles/test_reqrep_ipc.dir/flags.make
tests/CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.o: /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq/tests/test_reqrep_ipc.cpp
tests/CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.o: tests/CMakeFiles/test_reqrep_ipc.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.o -MF CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.o.d -o CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.o -c /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq/tests/test_reqrep_ipc.cpp

tests/CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq/tests/test_reqrep_ipc.cpp > CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.i

tests/CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq/tests/test_reqrep_ipc.cpp -o CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.s

# Object files for target test_reqrep_ipc
test_reqrep_ipc_OBJECTS = \
"CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.o"

# External object files for target test_reqrep_ipc
test_reqrep_ipc_EXTERNAL_OBJECTS =

bin/test_reqrep_ipc: tests/CMakeFiles/test_reqrep_ipc.dir/test_reqrep_ipc.cpp.o
bin/test_reqrep_ipc: tests/CMakeFiles/test_reqrep_ipc.dir/build.make
bin/test_reqrep_ipc: lib/libtestutil.a
bin/test_reqrep_ipc: /usr/lib/x86_64-linux-gnu/librt.a
bin/test_reqrep_ipc: lib/libzmq.so.5.2.4
bin/test_reqrep_ipc: lib/libunity.a
bin/test_reqrep_ipc: tests/CMakeFiles/test_reqrep_ipc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test_reqrep_ipc"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_reqrep_ipc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test_reqrep_ipc.dir/build: bin/test_reqrep_ipc
.PHONY : tests/CMakeFiles/test_reqrep_ipc.dir/build

tests/CMakeFiles/test_reqrep_ipc.dir/clean:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_reqrep_ipc.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test_reqrep_ipc.dir/clean

tests/CMakeFiles/test_reqrep_ipc.dir/depend:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq/tests /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/tests /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/tests/CMakeFiles/test_reqrep_ipc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/test_reqrep_ipc.dir/depend

