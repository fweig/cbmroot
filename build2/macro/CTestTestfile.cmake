# CMake generated Testfile for 
# Source directory: /home/heinemann/gpuunpacker/cbmroot/macro
# Build directory: /home/heinemann/gpuunpacker/cbmroot/build2/macro
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[load_libraries]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/checklibs.sh")
set_tests_properties([=[load_libraries]=] PROPERTIES  TIMEOUT "900" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/CMakeLists.txt;27;add_test;/home/heinemann/gpuunpacker/cbmroot/macro/CMakeLists.txt;0;")
subdirs("run")
subdirs("qa")
subdirs("mcbm")
subdirs("mvd")
subdirs("much")
subdirs("include")
subdirs("reco")
subdirs("beamtime")
