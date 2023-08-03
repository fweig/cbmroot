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
include sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/compiler_depend.make

# Include the progress variables for this target.
include sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/progress.make

# Include the compile flags for this target's objects.
include sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/flags.make

sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdSimLinkDef.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrd.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdDigitizer.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdModuleSim.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdModuleSimR.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdModuleSim2D.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdTrianglePRF.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdRawToDigiR.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdCheckUtil.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/qa/CbmTrdDigitizerPRFQa.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/qa/CbmTrdMCQa.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/qa/CbmTrdHitRateFastQa.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/qa/CbmTrdHitRateQa.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrd.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdDigitizer.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdModuleSim.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdModuleSimR.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdModuleSim2D.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdTrianglePRF.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdRawToDigiR.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdCheckUtil.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/qa/CbmTrdDigitizerPRFQa.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/qa/CbmTrdMCQa.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/qa/CbmTrdHitRateFastQa.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/qa/CbmTrdHitRateQa.h
sim/detectors/trd/G__CbmTrdSim.cxx: ../sim/detectors/trd/CbmTrdSimLinkDef.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating G__CbmTrdSim.cxx, ../../../lib/libCbmTrdSim_rdict.pcm, ../../../lib/libCbmTrdSim.rootmap"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/trd && /usr/bin/cmake -E env LD_LIBRARY_PATH=/home/cbmdata/_deps/fairsoft_apr22/install/lib:/usr/local/cuda/lib64::/home/heinemann/gpuunpacker/cbmroot/build2/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib /home/cbmdata/_deps/fairsoft_apr22/install/bin/rootcling -v2 -f G__CbmTrdSim.cxx -s /home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmTrdSim.so -rml libCbmTrdSim.so -rmf /home/heinemann/gpuunpacker/cbmroot/build2/lib/libCbmTrdSim.rootmap -DHAVE_SSE -DHAVE_SSE -DFMT_LOCALE -DBOOST_SERIALIZATION_NO_LIB -DBOOST_SERIALIZATION_DYN_LINK -DUSE_WEBSOCKET -DNO_SSL_DL -DHTTP_WITHOUT_FASTCGI -DBOOST_LOG_NO_LIB -DBOOST_LOG_DYN_LINK -DBOOST_ATOMIC_NO_LIB -DBOOST_ATOMIC_DYN_LINK -DBOOST_CHRONO_NO_LIB -DBOOST_CHRONO_DYN_LINK -DBOOST_FILESYSTEM_NO_LIB -DBOOST_FILESYSTEM_DYN_LINK -DBOOST_REGEX_NO_LIB -DBOOST_REGEX_DYN_LINK -DBOOST_THREAD_NO_LIB -DBOOST_THREAD_DYN_LINK -compilerI/usr/include/c++/11 -compilerI/usr/include/x86_64-linux-gnu/c++/11 -compilerI/usr/include/c++/11/backward -compilerI/usr/lib/gcc/x86_64-linux-gnu/11/include -compilerI/usr/local/include -compilerI/usr/include/x86_64-linux-gnu -compilerI/usr/include -compilerI/usr/lib/gcc/x86_64-linux-gnu/11/include -compilerI/usr/local/include -compilerI/usr/include/x86_64-linux-gnu -compilerI/usr/include -isystem /home/heinemann/gpuunpacker/cbmroot/external/GSL/include -I/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include -I/home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd -I/home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd/qa -I/home/cbmdata/_deps/fairsoft_apr22/install/lib/CLHEP-2.4.5.1/../../include -I/home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd -I/home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd/qa -I/home/heinemann/gpuunpacker/cbmroot/core/base -I/home/heinemann/gpuunpacker/cbmroot/core/base/report -I/home/heinemann/gpuunpacker/cbmroot/core/base/draw -I/home/heinemann/gpuunpacker/cbmroot/core/base/utils -I/home/heinemann/gpuunpacker/cbmroot/core/data -I/home/heinemann/gpuunpacker/cbmroot/core/data/base -I/home/heinemann/gpuunpacker/cbmroot/core/data/raw -I/home/heinemann/gpuunpacker/cbmroot/core/data/mvd -I/home/heinemann/gpuunpacker/cbmroot/core/data/bmon -I/home/heinemann/gpuunpacker/cbmroot/core/data/sts -I/home/heinemann/gpuunpacker/cbmroot/core/data/much -I/home/heinemann/gpuunpacker/cbmroot/core/data/trd -I/home/heinemann/gpuunpacker/cbmroot/core/data/tof -I/home/heinemann/gpuunpacker/cbmroot/core/data/rich -I/home/heinemann/gpuunpacker/cbmroot/core/data/psd -I/home/heinemann/gpuunpacker/cbmroot/core/data/global -I/home/heinemann/gpuunpacker/cbmroot/algo/log/. -I/home/cbmdata/_deps/fairroot_v18.6.9/install/include -I/home/cbmdata/_deps/fairsoft_apr22/install/include/fairlogger -I/home/cbmdata/_deps/fairsoft_apr22/install/include -I/home/cbmdata/_deps/fairsoft_apr22/install/include/root -I/home/cbmdata/_deps/fairsoft_apr22/install/include/vmc -I/home/heinemann/gpuunpacker/cbmroot/external/flesnet/lib/fles_ipc -I/home/heinemann/gpuunpacker/cbmroot/external/xpu/src -I/home/heinemann/gpuunpacker/cbmroot/external/GSL/include -I/home/heinemann/gpuunpacker/cbmroot/core/detectors/trd -I/home/heinemann/gpuunpacker/cbmroot/sim/transport/base CbmTrd.h CbmTrdDigitizer.h CbmTrdModuleSim.h CbmTrdModuleSimR.h CbmTrdModuleSim2D.h CbmTrdTrianglePRF.h CbmTrdRawToDigiR.h CbmTrdCheckUtil.h CbmTrdDigitizerPRFQa.h CbmTrdMCQa.h CbmTrdHitRateFastQa.h CbmTrdHitRateQa.h /home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd/CbmTrdSimLinkDef.h

