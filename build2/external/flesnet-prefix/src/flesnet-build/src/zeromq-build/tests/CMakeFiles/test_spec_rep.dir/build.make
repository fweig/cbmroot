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
include tests/CMakeFiles/test_spec_rep.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/test_spec_rep.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test_spec_rep.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test_spec_rep.dir/flags.make

tests/CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.o: tests/CMakeFiles/test_spec_rep.dir/flags.make
tests/CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.o: /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq/tests/test_spec_rep.cpp
tests/CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.o: tests/CMakeFiles/test_spec_rep.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.o -MF CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.o.d -o CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.o -c /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq/tests/test_spec_rep.cpp

tests/CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq/tests/test_spec_rep.cpp > CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.i

tests/CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq/tests/test_spec_rep.cpp -o CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.s

# Object files for target test_spec_rep
test_spec_rep_OBJECTS = \
"CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.o"

# External object files for target test_spec_rep
test_spec_rep_EXTERNAL_OBJECTS =

bin/test_spec_rep: tests/CMakeFiles/test_spec_rep.dir/test_spec_rep.cpp.o
bin/test_spec_rep: tests/CMakeFiles/test_spec_rep.dir/build.make
bin/test_spec_rep: lib/libtestutil.a
bin/test_spec_rep: /usr/lib/x86_64-linux-gnu/librt.a
bin/test_spec_rep: lib/libzmq.so.5.2.4
bin/test_spec_rep: lib/libunity.a
bin/test_spec_rep: tests/CMakeFiles/test_spec_rep.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test_spec_rep"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_spec_rep.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test_spec_rep.dir/build: bin/test_spec_rep
.PHONY : tests/CMakeFiles/test_spec_rep.dir/build

tests/CMakeFiles/test_spec_rep.dir/clean:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_spec_rep.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test_spec_rep.dir/clean

tests/CMakeFiles/test_spec_rep.dir/depend:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq/tests /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/tests /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/src/zeromq-build/tests/CMakeFiles/test_spec_rep.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/test_spec_rep.dir/depend

