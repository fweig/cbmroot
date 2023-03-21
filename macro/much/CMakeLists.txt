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
Set(randomSeed 1)

# =====   Cleanup the data directory   =======================================
add_test(much_cleanup ${CMAKE_COMMAND}
        -P ${CMAKE_SOURCE_DIR}/cmake/scripts/cleanmacrodir.cmake)
set_tests_properties(much_cleanup PROPERTIES
        TIMEOUT 10
        FIXTURES_SETUP much_cleanup
)

# --- Transport of UrQMD events
Set(testname much_transport)
add_test(${testname} ${MACRO_DIR}/run_transport.sh ${NumEvents} \"sis100_muon_lmvm\" \"data/muons\" \"\" \"\" kFALSE ${randomSeed})
SET_TESTS_PROPERTIES(${testname} PROPERTIES
          TIMEOUT "1200"
          PASS_REGULAR_EXPRESSION "TestPassed;All ok"
          FIXTURES_SETUP fixture_much_digi
          FIXTURES_REQUIRED much_cleanup
)

# --- Digitisation, event-by-event
Set(testname much_digi)
Add_Test(${testname} ${MACRO_DIR}/run_digi.sh \"data/muons\" ${NumEvents} \"data/muons.event\" -1.)
Set_Tests_Properties(${testname} PROPERTIES
          TIMEOUT "1200"
          PASS_REGULAR_EXPRESSION "Macro finished successfully"
          FIXTURES_REQUIRED fixture_much_digi
          FIXTURES_SETUP fixture_much_reco
)

# --- Reconstruction, event-by-event
Set(testname much_reco_event)
add_test(${testname} ${MACRO_DIR}/run_reco_event.sh ${NumEvents} \"data/muons\" \"sis100_muon_lmvm\" 1 1)
SET_TESTS_PROPERTIES(${testname} PROPERTIES
          TIMEOUT "1200"
          PASS_REGULAR_EXPRESSION "TestPassed;All ok"
          FIXTURES_REQUIRED fixture_much_reco
          FIXTURES_SETUP fixture_much_ana
)

# --- Analysis, event-by-event
Set(testname much_analysis)
add_test(${testname} ${MACRO_DIR}/run_ana.sh ${NumEvents} \"data/muons\" \"sis100_muon_lmvm\" 1 \"\" -1)
SET_TESTS_PROPERTIES(${testname} PROPERTIES
          TIMEOUT "1200"
          PASS_REGULAR_EXPRESSION "TestPassed;All ok"
          FIXTURES_REQUIRED fixture_much_ana
)


Install(FILES .rootrc run_transport.C run_ana.C
        DESTINATION share/cbmroot/macro/much
       )

Install(DIRECTORY data
        DESTINATION share/cbmroot/macro/much
       )
