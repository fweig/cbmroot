# =====   Generate the needed shell scripts   ================================
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_tra_file.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_transport_json_config.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_digi_json_config.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_tra_beam.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_digi.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_reco.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_qa.C)
# ============================================================================


# =====   Copy the .rootrc file into the directory from which root is executed
# --- Otherwise the rootalias file is not loaded
file(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc
	DESTINATION ${CBMROOT_BINARY_DIR}/macro/run)
# ============================================================================


# =====   Define variables for tests   =======================================
if(${CBM_TEST_MODEL} MATCHES Weekly)
  Set(nEvents 100)
  Set(uploadHistJPG 1)
  Set(pullDevAllowed .2)
  Set(randomSeed 1)
else()
  Set(nEvents 3)
  Set(uploadHistJPG 0)
  Set(pullDevAllowed .5)
  Set(randomSeed 1)
endIf()

math(EXPR timeOutTime "${nEvents} * 200")
math(EXPR nBeam "${nEvents} * 3")
# ============================================================================


# =====   Define the different setups to be tested with   ====================
if(NOT ${CBM_TEST_MODEL} MATCHES Experimental )
  List(APPEND cbm_setup sis100_hadron sis100_electron sis100_muon_lmvm
  	sis100_muon_jpsi sis300_electron)
else()
  List(APPEND cbm_setup sis100_electron sis100_muon_jpsi)
endif()
# ============================================================================


# =====   Cleanup the data directory   =======================================
add_test(run_cleanup ${CMAKE_COMMAND}
	-P ${CMAKE_SOURCE_DIR}/cmake/scripts/cleanmacrodir.cmake)
set_tests_properties(run_cleanup PROPERTIES
	TIMEOUT ${timeOutTime}
	FIXTURES_SETUP run_cleanup
)
# ============================================================================


