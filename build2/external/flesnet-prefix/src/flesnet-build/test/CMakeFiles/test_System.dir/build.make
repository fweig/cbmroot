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
CMAKE_SOURCE_DIR = /home/heinemann/gpuunpacker/cbmroot/external/flesnet

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build

# Include any dependencies generated for this target.
include test/CMakeFiles/test_System.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/test_System.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_System.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_System.dir/flags.make

test/CMakeFiles/test_System.dir/test_System.cpp.o: test/CMakeFiles/test_System.dir/flags.make
test/CMakeFiles/test_System.dir/test_System.cpp.o: /home/heinemann/gpuunpacker/cbmroot/external/flesnet/test/test_System.cpp
test/CMakeFiles/test_System.dir/test_System.cpp.o: test/CMakeFiles/test_System.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_System.dir/test_System.cpp.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/test_System.dir/test_System.cpp.o -MF CMakeFiles/test_System.dir/test_System.cpp.o.d -o CMakeFiles/test_System.dir/test_System.cpp.o -c /home/heinemann/gpuunpacker/cbmroot/external/flesnet/test/test_System.cpp

test/CMakeFiles/test_System.dir/test_System.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_System.dir/test_System.cpp.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/external/flesnet/test/test_System.cpp > CMakeFiles/test_System.dir/test_System.cpp.i

test/CMakeFiles/test_System.dir/test_System.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_System.dir/test_System.cpp.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/external/flesnet/test/test_System.cpp -o CMakeFiles/test_System.dir/test_System.cpp.s

# Object files for target test_System
test_System_OBJECTS = \
"CMakeFiles/test_System.dir/test_System.cpp.o"

# External object files for target test_System
test_System_EXTERNAL_OBJECTS =

test/test_System: test/CMakeFiles/test_System.dir/test_System.cpp.o
test/test_System: test/CMakeFiles/test_System.dir/build.make
test/test_System: lib/fles_ipc/libfles_ipc.a
test/test_System: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_program_options.so.1.78.0
test/test_System: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_serialization.so.1.78.0
test/test_System: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_unit_test_framework.so.1.78.0
test/test_System: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_iostreams.so.1.78.0
test/test_System: lib/shm_ipc/libshm_ipc.a
test/test_System: src/zeromq-build/lib/libzmq.a
test/test_System: lib/logging/liblogging.a
test/test_System: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_log.so.1.78.0
test/test_System: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_filesystem.so.1.78.0
test/test_System: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_regex.so.1.78.0
test/test_System: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_atomic.so.1.78.0
test/test_System: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_chrono.so.1.78.0
test/test_System: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_system.so.1.78.0
test/test_System: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libboost_thread.so.1.78.0
test/test_System: test/CMakeFiles/test_System.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_System"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_System.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_System.dir/build: test/test_System
.PHONY : test/CMakeFiles/test_System.dir/build

test/CMakeFiles/test_System.dir/clean:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/test && $(CMAKE_COMMAND) -P CMakeFiles/test_System.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_System.dir/clean

test/CMakeFiles/test_System.dir/depend:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heinemann/gpuunpacker/cbmroot/external/flesnet /home/heinemann/gpuunpacker/cbmroot/external/flesnet/test /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/test /home/heinemann/gpuunpacker/cbmroot/build2/external/flesnet-prefix/src/flesnet-build/test/CMakeFiles/test_System.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test_System.dir/depend

