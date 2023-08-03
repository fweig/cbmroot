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
CMAKE_SOURCE_DIR = /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build

# Include any dependencies generated for this target.
include src/CMakeFiles/AnalysisTreeQA.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/CMakeFiles/AnalysisTreeQA.dir/compiler_depend.make

# Include the progress variables for this target.
include src/CMakeFiles/AnalysisTreeQA.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/AnalysisTreeQA.dir/flags.make

src/G__AnalysisTreeQA.cxx: /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/AnalysisTreeQALinkDef.h
src/G__AnalysisTreeQA.cxx: /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/EntryConfig.hpp
src/G__AnalysisTreeQA.cxx: /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/Task.hpp
src/G__AnalysisTreeQA.cxx: /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/BasicQA.hpp
src/G__AnalysisTreeQA.cxx: /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/EntryConfig.hpp
src/G__AnalysisTreeQA.cxx: /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/Task.hpp
src/G__AnalysisTreeQA.cxx: /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/BasicQA.hpp
src/G__AnalysisTreeQA.cxx: /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/AnalysisTreeQALinkDef.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating G__AnalysisTreeQA.cxx, libAnalysisTreeQA_rdict.pcm, libAnalysisTreeQA.rootmap"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src && /usr/bin/cmake -E env LD_LIBRARY_PATH=/home/cbmdata/_deps/fairsoft_apr22/install/lib:/usr/local/cuda/lib64::/home/heinemann/gpuunpacker/cbmroot/build/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib:/home/heinemann/gpuunpacker/cbmroot/build/lib:/home/cbmdata/_deps/fairsoft_apr22/install/lib:/home/cbmdata/_deps/fairroot_v18.6.9/install/lib /home/cbmdata/_deps/fairsoft_apr22/install/bin/rootcling -v2 -f G__AnalysisTreeQA.cxx -s /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src/libAnalysisTreeQA.so -rml libAnalysisTreeQA.so -rmf /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src/libAnalysisTreeQA.rootmap -I/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/include -I/include -compilerI/usr/include/c++/11 -compilerI/usr/include/x86_64-linux-gnu/c++/11 -compilerI/usr/include/c++/11/backward -compilerI/usr/lib/gcc/x86_64-linux-gnu/11/include -compilerI/usr/local/include -compilerI/usr/include/x86_64-linux-gnu -compilerI/usr/include -I/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA -I/home/cbmdata/_deps/fairsoft_apr22/install/include/root -I/home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src -I/home/heinemann/gpuunpacker/cbmroot/build2/include -I/include EntryConfig.hpp Task.hpp BasicQA.hpp /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/AnalysisTreeQALinkDef.h

src/libAnalysisTreeQA_rdict.pcm: src/G__AnalysisTreeQA.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate src/libAnalysisTreeQA_rdict.pcm

src/libAnalysisTreeQA.rootmap: src/G__AnalysisTreeQA.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate src/libAnalysisTreeQA.rootmap

src/CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.o: src/CMakeFiles/AnalysisTreeQA.dir/flags.make
src/CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.o: /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/EntryConfig.cpp
src/CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.o: src/CMakeFiles/AnalysisTreeQA.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.o -MF CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.o.d -o CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.o -c /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/EntryConfig.cpp

src/CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/EntryConfig.cpp > CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.i

src/CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/EntryConfig.cpp -o CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.s

src/CMakeFiles/AnalysisTreeQA.dir/Task.cpp.o: src/CMakeFiles/AnalysisTreeQA.dir/flags.make
src/CMakeFiles/AnalysisTreeQA.dir/Task.cpp.o: /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/Task.cpp
src/CMakeFiles/AnalysisTreeQA.dir/Task.cpp.o: src/CMakeFiles/AnalysisTreeQA.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/AnalysisTreeQA.dir/Task.cpp.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/AnalysisTreeQA.dir/Task.cpp.o -MF CMakeFiles/AnalysisTreeQA.dir/Task.cpp.o.d -o CMakeFiles/AnalysisTreeQA.dir/Task.cpp.o -c /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/Task.cpp

