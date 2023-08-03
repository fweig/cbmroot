# CMake generated Testfile for 
# Source directory: /home/heinemann/gpuunpacker/cbmroot/macro/mcbm
# Build directory: /home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[mcbm_cleanup]=] "/usr/bin/cmake" "-P" "/home/heinemann/gpuunpacker/cbmroot/cmake/scripts/cleanmacrodir.cmake")
set_tests_properties([=[mcbm_cleanup]=] PROPERTIES  FIXTURES_SETUP "mcbm_cleanup" TIMEOUT "10" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;43;add_test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_transport_mcbm_beam_2021_07_surveyed]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_transport.sh" "10" "\"mcbm_beam_2021_07_surveyed\"" "\"data/mcbm_beam_2021_07_surveyed_test\"" "\"\"" "kTRUE" "1")
set_tests_properties([=[mcbm_transport_mcbm_beam_2021_07_surveyed]=] PROPERTIES  FIXTURES_REQUIRED "mcbm_cleanup" FIXTURES_SETUP "fixture_mcbm_transport_mcbm_beam_2021_07_surveyed" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "600" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;57;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_digi_event_mcbm_beam_2021_07_surveyed]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_digi.sh" "10" "\"data/mcbm_beam_2021_07_surveyed_test\"" "0." "0." "1")
set_tests_properties([=[mcbm_digi_event_mcbm_beam_2021_07_surveyed]=] PROPERTIES  FIXTURES_REQUIRED "fixture_mcbm_transport_mcbm_beam_2021_07_surveyed" FIXTURES_SETUP "fixture_mcbm_digi_event_mcbm_beam_2021_07_surveyed" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;68;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_reco_event_mcbm_beam_2021_07_surveyed]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_reco_event.sh" "10" "\"data/mcbm_beam_2021_07_surveyed_test\"" "\"mcbm_beam_2021_07_surveyed\"")
set_tests_properties([=[mcbm_reco_event_mcbm_beam_2021_07_surveyed]=] PROPERTIES  FIXTURES_REQUIRED "fixture_mcbm_digi_event_mcbm_beam_2021_07_surveyed" FIXTURES_SETUP "fixture_mcbm_reco_event_mcbm_beam_2021_07_surveyed" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;78;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_match_check_mcbm_beam_2021_07_surveyed]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_match_check.sh" "10" "\"mcbm_beam_2021_07_surveyed\"" "\"data/mcbm_beam_2021_07_surveyed_test\"")
set_tests_properties([=[mcbm_match_check_mcbm_beam_2021_07_surveyed]=] PROPERTIES  FIXTURES_REQUIRED "fixture_mcbm_reco_event_mcbm_beam_2021_07_surveyed" FIXTURES_SETUP "fixture_mcbm_match_check_mcbm_beam_2021_07_surveyed" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;88;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_hadron_analysis_mcbm_beam_2021_07_surveyed]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_hadron_analysis.sh" "10" "\"mcbm_beam_2021_07_surveyed\"" "\"data/mcbm_beam_2021_07_surveyed_test\"")
set_tests_properties([=[mcbm_hadron_analysis_mcbm_beam_2021_07_surveyed]=] PROPERTIES  FIXTURES_REQUIRED "fixture_mcbm_match_check_mcbm_beam_2021_07_surveyed" FIXTURES_SETUP "fixture_mcbm_hadron_analysis_mcbm_beam_2021_07_surveyed" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;98;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_digi_mcbm_beam_2021_07_surveyed]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_digi.sh" "10" "\"data/mcbm_beam_2021_07_surveyed_test\"")
set_tests_properties([=[mcbm_digi_mcbm_beam_2021_07_surveyed]=] PROPERTIES  FIXTURES_REQUIRED "fixture_mcbm_hadron_analysis_mcbm_beam_2021_07_surveyed" FIXTURES_SETUP "fixture_mcbm_digi_mcbm_beam_2021_07_surveyed" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;108;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_qa_event_mc_mcbm_beam_2021_07_surveyed]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_qa.sh" "10" "\"data/mcbm_beam_2021_07_surveyed_test\"" "\"mcbm_beam_2021_07_surveyed\"")
set_tests_properties([=[mcbm_qa_event_mc_mcbm_beam_2021_07_surveyed]=] PROPERTIES  FAIL_REGULAR_EXPRESSION "FATAL" FIXTURES_REQUIRED "fixture_mcbm_digi_mcbm_beam_2021_07_surveyed" FIXTURES_SETUP "fixture_mcbm_qa_event_mc_mcbm_beam_2021_07_surveyed" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;118;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_overlap_mcbm_beam_2021_07_surveyed]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_check_overlaps.sh" "\"data/mcbm_beam_2021_07_surveyed_test\"")
set_tests_properties([=[mcbm_overlap_mcbm_beam_2021_07_surveyed]=] PROPERTIES  FIXTURES_REQUIRED "fixture_mcbm_qa_event_mc_mcbm_beam_2021_07_surveyed" FIXTURES_SETUP "fixture_mcbm_overlap_mcbm_beam_2021_07_surveyed" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "600" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;138;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_transport_mcbm_beam_2022_03]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_transport.sh" "10" "\"mcbm_beam_2022_03\"" "\"data/mcbm_beam_2022_03_test\"" "\"\"" "kTRUE" "1")
set_tests_properties([=[mcbm_transport_mcbm_beam_2022_03]=] PROPERTIES  FIXTURES_REQUIRED "mcbm_cleanup" FIXTURES_SETUP "fixture_mcbm_transport_mcbm_beam_2022_03" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "600" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;57;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_digi_event_mcbm_beam_2022_03]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_digi.sh" "10" "\"data/mcbm_beam_2022_03_test\"" "0." "0." "1")
set_tests_properties([=[mcbm_digi_event_mcbm_beam_2022_03]=] PROPERTIES  FIXTURES_REQUIRED "fixture_mcbm_transport_mcbm_beam_2022_03" FIXTURES_SETUP "fixture_mcbm_digi_event_mcbm_beam_2022_03" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;68;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_reco_event_mcbm_beam_2022_03]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_reco_event.sh" "10" "\"data/mcbm_beam_2022_03_test\"" "\"mcbm_beam_2022_03\"")
set_tests_properties([=[mcbm_reco_event_mcbm_beam_2022_03]=] PROPERTIES  FIXTURES_REQUIRED "fixture_mcbm_digi_event_mcbm_beam_2022_03" FIXTURES_SETUP "fixture_mcbm_reco_event_mcbm_beam_2022_03" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;78;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_match_check_mcbm_beam_2022_03]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_match_check.sh" "10" "\"mcbm_beam_2022_03\"" "\"data/mcbm_beam_2022_03_test\"")
set_tests_properties([=[mcbm_match_check_mcbm_beam_2022_03]=] PROPERTIES  FIXTURES_REQUIRED "fixture_mcbm_reco_event_mcbm_beam_2022_03" FIXTURES_SETUP "fixture_mcbm_match_check_mcbm_beam_2022_03" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;88;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_hadron_analysis_mcbm_beam_2022_03]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_hadron_analysis.sh" "10" "\"mcbm_beam_2022_03\"" "\"data/mcbm_beam_2022_03_test\"")
set_tests_properties([=[mcbm_hadron_analysis_mcbm_beam_2022_03]=] PROPERTIES  FIXTURES_REQUIRED "fixture_mcbm_match_check_mcbm_beam_2022_03" FIXTURES_SETUP "fixture_mcbm_hadron_analysis_mcbm_beam_2022_03" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;98;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_digi_mcbm_beam_2022_03]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_digi.sh" "10" "\"data/mcbm_beam_2022_03_test\"")
set_tests_properties([=[mcbm_digi_mcbm_beam_2022_03]=] PROPERTIES  FIXTURES_REQUIRED "fixture_mcbm_hadron_analysis_mcbm_beam_2022_03" FIXTURES_SETUP "fixture_mcbm_digi_mcbm_beam_2022_03" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;108;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_qa_event_mc_mcbm_beam_2022_03]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_qa.sh" "10" "\"data/mcbm_beam_2022_03_test\"" "\"mcbm_beam_2022_03\"")
set_tests_properties([=[mcbm_qa_event_mc_mcbm_beam_2022_03]=] PROPERTIES  FAIL_REGULAR_EXPRESSION "FATAL" FIXTURES_REQUIRED "fixture_mcbm_digi_mcbm_beam_2022_03" FIXTURES_SETUP "fixture_mcbm_qa_event_mc_mcbm_beam_2022_03" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "300" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;118;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
add_test([=[mcbm_overlap_mcbm_beam_2022_03]=] "/home/heinemann/gpuunpacker/cbmroot/build2/macro/mcbm/mcbm_check_overlaps.sh" "\"data/mcbm_beam_2022_03_test\"")
set_tests_properties([=[mcbm_overlap_mcbm_beam_2022_03]=] PROPERTIES  FIXTURES_REQUIRED "fixture_mcbm_qa_event_mc_mcbm_beam_2022_03" FIXTURES_SETUP "fixture_mcbm_overlap_mcbm_beam_2022_03" PASS_REGULAR_EXPRESSION "Test Passed;All ok" TIMEOUT "600" _BACKTRACE_TRIPLES "/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;138;Add_Test;/home/heinemann/gpuunpacker/cbmroot/macro/mcbm/CMakeLists.txt;0;")