# =====   Define tests for each setup   ======================================
foreach(setup IN LISTS cbm_setup)

  # --- Short name for setup
  if(setup MATCHES sis100_hadron)
  	set(sname s100h)
  elseif(setup MATCHES sis100_electron)
  	set(sname s100e)
  elseif(setup MATCHES sis100_muon_lmvm)
  	set(sname s100m2)
  elseif(setup MATCHES sis100_muon_jpsi)
  	set(sname s100m3)
  elseif(setup MATCHES sis300_electron)
  	set(sname s300e)
  else()
  	set(sname test)
  endif()

  # --- Test run_transport_json_config
  # --- Transport run using run_transport_json_config.C
  set(testname run_transport_json_config)
  add_test(${testname} ${MACRO_DIR}/run_transport_json_config.sh \"\" 2 ${randomSeed})
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
  	FIXTURES_REQUIRED run_cleanup
  	FIXTURES_SETUP fixt_tra_json_config
  	RESOURCE_LOCK json_config_ParDb
  )

  # --- Test run_digi_json_config
  # --- Digitization run with using run_digi_json_config.C
  set(testname run_digi_json_config)
  add_test(${testname} ${MACRO_DIR}/run_digi_json_config.sh)
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
  	FIXTURES_REQUIRED fixt_tra_json_config
  	FIXTURES_SETUP fixt_digi_json_config
  	RESOURCE_LOCK json_config_ParDb
  )

  # --- Test run_tra_coll
  # --- Transport run with collision events, using run_tra_file.C
  set(testname run_${sname}_tra_coll)
  set(input ${CBMROOT_SOURCE_DIR}/input/urqmd.auau.10gev.centr.root)
  add_test(${testname} ${MACRO_DIR}/run_tra_file.sh
  	\"${input}\" ${nEvents} \"data/${sname}_coll\" \"${setup}\" kGeant3 ${randomSeed})
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
  	FIXTURES_REQUIRED cleanup
  	FIXTURES_SETUP fixt_tra_coll_${setup}
  	RESOURCE_LOCK collParDb_${setup}
  )

  # --- Test run_tra_sign
  # --- Transport run with signal events, using run_tra_file.C
  set(testname run_${sname}_tra_sign)
  set(input ${CBMROOT_SOURCE_DIR}/input/pluto.auau.8gev.omega.mpmm.0001.root)
  add_test(${testname} ${MACRO_DIR}/run_tra_file.sh
  	\"${input}\" ${nEvents} \"data/${sname}_sign\" \"${setup}\" kGeant3 ${randomSeed})
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
  	FIXTURES_REQUIRED run_cleanup
  	FIXTURES_SETUP fixt_tra_sign_${setup}
  	RESOURCE_LOCK signParDb_${setup}
  )

  # --- Test run_tra_beam
  # --- Transport run with beam events, using run_tra_beam.C
  set(testname run_${sname}_tra_beam)
  add_test(${testname} ${MACRO_DIR}/run_tra_beam.sh
  	${nBeam} \"Au\" 10 -1 \"data/${sname}_beam\" \"${setup}\" kGeant3 ${randomSeed})
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
  	FIXTURES_REQUIRED run_cleanup
  	FIXTURES_SETUP fixt_tra_beam_${setup}
  	RESOURCE_LOCK beamParDb_${setup}
  )

  # --- Test run_digi_ev
  # --- Detector response simulation, event-by-event, using run_digi.C
  set(testname run_${sname}_digi_ev)
  add_test(${testname} ${MACRO_DIR}/run_digi.sh
  	\"data/${sname}_coll\" -1 \"data/${sname}_ev\" -1.)
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
	FIXTURES_REQUIRED fixt_tra_coll_${setup}
  	FIXTURES_SETUP fixt_digi_ev_${setup}
 	RESOURCE_LOCK collParDb_${setup}
  )

  # --- Test run_digi_ts
  # --- Detector response simulation, time-based, using run_digi.C
  set(testname run_${sname}_digi_ts)
  set(eventrate 1.e7)
  set(beamrate 1.e7)
  set(tslength 1.e6)
  add_test(${testname} ${MACRO_DIR}/run_digi.sh
  	\"data/${sname}_coll\" -1 \"data/${sname}_ts\"\ ${eventrate} ${tslength}
  	\"data/${sname}_sign\" \"data/${sname}_beam\" ${beamrate})
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
 	FIXTURES_REQUIRED "fixt_tra_coll_${setup};fixt_tra_sign_${setup};fixt_tra_beam_${setup}"
  	FIXTURES_SETUP fixt_digi_ts_${setup}
 	RESOURCE_LOCK collParDb_${setup}
  )

  # --- Test run_reco_ev_ideal
  # --- Event-by-event reconstruction from event-based simulation
  # --- Ideal raw event builder
  set(testname run_${sname}_reco_ev_ideal)
  add_test(${testname} ${MACRODIR}/run_reco.sh
  	\"data/${sname}_ev\" -1 0 \"data/${sname}_eb_eb_ideal\" \"Ideal\" \"${setup}\" \"data/${sname}_coll\")
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
	FIXTURES_REQUIRED fixt_digi_ev_${setup}
	FIXTURES_SETUP fixt_reco_ev_ideal_${setup}
 	RESOURCE_LOCK collParDb_${setup}
  )

  # --- Test run_reco_ev_real
  # --- Event-by-event reconstruction from event-based simulation
  # --- Real raw event builder
  set(testname run_${sname}_reco_ev_real)
  add_test(${testname} ${MACRODIR}/run_reco.sh
    \"data/${sname}_ev\" -1 0 \"data/${sname}_eb_eb_real\" \"Real\" \"${setup}\"
    \"data/${sname}_coll\")
  set_tests_properties(${testname} PROPERTIES
    TIMEOUT ${timeOutTime}
    PASS_REGULAR_EXPRESSION "Macro finished successfully"
    FIXTURES_REQUIRED fixt_digi_ev_${setup}
    FIXTURES_SETUP fixt_reco_ev_real_${setup}
    RESOURCE_LOCK collParDb_${setup}
  )

  # --- Test run_reco_ts_eb_ideal
  # --- Event-by-event reconstruction from time-based simulation
  # --- Ideal raw event builder
  set(testname run_${sname}_reco_ts_eb_ideal)
  add_test(${testname} ${MACRODIR}/run_reco.sh
  	\"data/${sname}_ts\" -1 0 \"data/${sname}_ts_eb_ideal\" \"Ideal\" \"${setup}\"
  	\"data/${sname}_coll\")
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
	FIXTURES_REQUIRED fixt_digi_ts_${setup}
	FIXTURES_SETUP fixt_reco_ts_eb_ideal_${setup}
 	RESOURCE_LOCK collParDb_${setup}
  )

  # --- Test run_reco_ts_eb_real
  # --- Event-by-event reconstruction from time-based simulation
  # --- Real raw event builder
  set(testname run_${sname}_reco_ts_eb_real)
  add_test(${testname} ${MACRODIR}/run_reco.sh
  	\"data/${sname}_ts\" -1 0 \"data/${sname}_ts_eb_real\" \"Real\" \"${setup}\"
  	\"data/${sname}_coll\")
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
	FIXTURES_REQUIRED fixt_digi_ts_${setup}
	FIXTURES_SETUP fixt_reco_ts_eb_real_${setup}
 	RESOURCE_LOCK collParDb_${setup}
  )

  # --- Test run_qa
  # --- Run QA tasks with Event-by-event reconstruction from time-based simulation
  # --- Real raw event builder
  set(testname run_${sname}_qa)
  add_test(${testname} ${MACRODIR}/run_qa.sh
  	\"data/${sname}_coll\" \"data/${sname}_ts\"  \"data/${sname}_ts_eb_real\" 
	\"data/${sname}_coll\" \"data/${sname}_qa\" \"${setup}\" -1 
	\"data/${sname}_sign\" \"data/${sname}_beam\" )
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
	FIXTURES_REQUIRED "fixt_tra_coll_${setup};fixt_digi_ts_${setup};fixt_reco_ts_eb_real_${setup}"
	FIXTURES_SETUP fixt_qa_${setup}
 	RESOURCE_LOCK collParDb_${setup}
  )

  # --- Test run_reco_ts_tb
  # --- Time-based reconstruction from time-based simulation
  set(testname run_${sname}_reco_ts_tb)
  add_test(${testname} ${MACRODIR}/run_reco.sh
  	\"data/${sname}_ts\" -1 0 \"data/${sname}_ts_tb\" \"\" \"${setup}\"
  	\"data/${sname}_coll\")
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
	FIXTURES_REQUIRED fixt_digi_ts_${setup}
	FIXTURES_SETUP fixt_reco_ts_tb_${setup}
 	RESOURCE_LOCK collParDb_${setup}
  )

  # --- Test run_transport_json_config
  # --- Transport run using run_transport_json_config.C
  set(testname run_transport_json_config)
  add_test(${testname} ${MACRO_DIR}/run_transport_json_config.sh \"\" 2 ${randomSeed})
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
  	FIXTURES_REQUIRED cleanup
  	FIXTURES_SETUP fixt_tra_json_config
  	RESOURCE_LOCK json_config_ParDb
  )

  # --- Test run_digi_json_config
  # --- Digitization run with using run_digi_json_config.C
  set(testname run_digi_json_config)
  add_test(${testname} ${MACRO_DIR}/run_digi_json_config.sh)
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
  	FIXTURES_REQUIRED fixt_tra_json_config
  	FIXTURES_SETUP fixt_digi_json_config
  	RESOURCE_LOCK json_config_ParDb
  )


