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

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /home/heinemann/gpuunpacker/cbmroot/external/NicaFemto

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build

# Include any dependencies generated for this target.
include features/CMakeFiles/nica-jobs.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include features/CMakeFiles/nica-jobs.dir/compiler_depend.make

# Include the progress variables for this target.
include features/CMakeFiles/nica-jobs.dir/progress.make

# Include the compile flags for this target's objects.
include features/CMakeFiles/nica-jobs.dir/flags.make

features/CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.o: features/CMakeFiles/nica-jobs.dir/flags.make
features/CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.o: /home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/features_apps/nica_jobs.cpp
features/CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.o: features/CMakeFiles/nica-jobs.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object features/CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/features && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT features/CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.o -MF CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.o.d -o CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.o -c /home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/features_apps/nica_jobs.cpp

features/CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/features && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/features_apps/nica_jobs.cpp > CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.i

features/CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/features && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features/features_apps/nica_jobs.cpp -o CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.s

# Object files for target nica-jobs
nica__jobs_OBJECTS = \
"CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.o"

# External object files for target nica-jobs
nica__jobs_EXTERNAL_OBJECTS =

bin/nica-jobs: features/CMakeFiles/nica-jobs.dir/features_apps/nica_jobs.cpp.o
bin/nica-jobs: features/CMakeFiles/nica-jobs.dir/build.make
bin/nica-jobs: lib/libNicaFemto.so.0
bin/nica-jobs: lib/libNicaFlow.so.0
bin/nica-jobs: lib/libNicaSpectra.so.0
bin/nica-jobs: lib/libNicaAna.so.0
bin/nica-jobs: lib/libNicaCut.so.0
bin/nica-jobs: lib/libNicaDataFormat.so.0
bin/nica-jobs: lib/libNicaFeatures.so.0
bin/nica-jobs: features/CMakeFiles/nica-jobs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/nica-jobs"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/features && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/nica-jobs.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
features/CMakeFiles/nica-jobs.dir/build: bin/nica-jobs
.PHONY : features/CMakeFiles/nica-jobs.dir/build

features/CMakeFiles/nica-jobs.dir/clean:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/features && $(CMAKE_COMMAND) -P CMakeFiles/nica-jobs.dir/cmake_clean.cmake
.PHONY : features/CMakeFiles/nica-jobs.dir/clean

features/CMakeFiles/nica-jobs.dir/depend:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heinemann/gpuunpacker/cbmroot/external/NicaFemto /home/heinemann/gpuunpacker/cbmroot/external/NicaFemto/features /home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build /home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/features /home/heinemann/gpuunpacker/cbmroot/build2/external/NICAFEMTO-prefix/src/NICAFEMTO-build/features/CMakeFiles/nica-jobs.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : features/CMakeFiles/nica-jobs.dir/depend

