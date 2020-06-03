# Generate shell scripts
Set(MACRO_DIR ${CBMROOT_BINARY_DIR}/macro/KF)
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/KF/kf_thermal_signal_generator.C ${MACRO_DIR})
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/KF/kf_transport.C ${MACRO_DIR})
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_digi.C ${MACRO_DIR})
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_reco_event.C ${MACRO_DIR})
GENERATE_CBM_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/KF/kf_kfparticle.C ${MACRO_DIR})


# Put the .rootrc file into the directory from which root is executed.+
# Otherwise the rootalias file is not loaded
File(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc DESTINATION ${CBMROOT_BINARY_DIR}/macro/KF)


# Define the number of signal events to run
Set(NSignalEvents 5000)


# List of signals to be tested
List(APPEND signalTitle  K0          # 0
                         Lambda      # 1
                         Lambda_bar  # 2
                         Xi          # 3
                         Omega       # 4
                         KStar0      # 5
                         SigmaStar   # 6
                         phi         # 7
                         rhoee       # 8
                         rhomm       # 9
                         LN          # 10
                         LNN         # 11
                         H3L         # 12
                         H4L         # 13
                         pi          # 14
                         K           # 15
                         p           # 16
                         )
                         
                         
# List of corresponding signal IDs                         
List(APPEND signalId     0   # 0
                         1   # 1
                         2   # 2
                         3   # 3
                         7   # 4
                         13  # 5
                         20  # 6
                         37  # 7
                         39  # 8
                         40  # 9
                         114 # 10
                         116 # 11
                         118 # 12
                         120 # 13
                         171 # 14
                         173 # 15
                         175 # 16
                         )
                         
                         
# List of corresponding setups
List(APPEND signalSetup  sis100_electron  # 0
                         sis100_electron  # 1
                         sis100_electron  # 2
                         sis100_electron  # 3
                         sis100_electron  # 4
                         sis100_electron  # 5
                         sis100_electron  # 6
                         sis100_electron  # 7
                         sis100_electron  # 8
                         sis100_muon_lmvm # 9
                         sis100_electron  # 10
                         sis100_electron  # 11
                         sis100_electron  # 12
                         sis100_electron  # 13
                         sis100_electron  # 14
                         sis100_electron  # 15
                         sis100_electron  # 16
                         )

ForEach(iSignal RANGE 16)

  # The test dependencies are set such that parallel execution is prevented, even is there is no logical
  # dependence. The reason is that all runs (for a given setup) use the same ROOT parameter file,
  # and concurrent access to this file results in erros.
  # The logical dependences are expressed in the required "ok" files.
  
  List(GET signalTitle ${iSignal} title)
  List(GET signalId    ${iSignal} id)
  List(GET signalSetup ${iSignal} setup)
  
  # Create directory for the data, if necessary
  Set(WORKDIR ${CBMROOT_BINARY_DIR}/macro/KF/data/${title})
  File(MAKE_DIRECTORY ${WORKDIR})
  
  # Generate input events (signal)
#   Set(testname KF_thermal_signal_generator_${title})
#   Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/KF/kf_thermal_signal_generator.sh ${id} ${NSignalEvents} \"${WORKDIR}/${title}_test_Signal.root\")
#   Set_Tests_Properties(${testname} PROPERTIES WORKING_DIRECTORY ${WORKDIR})
#   Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "100")
#   Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
#   Set(fixture_generator fixture_generator_${testname})
#   set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_generator})
  
  # --- Transport run (uses KF/kf_transport.C)
  # --- For the input, not the files generated in the previous step but those predefined in the
  # --- reference directory are taken, to provide a better test stability.
  Set(testname KF_transport_${title})
 # Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/KF/kf_transport.sh ${NSignalEvents} \"${setup}\" 
 #                      \"${WORKDIR}/${title}_test\" \"${WORKDIR}/${title}_test_Signal.root\" ${id})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/KF/kf_transport.sh ${NSignalEvents} \"${setup}\" 
                       \"${WORKDIR}/${title}_test\"
                       \"${CBMROOT_SOURCE_DIR}/input/qa/KF/reference/input/${title}_test_Signal.root\" 
                       ${id})
  Set_Tests_Properties(${testname} PROPERTIES WORKING_DIRECTORY ${WORKDIR})
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "1000")
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
#   Set_Tests_Properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_generator})
  Set(fixture_transport fixture_transport_${testname})
  Set_Tests_Properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_transport})

  # --- Digitisation run, event-by-event (uses run/run_digi.C)
  Set(testname KF_digi_${title})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/KF/run_digi.sh ${NSignalEvents} \"${WORKDIR}/${title}_test\" 0. 0. 1)
  Set_Tests_Properties(${testname} PROPERTIES WORKING_DIRECTORY ${WORKDIR})
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "1400")
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  Set_Tests_Properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_transport})
  Set(fixture_digi fixture_digi_${testname})
  Set_Tests_Properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_digi})

  # --- Reconstruction run, event-by-event, with MC input, no PV finder (uses run/run_reco_event.C)
  Set(testname KF_reco_${title})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/KF/run_reco_event.sh ${NSignalEvents} \"${WORKDIR}/${title}_test\" \"${setup}\" kTRUE kFALSE)
  Set_Tests_Properties(${testname} PROPERTIES WORKING_DIRECTORY ${WORKDIR})
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "1000")
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  Set_Tests_Properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_digi})
  Set(fixture_reco fixture_reco_${testname})
  Set_Tests_Properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_reco})

  # --- KF Particle Finder run, event-by-event, with MC pid (uses KF/kf_kfparticle.C)
  Set(testname KF_kfparticle_${title})
  Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/KF/kf_kfparticle.sh ${NSignalEvents} \"${setup}\" \"${WORKDIR}/${title}_test\"  kFALSE kFALSE ${id})
  Set_Tests_Properties(${testname} PROPERTIES WORKING_DIRECTORY ${WORKDIR})
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "1000")
  Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
  Set_Tests_Properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_reco})
  Set(fixture_kfparticle fixture_kfparticle_${testname})
  Set_Tests_Properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_kfparticle})
  
 
EndForEach(iSignal)


#Install(FILES .rootrc 
#               DecayConfig.C registerGeantDecays.C registerLightIons.C registerPythiaDecays.C
#               kf_kfparticle.C kf_thermal_signal_generator.C  kf_transport.C
#        DESTINATION share/cbmroot/macro/KF
#       )
#Install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/KF/data)")
