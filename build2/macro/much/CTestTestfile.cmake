# CMake generated Testfile for 
# Source directory: /home/heinemann/gpuunpacker/cbmroot/macro/much
# Build directory: /home/heinemann/gpuunpacker/cbmroot/build2/macro/much
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[much_cleanup]=] "/usr/bin/cmake" "-P" "/home/heinemann/gpuunpacker/cbmroot/cmake/scripts/cleanmacrodir.cmake")
set_tests_properties([=[much_cleanup]=] PROPERTIES  FIXTURES_SETUP "much_cleanup" TIMEOUT "10" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/much/CMakeLists.txt;16;add_test;/home/heinemann/gpuunpacker/cbmroot/macro/much/CMakeLists.txt;0;")
add_test([=[much_transport]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/much/run_transport.sh" "3" "\"sis100_muon_lmvm\"" "\"data/muons\"" "\"\"" "\"\"" "kFALSE" "1")
set_tests_properties([=[much_transport]=] PROPERTIES  FIXTURES_REQUIRED "much_cleanup" FIXTURES_SETUP "fixture_much_digi" PASS_REGULAR_EXPRESSION "TestPassed;All ok" TIMEOUT "1200" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/much/CMakeLists.txt;25;add_test;/home/heinemann/gpuunpacker/cbmroot/macro/much/CMakeLists.txt;0;")
add_test([=[much_digi]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/much/run_digi.sh" "\"data/muons\"" "3" "\"data/muons.event\"" "-1.")
set_tests_properties([=[much_digi]=] PROPERTIES  FIXTURES_REQUIRED "fixture_much_digi" FIXTURES_SETUP "fixture_much_reco" PASS_REGULAR_EXPRESSION "Macro finished successfully" TIMEOUT "1200" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/much/CMakeLists.txt;35;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/much/CMakeLists.txt;0;")
add_test([=[much_reco_event]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/much/run_reco_event.sh" "3" "\"data/muons\"" "\"sis100_muon_lmvm\"" "1" "1")
set_tests_properties([=[much_reco_event]=] PROPERTIES  FIXTURES_REQUIRED "fixture_much_reco" FIXTURES_SETUP "fixture_much_ana" PASS_REGULAR_EXPRESSION "TestPassed;All ok" TIMEOUT "1200" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/much/CMakeLists.txt;45;add_test;/home/heinemann/gpuunpacker/cbmroot/macro/much/CMakeLists.txt;0;")
add_test([=[much_analysis]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/much/run_ana.sh" "3" "\"data/muons\"" "\"sis100_muon_lmvm\"" "1" "\"\"" "-1")
set_tests_properties([=[much_analysis]=] PROPERTIES  FIXTURES_REQUIRED "fixture_much_ana" PASS_REGULAR_EXPRESSION "TestPassed;All ok" TIMEOUT "1200" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/much/CMakeLists.txt;55;add_test;/home/heinemann/gpuunpacker/cbmroot/macro/much/CMakeLists.txt;0;")
