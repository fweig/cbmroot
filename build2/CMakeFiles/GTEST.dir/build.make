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

# Utility rule file for GTEST.

# Include any custom commands dependencies for this target.
include CMakeFiles/GTEST.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/GTEST.dir/progress.make

CMakeFiles/GTEST: CMakeFiles/GTEST-complete

CMakeFiles/GTEST-complete: GTEST-prefix/src/GTEST-stamp/GTEST-install
CMakeFiles/GTEST-complete: GTEST-prefix/src/GTEST-stamp/GTEST-mkdir
CMakeFiles/GTEST-complete: GTEST-prefix/src/GTEST-stamp/GTEST-download
CMakeFiles/GTEST-complete: GTEST-prefix/src/GTEST-stamp/GTEST-update
CMakeFiles/GTEST-complete: GTEST-prefix/src/GTEST-stamp/GTEST-patch
CMakeFiles/GTEST-complete: GTEST-prefix/src/GTEST-stamp/GTEST-configure
CMakeFiles/GTEST-complete: GTEST-prefix/src/GTEST-stamp/GTEST-build
CMakeFiles/GTEST-complete: GTEST-prefix/src/GTEST-stamp/GTEST-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'GTEST'"
	/usr/bin/cmake -E make_directory /home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles
	/usr/bin/cmake -E touch /home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles/GTEST-complete
	/usr/bin/cmake -E touch /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-stamp/GTEST-done

GTEST-prefix/src/GTEST-stamp/GTEST-build: GTEST-prefix/src/GTEST-stamp/GTEST-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing build step for 'GTEST'"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-build && /usr/bin/cmake -Dmake=$(MAKE) -P /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-stamp/GTEST-build-FullWarnings.cmake
	cd /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-build && /usr/bin/cmake -E touch /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-stamp/GTEST-build

GTEST-prefix/src/GTEST-stamp/GTEST-configure: GTEST-prefix/tmp/GTEST-cfgcmd.txt
GTEST-prefix/src/GTEST-stamp/GTEST-configure: GTEST-prefix/src/GTEST-stamp/GTEST-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Performing configure step for 'GTEST'"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-build && /usr/bin/cmake -P /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-stamp/GTEST-configure-FullWarnings.cmake
	cd /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-build && /usr/bin/cmake -E touch /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-stamp/GTEST-configure

GTEST-prefix/src/GTEST-stamp/GTEST-download: GTEST-prefix/src/GTEST-stamp/GTEST-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "No download step for 'GTEST'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-stamp/GTEST-download

GTEST-prefix/src/GTEST-stamp/GTEST-install: GTEST-prefix/src/GTEST-stamp/GTEST-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Performing install step for 'GTEST'"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-build && /usr/bin/cmake -P /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-stamp/GTEST-install-FullWarnings.cmake
	cd /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-build && /usr/bin/cmake -E touch /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-stamp/GTEST-install

GTEST-prefix/src/GTEST-stamp/GTEST-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'GTEST'"
	/usr/bin/cmake -E make_directory /home/heinemann/gpuunpacker/cbmroot/external/googletest
	/usr/bin/cmake -E make_directory /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-build
	/usr/bin/cmake -E make_directory /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix
	/usr/bin/cmake -E make_directory /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/tmp
	/usr/bin/cmake -E make_directory /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-stamp
	/usr/bin/cmake -E make_directory /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src
	/usr/bin/cmake -E make_directory /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-stamp
	/usr/bin/cmake -E touch /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-stamp/GTEST-mkdir

GTEST-prefix/src/GTEST-stamp/GTEST-patch: GTEST-prefix/src/GTEST-stamp/GTEST-update
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'GTEST'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-stamp/GTEST-patch

GTEST-prefix/src/GTEST-stamp/GTEST-update: GTEST-prefix/src/GTEST-stamp/GTEST-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No update step for 'GTEST'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/heinemann/gpuunpacker/cbmroot/build2/GTEST-prefix/src/GTEST-stamp/GTEST-update

GTEST: CMakeFiles/GTEST
GTEST: CMakeFiles/GTEST-complete
GTEST: GTEST-prefix/src/GTEST-stamp/GTEST-build
GTEST: GTEST-prefix/src/GTEST-stamp/GTEST-configure
GTEST: GTEST-prefix/src/GTEST-stamp/GTEST-download
GTEST: GTEST-prefix/src/GTEST-stamp/GTEST-install
GTEST: GTEST-prefix/src/GTEST-stamp/GTEST-mkdir
GTEST: GTEST-prefix/src/GTEST-stamp/GTEST-patch
GTEST: GTEST-prefix/src/GTEST-stamp/GTEST-update
GTEST: CMakeFiles/GTEST.dir/build.make
.PHONY : GTEST

# Rule to build all files generated by this target.
CMakeFiles/GTEST.dir/build: GTEST
.PHONY : CMakeFiles/GTEST.dir/build

CMakeFiles/GTEST.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GTEST.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GTEST.dir/clean

CMakeFiles/GTEST.dir/depend:
	cd /home/heinemann/gpuunpacker/cbmroot/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heinemann/gpuunpacker/cbmroot /home/heinemann/gpuunpacker/cbmroot /home/heinemann/gpuunpacker/cbmroot/build2 /home/heinemann/gpuunpacker/cbmroot/build2 /home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles/GTEST.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/GTEST.dir/depend