lib/libCbmTrdSim_rdict.pcm: sim/detectors/trd/G__CbmTrdSim.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate lib/libCbmTrdSim_rdict.pcm

lib/libCbmTrdSim.rootmap: sim/detectors/trd/G__CbmTrdSim.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate lib/libCbmTrdSim.rootmap

sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/G__CbmTrdSim.cxx.o: sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/flags.make
sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/G__CbmTrdSim.cxx.o: sim/detectors/trd/G__CbmTrdSim.cxx
sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/G__CbmTrdSim.cxx.o: sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/G__CbmTrdSim.cxx.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/trd && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/G__CbmTrdSim.cxx.o -MF CMakeFiles/G__CbmTrdSim.dir/G__CbmTrdSim.cxx.o.d -o CMakeFiles/G__CbmTrdSim.dir/G__CbmTrdSim.cxx.o -c /home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/trd/G__CbmTrdSim.cxx

sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/G__CbmTrdSim.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/G__CbmTrdSim.dir/G__CbmTrdSim.cxx.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/trd && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/trd/G__CbmTrdSim.cxx > CMakeFiles/G__CbmTrdSim.dir/G__CbmTrdSim.cxx.i

sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/G__CbmTrdSim.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/G__CbmTrdSim.dir/G__CbmTrdSim.cxx.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/trd && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/trd/G__CbmTrdSim.cxx -o CMakeFiles/G__CbmTrdSim.dir/G__CbmTrdSim.cxx.s

G__CbmTrdSim: sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/G__CbmTrdSim.cxx.o
G__CbmTrdSim: sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/build.make
.PHONY : G__CbmTrdSim

# Rule to build all files generated by this target.
sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/build: G__CbmTrdSim
.PHONY : sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/build

sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/clean:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/trd && $(CMAKE_COMMAND) -P CMakeFiles/G__CbmTrdSim.dir/cmake_clean.cmake
.PHONY : sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/clean

sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/depend: lib/libCbmTrdSim.rootmap
sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/depend: lib/libCbmTrdSim_rdict.pcm
sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/depend: sim/detectors/trd/G__CbmTrdSim.cxx
	cd /home/heinemann/gpuunpacker/cbmroot/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heinemann/gpuunpacker/cbmroot /home/heinemann/gpuunpacker/cbmroot/sim/detectors/trd /home/heinemann/gpuunpacker/cbmroot/build2 /home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/trd /home/heinemann/gpuunpacker/cbmroot/build2/sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : sim/detectors/trd/CMakeFiles/G__CbmTrdSim.dir/depend

