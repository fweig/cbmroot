# start of test CBM setups from geometry/setup
Set(MACRO_DIR ${CBMROOT_BINARY_DIR}/macro/dimuon)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/dimuon/dimuon_transport.C)
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_digi.C ${MACRO_DIR})
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_reco_event.C ${MACRO_DIR})
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/much/run_ana.C ${MACRO_DIR})
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/dimuon/invM.C)

# Put the .rootrc file into the directory from which root is executed.
# Otherwise the rootalias file is not loaded
File(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc DESTINATION ${CBMROOT_BINARY_DIR}/macro/dimuon)
File(COPY ${CBMROOT_SOURCE_DIR}/parameters/much/TOF8gev_fitParam_sigma2.sis100_muon_lmvm.root DESTINATION ${CBMROOT_BINARY_DIR}/macro/dimuon)

# Define the number of events to run
Set(NumEvents 1000)
Set(randomSeed 1)

# Define the different setups to be tested
List(APPEND cbm_setup sis100_muon_lmvm)

# =====   Cleanup the data directory   =======================================
add_test(dimuon_cleanup ${CMAKE_COMMAND}
        -P ${CMAKE_SOURCE_DIR}/cmake/scripts/cleanmacrodir.cmake)
set_tests_properties(dimuon_cleanup PROPERTIES
        TIMEOUT 10
        FIXTURES_SETUP dimuon_cleanup
)


ForEach(setup IN LISTS cbm_setup)

    # The test dependencies are set such that parallel execution is prevented, even is there is no logical
    # dependence. The reason is that all runs (for a given setup) use the same ROOT parameter file,
    # and concurrent access to this file results in erros.
    # The logical dependences are expressed in the required "ok" files.

    # --- Transport run, using run_transport.C
    Set(testname dimuon_transport_${setup})
    Set(fixture_dimuon_digi fixture_dimuon_digi_${testname})
    Add_Test(${testname} ${MACRO_DIR}/dimuon_transport.sh ${NumEvents} \"${setup}\" \"data/${setup}_test\" \"\" kTRUE ${randomSeed})
    Set_Tests_Properties(${testname} PROPERTIES 
            TIMEOUT "600"
            PASS_REGULAR_EXPRESSION "Test Passed;All ok"
            FIXTURES_REQUIRED dimuon_cleanup
            FIXTURES_SETUP ${fixture_dimuon_digi}
    )
  
    # --- Digitisation run, event-by-event
    Set(testname dimuon_digi_event_${setup})
    Set(fixture_dimuon_reco_event fixture_dimuon_reco_event_${testname})
    Add_Test(${testname} ${MACRO_DIR}/run_digi.sh \"data/${setup}_test\" ${NumEvents} 
    	\"data/${setup}_test.event\" -1.)
    Set_Tests_Properties(${testname} PROPERTIES 
            TIMEOUT "300"
            PASS_REGULAR_EXPRESSION "Macro finished successfully"
            FIXTURES_REQUIRED ${fixture_dimuon_digi}
            FIXTURES_SETUP ${fixture_dimuon_reco_event}
    )

    # --- Reconstruction run, event-by-event
    Set(testname dimuon_reco_event_${setup})
    Set(fixture_dimuon_ana fixture_dimuon_ana_${testname})
    Add_Test(${testname} ${MACRO_DIR}/run_reco_event.sh ${NumEvents} \"data/${setup}_test\" \"${setup}\" 1 1)
    Set_Tests_Properties(${testname} PROPERTIES
            TIMEOUT "800"
            PASS_REGULAR_EXPRESSION "Test Passed;All ok"
            FIXTURES_REQUIRED ${fixture_dimuon_reco_event}
            FIXTURES_SETUP ${fixture_dimuon_ana}
    )

    # --- Much analysis
    Set(testname dimuon_analysis_${setup})
    Set(fixture_dimuon_mass fixture_dimuon_mass_${testname})
    Add_Test(${testname} ${MACRO_DIR}/run_ana.sh ${NumEvents} \"data/${setup}_test\" \"${setup}\" 1 \"\" -1)
    Set_Tests_Properties(${testname} PROPERTIES 
            TIMEOUT "300"
            PASS_REGULAR_EXPRESSION "Test Passed;All ok"
            FIXTURES_REQUIRED ${fixture_dimuon_ana}
            FIXTURES_SETUP ${fixture_dimuon_mass}
    )
  
    # --- Invariant Mass Spectrum
    Set(testname dimuon_inv_mass_${setup})
    Add_Test(${testname} ${MACRO_DIR}/invM.sh \"data/${setup}_test\")
    Set_Tests_Properties(${testname} PROPERTIES 
            TIMEOUT "100"
            PASS_REGULAR_EXPRESSION "Test Passed;All ok"
            FIXTURES_REQUIRED ${fixture_dimuon_mass}
    )
  
EndForEach(setup IN LISTS cbm_setup)
# end of test CBM setups from geometry/setup

# Installation of macros is done in macro/CMakeLists.txt to install the
# macros even if the tests are not executed
