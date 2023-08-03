# CMake generated Testfile for 
# Source directory: /home/heinemann/gpuunpacker/cbmroot/core/data/test/psd
# Build directory: /home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/psd
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[_GTestCbmPsdDigi]=] "/home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/psd/_GTestCbmPsdDigi.sh")
set_tests_properties([=[_GTestCbmPsdDigi]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/core/data/test/CMakeLists.txt;10;Add_Test;/home/heinemann/gpuunpacker/cbmroot/core/data/test/psd/CMakeLists.txt;18;CreateGTestExeAndAddTest;/home/heinemann/gpuunpacker/cbmroot/core/data/test/psd/CMakeLists.txt;0;")
add_test([=[_GTestCbmPsdAddress]=] "/home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/psd/_GTestCbmPsdAddress.sh")
set_tests_properties([=[_GTestCbmPsdAddress]=] PROPERTIES  DEPENDS "_GTestPsdDigi" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/core/data/test/CMakeLists.txt;10;Add_Test;/home/heinemann/gpuunpacker/cbmroot/core/data/test/psd/CMakeLists.txt;25;CreateGTestExeAndAddTest;/home/heinemann/gpuunpacker/cbmroot/core/data/test/psd/CMakeLists.txt;0;")
add_test([=[_GTestCbmPsdPoint]=] "/home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/psd/_GTestCbmPsdPoint.sh")
set_tests_properties([=[_GTestCbmPsdPoint]=] PROPERTIES  DEPENDS "_GTestPsdAddress" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/core/data/test/CMakeLists.txt;10;Add_Test;/home/heinemann/gpuunpacker/cbmroot/core/data/test/psd/CMakeLists.txt;32;CreateGTestExeAndAddTest;/home/heinemann/gpuunpacker/cbmroot/core/data/test/psd/CMakeLists.txt;0;")
add_test([=[_GTestCbmPsdHit]=] "/home/heinemann/gpuunpacker/cbmroot/build2/core/data/test/psd/_GTestCbmPsdHit.sh")
set_tests_properties([=[_GTestCbmPsdHit]=] PROPERTIES  DEPENDS "_GTestPsdPoint" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/core/data/test/CMakeLists.txt;10;Add_Test;/home/heinemann/gpuunpacker/cbmroot/core/data/test/psd/CMakeLists.txt;38;CreateGTestExeAndAddTest;/home/heinemann/gpuunpacker/cbmroot/core/data/test/psd/CMakeLists.txt;0;")
