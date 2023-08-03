# CMake generated Testfile for 
# Source directory: /home/heinemann/gpuunpacker/cbmroot/macro/mvd/qa
# Build directory: /home/heinemann/gpuunpacker/cbmroot/build2/macro/mvd/qa
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[mvd_cleanup]=] "/usr/bin/cmake" "-P" "/home/heinemann/gpuunpacker/cbmroot/cmake/scripts/cleanmacrodir.cmake")
set_tests_properties([=[mvd_cleanup]=] PROPERTIES  FIXTURES_SETUP "mvd_cleanup" TIMEOUT "10" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mvd/qa/CMakeLists.txt;6;add_test;/home/heinemann/gpuunpacker/cbmroot/macro/mvd/qa/CMakeLists.txt;0;")
add_test([=[mvd_qa1_transUrqmd]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mvd/qa/mvd_qa1_transUrqmd.sh")
set_tests_properties([=[mvd_qa1_transUrqmd]=] PROPERTIES  FIXTURES_REQUIRED "mvd_cleanup" FIXTURES_SETUP "fixture_mvd_qa3_digitize" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mvd/qa/CMakeLists.txt;15;add_test;/home/heinemann/gpuunpacker/cbmroot/macro/mvd/qa/CMakeLists.txt;0;")
add_test([=[mvd_qa2_transDelta]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mvd/qa/mvd_qa2_transDelta.sh")
set_tests_properties([=[mvd_qa2_transDelta]=] PROPERTIES  FIXTURES_REQUIRED "mvd_cleanup" FIXTURES_SETUP "fixture_mvd_qa3_digitize" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "500" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mvd/qa/CMakeLists.txt;24;add_test;/home/heinemann/gpuunpacker/cbmroot/macro/mvd/qa/CMakeLists.txt;0;")
add_test([=[mvd_qa3_digitize]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mvd/qa/mvd_qa3_digitize.sh")
set_tests_properties([=[mvd_qa3_digitize]=] PROPERTIES  FIXTURES_REQUIRED "fixture_mvd_qa3_digitize" FIXTURES_SETUP "fixture_mvd_qa4_reco" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mvd/qa/CMakeLists.txt;33;add_test;/home/heinemann/gpuunpacker/cbmroot/macro/mvd/qa/CMakeLists.txt;0;")
add_test([=[mvd_qa4_reco]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mvd/qa/mvd_qa4_reco.sh")
set_tests_properties([=[mvd_qa4_reco]=] PROPERTIES  FIXTURES_REQUIRED "fixture_mvd_qa4_reco" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mvd/qa/CMakeLists.txt;42;add_test;/home/heinemann/gpuunpacker/cbmroot/macro/mvd/qa/CMakeLists.txt;0;")