endforeach(setup IN LISTS cbm_setup)
# ============================================================================


# =====   mCBM 2021 tests   ==================================================
# ====> only if test TSA files for "typical runs" present
If(DEFINED ENV{RAW_DATA_PATH} )
  SET( RAW_DATA_PATH $ENV{RAW_DATA_PATH} )

  Message( STATUS "Raw data path provided => Performing mCBM 2021 tests using tsa files in ${RAW_DATA_PATH}" )

  ## Generate scripts
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_unpack_tsa.C )  # Unpackers
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2021/check_timing_any.C )  # Check Timing
  ## Run only if raw data files present
  ForEach(RUN 1588 )
    Message( STATUS "Checking if raw data file exists: ${RAW_DATA_PATH}/${RUN}_first20Ts.tsa" )
    If(EXISTS ${RAW_DATA_PATH}/${RUN}_first20Ts.tsa)
      Message( STATUS "Raw data file found, tests will be performed for mCBM run ${RUN}" )

      ## Define tests + dependencies
      ### Raw data analysis tests: unpacking
      Set(testname mcbm_unpack_2021_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_unpack_tsa.sh \"${RAW_DATA_PATH}/${RUN}_first20Ts.tsa\" ${RUN})
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Macro finished successfully")
      set_tests_properties(${testname} PROPERTIES DEPENDS run_cleanup)
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED cleanup)
      Set(fixture_unpacked_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_unpacked_${RUN}})

      ### Raw data analysis tests: time offsets check
      Set(testname mcbm_chktime_2021_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2021/check_timing_any.sh \"${CBMROOT_BINARY_DIR}/macro/run/data/${RUN}_first20Ts.digi.root\" ${RUN})
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Macro finished successfully")
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_unpack_2021_${RUN})
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked_${RUN}})
      Set(fixture_chktime_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_chktime_${RUN}})
    EndIf() # If(EXISTS ${RAW_DATA_PATH}/${RUN}_first20Ts.tsa))
  EndForEach() # ForEach(RUN 1588 )
EndIf() # If(DEFINED ENV{RAW_DATA_PATH} )
#####################
# ============================================================================

Install(FILES .rootrc run_tra_file.C run_tra_beam.C run_transport_json_config.C run_digi_json_config.C config.json
              run_digi.C run_reco.C run_unpack_online.C run_unpack_tsa.C
        DESTINATION share/cbmroot/macro/run
       )
Install(PROGRAMS run_tests.sh
        DESTINATION share/cbmroot/macro/run
       )
Install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/run/data)")
