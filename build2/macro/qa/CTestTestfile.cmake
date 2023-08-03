# CMake generated Testfile for 
# Source directory: /home/heinemann/gpuunpacker/cbmroot/macro/qa
# Build directory: /home/heinemann/gpuunpacker/cbmroot/build2/macro/qa
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[qa_cleanup]=] "/usr/bin/cmake" "-P" "/home/heinemann/gpuunpacker/cbmroot/cmake/scripts/cleanmacrodir.cmake")
set_tests_properties([=[qa_cleanup]=] PROPERTIES  FIXTURES_SETUP "qa_cleanup" TIMEOUT "10000" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/qa/CMakeLists.txt;52;add_test;/home/heinemann/gpuunpacker/cbmroot/macro/qa/CMakeLists.txt;0;")
add_test([=[qa_s100e_compare]=] "/qa_compare.sh" "\"objects.yaml\"" "\"s100e\"" "\"../run/data\"" "\"../run/data/\"" "\"data/QACheckerOutput.s100e.root\"")
set_tests_properties([=[qa_s100e_compare]=] PROPERTIES  FAIL_REGULAR_EXPRESSION "segmentation violation" FIXTURES_REQUIRED "fixt_qa_sis100_electron" FIXTURES_SETUP "fixt_qa_compare_sis100_electron" PASS_REGULAR_EXPRESSION "Macro finished successfully" TIMEOUT "10000" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/qa/CMakeLists.txt;83;add_test;/home/heinemann/gpuunpacker/cbmroot/macro/qa/CMakeLists.txt;0;")
add_test([=[qa_s100m3_compare]=] "/qa_compare.sh" "\"objects.yaml\"" "\"s100m3\"" "\"../run/data\"" "\"../run/data/\"" "\"data/QACheckerOutput.s100m3.root\"")
set_tests_properties([=[qa_s100m3_compare]=] PROPERTIES  FAIL_REGULAR_EXPRESSION "segmentation violation" FIXTURES_REQUIRED "fixt_qa_sis100_muon_jpsi" FIXTURES_SETUP "fixt_qa_compare_sis100_muon_jpsi" PASS_REGULAR_EXPRESSION "Macro finished successfully" TIMEOUT "10000" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/qa/CMakeLists.txt;83;add_test;/home/heinemann/gpuunpacker/cbmroot/macro/qa/CMakeLists.txt;0;")
