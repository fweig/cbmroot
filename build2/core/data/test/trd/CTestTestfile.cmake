# CMake generated Testfile for 
# Source directory: /home/heinemann/gpuunpacker/cbmroot/core/data/test/trd
# Build directory: /home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/trd
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[_GTestCbmTrdDigi]=] "/home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/trd/_GTestCbmTrdDigi.sh")
set_tests_properties([=[_GTestCbmTrdDigi]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/core/data/test/CMakeLists.txt;10;Add_Test;/home/heinemann/gpuunpacker/cbmroot/core/data/test/trd/CMakeLists.txt;19;CreateGTestExeAndAddTest;/home/heinemann/gpuunpacker/cbmroot/core/data/test/trd/CMakeLists.txt;0;")
add_test([=[_GTestCbmTrdPoint]=] "/home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/trd/_GTestCbmTrdPoint.sh")
set_tests_properties([=[_GTestCbmTrdPoint]=] PROPERTIES  DEPENDS "_GTestTrdDigi" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/core/data/test/CMakeLists.txt;10;Add_Test;/home/heinemann/gpuunpacker/cbmroot/core/data/test/trd/CMakeLists.txt;26;CreateGTestExeAndAddTest;/home/heinemann/gpuunpacker/cbmroot/core/data/test/trd/CMakeLists.txt;0;")
add_test([=[_GTestCbmTrdHit]=] "/home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/trd/_GTestCbmTrdHit.sh")
set_tests_properties([=[_GTestCbmTrdHit]=] PROPERTIES  DEPENDS "_GTestTrdPoint" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/core/data/test/CMakeLists.txt;10;Add_Test;/home/heinemann/gpuunpacker/cbmroot/core/data/test/trd/CMakeLists.txt;33;CreateGTestExeAndAddTest;/home/heinemann/gpuunpacker/cbmroot/core/data/test/trd/CMakeLists.txt;0;")
