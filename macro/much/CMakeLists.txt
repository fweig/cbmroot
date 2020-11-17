# start of test CBM setups from geometry/setup
Set(MACRO_DIR ${CBMROOT_BINARY_DIR}/macro/much)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/much/run_transport.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/much/run_ana.C)
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_digi.C ${MACRO_DIR})
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_reco_event.C ${MACRO_DIR})

# Put the .rootrc file into the directory from which root is executed.
# Otherwise the rootalias file is not loaded
File(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc DESTINATION ${CBMROOT_BINARY_DIR}/macro/much)

Set(NumEvents 3)

# --- Transport of UrQMD events
Set(testname much_transport)
add_test(${testname} ${MACRO_DIR}/run_transport.sh ${NumEvents} \"sis100_muon_lmvm\" \"muons\")
SET_TESTS_PROPERTIES(${testname} PROPERTIES TIMEOUT "1200")
SET_TESTS_PROPERTIES(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "TestPassed;All ok")
set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP fixture_much_digi)

# --- Digitisation, event-by-event
Set(testname much_digi)
Add_Test(${testname} ${MACRO_DIR}/run_digi.sh \"muons\" ${NumEvents} \"muons.event\" -1.)
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "1200")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Macro finished successfully")
set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED fixture_much_digi)
set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP fixture_much_reco)

# --- Reconstruction, event-by-event
Set(testname much_reco_event)
add_test(${testname} ${MACRO_DIR}/run_reco_event.sh ${NumEvents} \"muons\" \"sis100_muon_lmvm\" 1 1)
SET_TESTS_PROPERTIES(${testname} PROPERTIES TIMEOUT "1200")
SET_TESTS_PROPERTIES(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "TestPassed;All ok")
set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED fixture_much_reco)
set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP fixture_much_ana)

# --- Analysis, event-by-event
Set(testname much_analysis)
add_test(${testname} ${MACRO_DIR}/run_ana.sh ${NumEvents} \"muons\" \"sis100_muon_lmvm\" 1 \"\" -1)
SET_TESTS_PROPERTIES(${testname} PROPERTIES TIMEOUT "1200")
SET_TESTS_PROPERTIES(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "TestPassed;All ok")
set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED fixture_much_ana)


#Install(FILES .rootrc setup_much_test.C 
#        DESTINATION share/cbmroot/macro/much
#       )
#Install(FILES data/jpsi.root
#              data/much_digi_sector_4station.seg
#        DESTINATION share/cbmroot/macro/much/data
#       )

#Install(CODE "FILE(MAKE_DIRECTORY
#${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/much/data)")
