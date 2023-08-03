# CMake generated Testfile for 
# Source directory: /home/heinemann/gpuunpacker/cbmroot/core/data/test/global
# Build directory: /home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/global
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[_GTestCbmDigiEvent]=] "/home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/global/_GTestCbmDigiEvent.sh")
set_tests_properties([=[_GTestCbmDigiEvent]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/core/data/test/CMakeLists.txt;10;Add_Test;/home/heinemann/gpuunpacker/cbmroot/core/data/test/global/CMakeLists.txt;26;CreateGTestExeAndAddTest;/home/heinemann/gpuunpacker/cbmroot/core/data/test/global/CMakeLists.txt;0;")
add_test([=[_GTestCbmDigiTimeslice]=] "/home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/global/_GTestCbmDigiTimeslice.sh")
set_tests_properties([=[_GTestCbmDigiTimeslice]=] PROPERTIES  DEPENDS "_GTestCbmDigiEvent" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/core/data/test/CMakeLists.txt;10;Add_Test;/home/heinemann/gpuunpacker/cbmroot/core/data/test/global/CMakeLists.txt;33;CreateGTestExeAndAddTest;/home/heinemann/gpuunpacker/cbmroot/core/data/test/global/CMakeLists.txt;0;")
add_test([=[_GTestCbmVertex]=] "/home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/global/_GTestCbmVertex.sh")
set_tests_properties([=[_GTestCbmVertex]=] PROPERTIES  DEPENDS "_GTestCbmDigiTimeslice" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/core/data/test/CMakeLists.txt;10;Add_Test;/home/heinemann/gpuunpacker/cbmroot/core/data/test/global/CMakeLists.txt;40;CreateGTestExeAndAddTest;/home/heinemann/gpuunpacker/cbmroot/core/data/test/global/CMakeLists.txt;0;")