src/CMakeFiles/AnalysisTreeQA.dir/Task.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AnalysisTreeQA.dir/Task.cpp.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/Task.cpp > CMakeFiles/AnalysisTreeQA.dir/Task.cpp.i

src/CMakeFiles/AnalysisTreeQA.dir/Task.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AnalysisTreeQA.dir/Task.cpp.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src/Task.cpp -o CMakeFiles/AnalysisTreeQA.dir/Task.cpp.s

src/CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.o: src/CMakeFiles/AnalysisTreeQA.dir/flags.make
src/CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.o: src/G__AnalysisTreeQA.cxx
src/CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.o: src/CMakeFiles/AnalysisTreeQA.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.o"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.o -MF CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.o.d -o CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.o -c /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src/G__AnalysisTreeQA.cxx

src/CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.i"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src/G__AnalysisTreeQA.cxx > CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.i

src/CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.s"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src/G__AnalysisTreeQA.cxx -o CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.s

# Object files for target AnalysisTreeQA
AnalysisTreeQA_OBJECTS = \
"CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.o" \
"CMakeFiles/AnalysisTreeQA.dir/Task.cpp.o" \
"CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.o"

# External object files for target AnalysisTreeQA
AnalysisTreeQA_EXTERNAL_OBJECTS =

src/libAnalysisTreeQA.so: src/CMakeFiles/AnalysisTreeQA.dir/EntryConfig.cpp.o
src/libAnalysisTreeQA.so: src/CMakeFiles/AnalysisTreeQA.dir/Task.cpp.o
src/libAnalysisTreeQA.so: src/CMakeFiles/AnalysisTreeQA.dir/G__AnalysisTreeQA.cxx.o
src/libAnalysisTreeQA.so: src/CMakeFiles/AnalysisTreeQA.dir/build.make
src/libAnalysisTreeQA.so: /home/heinemann/gpuunpacker/cbmroot/build2/lib/libAnalysisTreeInfra.so
src/libAnalysisTreeQA.so: /home/heinemann/gpuunpacker/cbmroot/build2/lib/libAnalysisTreeBase.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libCore.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libImt.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libRIO.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libNet.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libHist.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGraf3d.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libGpad.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libROOTDataFrame.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libTree.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libTreePlayer.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libRint.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libPostscript.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libMatrix.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libPhysics.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libMathCore.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libThread.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libMultiProc.so
src/libAnalysisTreeQA.so: /home/cbmdata/_deps/fairsoft_apr22/install/lib/libROOTVecOps.so
src/libAnalysisTreeQA.so: src/CMakeFiles/AnalysisTreeQA.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX shared library libAnalysisTreeQA.so"
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AnalysisTreeQA.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/AnalysisTreeQA.dir/build: src/libAnalysisTreeQA.so
.PHONY : src/CMakeFiles/AnalysisTreeQA.dir/build

src/CMakeFiles/AnalysisTreeQA.dir/clean:
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src && $(CMAKE_COMMAND) -P CMakeFiles/AnalysisTreeQA.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/AnalysisTreeQA.dir/clean

src/CMakeFiles/AnalysisTreeQA.dir/depend: src/G__AnalysisTreeQA.cxx
src/CMakeFiles/AnalysisTreeQA.dir/depend: src/libAnalysisTreeQA.rootmap
src/CMakeFiles/AnalysisTreeQA.dir/depend: src/libAnalysisTreeQA_rdict.pcm
	cd /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA /home/heinemann/gpuunpacker/cbmroot/external/AnalysisTreeQA/src /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src /home/heinemann/gpuunpacker/cbmroot/build2/external/ANALYSISTREEQA-prefix/src/ANALYSISTREEQA-build/src/CMakeFiles/AnalysisTreeQA.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/AnalysisTreeQA.dir/depend

