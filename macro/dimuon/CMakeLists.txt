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

# Define the different setups to be tested
If( ($ENV{ctest_model} MATCHES Nightly) OR ($ENV{ctest_model} MATCHES Weekly) )
  List(APPEND cbm_setup sis100_muon_lmvm)

  ForEach(setup IN LISTS cbm_setup)

    # The test dependencies are set such that parallel execution is prevented, even is there is no logical
    # dependence. The reason is that all runs (for a given setup) use the same ROOT parameter file,
    # and concurrent access to this file results in erros.
    # The logical dependences are expressed in the required "ok" files.

    # --- Transport run, using run_transport.C
    Set(testname dimuon_transport_${setup})
    Add_Test(${testname} ${MACRO_DIR}/dimuon_transport.sh ${NumEvents} \"${setup}\" \"data/${setup}_test\")
    Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
    Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
    Set(fixture_dimuon_digi fixture_dimuon_digi_${testname})
    set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_dimuon_digi})
  
    # --- Digitisation run, event-by-event
    Set(testname dimuon_digi_event_${setup})
    Add_Test(${testname} ${MACRO_DIR}/run_digi.sh ${NumEvents} \"data/${setup}_test\" 0. 0. 1)
    Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
    Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
    set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_dimuon_digi})
    Set(fixture_dimuon_reco_event fixture_dimuon_reco_event_${testname})
    set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_dimuon_reco_event})

    # --- Reconstruction run, event-by-event
    Set(testname dimuon_reco_event_${setup})
    Add_Test(${testname} ${MACRO_DIR}/run_reco_event.sh ${NumEvents} \"data/${setup}_test\" \"${setup}\" 1 1)
    Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "800")
    Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
    set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_dimuon_reco_event})
    Set(fixture_dimuon_ana fixture_dimuon_ana_${testname})
    set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_dimuon_ana})

    # --- Much analysis
    Set(testname dimuon_analysis_${setup})
    Add_Test(${testname} ${MACRO_DIR}/run_ana.sh ${NumEvents} \"data/${setup}_test\" \"${setup}\" 1 \"\" -1)
    Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
    Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
    set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_dimuon_ana})
    Set(fixture_dimuon_mass fixture_dimuon_mass_${testname})
    set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_dimuon_mass})
  
    # --- Invariant Mass Spectrum
    Set(testname dimuon_inv_mass_${setup})
    Add_Test(${testname} ${MACRO_DIR}/invM.sh \"data/${setup}_test\")
    Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "100")
    Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
    set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_dimuon_mass})
  
  EndForEach(setup IN LISTS cbm_setup)
  # end of test CBM setups from geometry/setup
EndIf()

Install(FILES .rootrc dimuon_transport.C 
        DESTINATION share/cbmroot/macro/dimuon
       )
Install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/run/data)")
