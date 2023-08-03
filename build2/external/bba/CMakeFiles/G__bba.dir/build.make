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

# Utility rule file for G__bba.

# Include any custom commands dependencies for this target.
include external/bba/CMakeFiles/G__bba.dir/compiler_depend.make

# Include the progress variables for this target.
include external/bba/CMakeFiles/G__bba.dir/progress.make

external/bba/CMakeFiles/G__bba: external/bba/G__bba.cxx
external/bba/CMakeFiles/G__bba: lib/libbba_rdict.pcm
external/bba/CMakeFiles/G__bba: lib/libbba.rootmap

external/bba/G__bba.cxx: ../external/bba/BBALinkDef.h
external/bba/G__bba.cxx: ../external/bba/include/bba/BBA.h
external/bba/G__bba.cxx: ../external/bba/include/bba/BBA.h
external/bba/G__bba.cxx: ../external/bba/BBALinkDef.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating G__bba.cxx, ../../lib/libbba_rdict.pcm, ../../lib/libbba.rootmap"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/bba && /usr/bin/cmake -E env LD_LIBRARY_PATH=/home/cbmdata/_deps/fairsoft_apr22/install/lib:/usr/local/cuda/lib64::/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib /home/cbmdata/_deps/fairsoft_apr22/install/bin/rootcling -v2 -f G__bba.cxx -s /home/heinemann/gpuunpacker/cbmroot/build2/lib/libbba.so -rml libbba.so -rmf /home/heinemann/gpuunpacker/cbmroot/build2/lib/libbba.rootmap -compilerI/usr/include/c++/11 -compilerI/usr/include/x86_64-linux-gnu/c++/11 -compilerI/usr/include/c++/11/backward -compilerI/usr/lib/gcc/x86_64-linux-gnu/11/include -compilerI/usr/local/include -compilerI/usr/include/x86_64-linux-gnu -compilerI/usr/include -compilerI/usr/lib/gcc/x86_64-linux-gnu/11/include -compilerI/usr/local/include -compilerI/usr/include/x86_64-linux-gnu -compilerI/usr/include -I/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include -I/home/cbmdata/_deps/fairsoft_apr22/install/include/root -I/home/heinemann/gpuunpacker/cbmroot/external/bba/include -I/home/heinemann/gpuunpacker/cbmroot/external/bba -I/home/heinemann/gpuunpacker/cbmroot/external/bba include/bba/BBA.h /home/heinemann/gpuunpacker/cbmroot/external/bba/BBALinkDef.h

lib/libbba_rdict.pcm: external/bba/G__bba.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate lib/libbba_rdict.pcm

lib/libbba.rootmap: external/bba/G__bba.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate lib/libbba.rootmap

G__bba: external/bba/CMakeFiles/G__bba
G__bba: external/bba/G__bba.cxx
G__bba: lib/libbba.rootmap
G__bba: lib/libbba_rdict.pcm
G__bba: external/bba/CMakeFiles/G__bba.dir/build.make
.PHONY : G__bba

# Rule to build all files generated by this target.
external/bba/CMakeFiles/G__bba.dir/build: G__bba
.PHONY : external/bba/CMakeFiles/G__bba.dir/build

external/bba/CMakeFiles/G__bba.dir/clean:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/bba && $(CMAKE_COMMAND) -P CMakeFiles/G__bba.dir/cmake_clean.cmake
.PHONY : external/bba/CMakeFiles/G__bba.dir/clean

external/bba/CMakeFiles/G__bba.dir/depend:
	cd /home/heinemann/gpuunpacker/cbmroot/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heinemann/gpuunpacker/cbmroot /home/heinemann/gpuunpacker/cbmroot/external/bba /home/heinemann/gpuunpacker/cbmroot/build2 /home/heinemann/gpuunpacker/cbmroot/build2/external/bba /home/heinemann/gpuunpacker/cbmroot/build2/external/bba/CMakeFiles/G__bba.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/bba/CMakeFiles/G__bba.dir/depend

