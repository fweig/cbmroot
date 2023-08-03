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
include core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/compiler_depend.make

# Include the progress variables for this target.
include core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/progress.make

# Include the compile flags for this target's objects.
include core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/flags.make

core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmDisplayLinkDef.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmTrdDigiDraw.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmPixelHitSetDraw.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmEvDisTracks.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmRecoTracks.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmPointSetArray.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmPointSetArrayDraw.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmPointSetArrayEditor.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmTrdDigiDraw.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmPixelHitSetDraw.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmEvDisTracks.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmRecoTracks.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmPointSetArray.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmPointSetArrayDraw.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmPointSetArrayEditor.h
core/eventdisplay/G__CbmDisplay.cxx: ../core/eventdisplay/CbmDisplayLinkDef.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating G__CbmDisplay.cxx, ../../lib/libCbmDisplay_rdict.pcm, ../../lib/libCbmDisplay.rootmap"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/core/eventdisplay && /usr/bin/cmake -E env LD_LIBRARY_PATH=/home/cbmdata/_deps/fairsoft_apr22/install/lib:/usr/local/cuda/lib64::/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib /home/cbmdata/_deps/fairsoft_apr22/install/bin/rootcling -v2 -f G__CbmDisplay.cxx -s /home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmDisplay.so -rml libCbmDisplay.so -rmf /home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmDisplay.rootmap -DFMT_LOCALE -DBOOST_SERIALIZATION_NO_LIB -DBOOST_SERIALIZATION_DYN_LINK -DUSE_WEBSOCKET -DNO_SSL_DL -DHTTP_WITHOUT_FASTCGI -DBOOST_LOG_NO_LIB -DBOOST_LOG_DYN_LINK -DBOOST_ATOMIC_NO_LIB -DBOOST_ATOMIC_DYN_LINK -DBOOST_CHRONO_NO_LIB -DBOOST_CHRONO_DYN_LINK -DBOOST_FILESYSTEM_NO_LIB -DBOOST_FILESYSTEM_DYN_LINK -DBOOST_REGEX_NO_LIB -DBOOST_REGEX_DYN_LINK -DBOOST_THREAD_NO_LIB -DBOOST_THREAD_DYN_LINK -compilerI/usr/include/c++/11 -compilerI/usr/include/x86_64-linux-gnu/c++/11 -compilerI/usr/include/c++/11/backward -compilerI/usr/lib/gcc/x86_64-linux-gnu/11/include -compilerI/usr/local/include -compilerI/usr/include/x86_64-linux-gnu -compilerI/usr/include -compilerI/usr/lib/gcc/x86_64-linux-gnu/11/include -compilerI/usr/local/include -compilerI/usr/include/x86_64-linux-gnu -compilerI/usr/include -I/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include -I/home/heinemann/gpuunpacker/cbmroot/core/eventdisplay -I/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include -I/home/heinemann/gpuunpacker/cbmroot/core/eventdisplay -I/home/cbmdata/_deps/fairroot_v18.6.9/install/include -I/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger -I/home/cbmdata/_deps/fairsoft_apr22/install/include -I/home/cbmdata/_deps/fairsoft_apr22/install/include/root -I/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc -I/home/heinemann/gpuunpacker/cbmroot/core/data -I/home/heinemann/gpuunpacker/cbmroot/core/data/base -I/home/heinemann/gpuunpacker/cbmroot/core/data/raw -I/home/heinemann/gpuunpacker/cbmroot/core/data/mvd -I/home/heinemann/gpuunpacker/cbmroot/core/data/bmon -I/home/heinemann/gpuunpacker/cbmroot/core/data/sts -I/home/heinemann/gpuunpacker/cbmroot/core/data/much -I/home/heinemann/gpuunpacker/cbmroot/core/data/trd -I/home/heinemann/gpuunpacker/cbmroot/core/data/tof -I/home/heinemann/gpuunpacker/cbmroot/core/data/rich -I/home/heinemann/gpuunpacker/cbmroot/core/data/psd -I/home/heinemann/gpuunpacker/cbmroot/core/data/global -I/home/heinemann/gpuunpacker/cbmroot/algo/log/. -I/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc -I/home/heinemann/gpuunpacker/cbmroot/external/xpu/src -I/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd CbmTrdDigiDraw.h CbmPixelHitSetDraw.h CbmEvDisTracks.h CbmRecoTracks.h CbmPointSetArray.h CbmPointSetArrayDraw.h CbmPointSetArrayEditor.h /home/heinemann/gpuunpacker/cbmroot/core/eventdisplay/CbmDisplayLinkDef.h

lib/libCbmDisplay_rdict.pcm: core/eventdisplay/G__CbmDisplay.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate lib/libCbmDisplay_rdict.pcm

lib/libCbmDisplay.rootmap: core/eventdisplay/G__CbmDisplay.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate lib/libCbmDisplay.rootmap

core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/G__CbmDisplay.cxx.o: core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/flags.make
core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/G__CbmDisplay.cxx.o: core/eventdisplay/G__CbmDisplay.cxx
core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/G__CbmDisplay.cxx.o: core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/G__CbmDisplay.cxx.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/core/eventdisplay && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/G__CbmDisplay.cxx.o -MF CMakeFiles/G__CbmDisplay.dir/G__CbmDisplay.cxx.o.d -o CMakeFiles/G__CbmDisplay.dir/G__CbmDisplay.cxx.o -c /home/heinemann/gpuunpacker/cbmroot/build2/core/eventdisplay/G__CbmDisplay.cxx

core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/G__CbmDisplay.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/G__CbmDisplay.dir/G__CbmDisplay.cxx.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/core/eventdisplay && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/build2/core/eventdisplay/G__CbmDisplay.cxx > CMakeFiles/G__CbmDisplay.dir/G__CbmDisplay.cxx.i

core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/G__CbmDisplay.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/G__CbmDisplay.dir/G__CbmDisplay.cxx.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/core/eventdisplay && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/build2/core/eventdisplay/G__CbmDisplay.cxx -o CMakeFiles/G__CbmDisplay.dir/G__CbmDisplay.cxx.s

G__CbmDisplay: core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/G__CbmDisplay.cxx.o
G__CbmDisplay: core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/build.make
.PHONY : G__CbmDisplay

# Rule to build all files generated by this target.
core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/build: G__CbmDisplay
.PHONY : core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/build

core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/clean:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/core/eventdisplay && $(CMAKE_COMMAND) -P CMakeFiles/G__CbmDisplay.dir/cmake_clean.cmake
.PHONY : core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/clean

core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/depend: core/eventdisplay/G__CbmDisplay.cxx
core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/depend: lib/libCbmDisplay.rootmap
core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/depend: lib/libCbmDisplay_rdict.pcm
	cd /home/heinemann/gpuunpacker/cbmroot/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heinemann/gpuunpacker/cbmroot /home/heinemann/gpuunpacker/cbmroot/core/eventdisplay /home/heinemann/gpuunpacker/cbmroot/build2 /home/heinemann/gpuunpacker/cbmroot/build2/core/eventdisplay /home/heinemann/gpuunpacker/cbmroot/build2/core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : core/eventdisplay/CMakeFiles/G__CbmDisplay.dir/depend

