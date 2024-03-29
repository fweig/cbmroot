# =====   Generate the needed shell scripts   ================================
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_tra_file.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_tra_beam.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_digi.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_reco.C)
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_qa.C)

Set(MACRO_DIR ${CMAKE_CURRENT_BINARY_DIR})
# ============================================================================


# =====   Copy the .rootrc file into the directory from which root is executed
# --- Otherwise the rootalias file is not loaded
file(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc
	DESTINATION ${CBMROOT_BINARY_DIR}/macro/run)
# ============================================================================

# =====   Copy configuration file for QA tasks to the build directory ========
file(COPY ${CBMROOT_SOURCE_DIR}/macro/run/qa_config.cbm.yaml
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
  List(APPEND cbm_setup
	sis100_hadron
        sis100_electron
	sis100_muon_lmvm
        sis100_muon_jpsi
	sis300_electron
	)
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

  # --- Test run_tra_coll
  # --- Transport run with collision events, using run_tra_file.C
  set(testname run_${sname}_tra_coll)
  set(input ${CBMROOT_SOURCE_DIR}/input/urqmd.auau.10gev.centr.root)
  add_test(${testname} ${MACRO_DIR}/run_tra_file.sh
  	\"${input}\" ${nEvents} \"data/${sname}_coll\" \"${setup}\" kGeant3 ${randomSeed} kTRUE)
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
	FAIL_REGULAR_EXPRESSION "segmentation violation"
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
        FIXTURES_REQUIRED run_cleanup
  	FIXTURES_SETUP fixt_tra_coll_${setup}
  	RESOURCE_LOCK collParDb_${setup}
  )

  # --- Test run_tra_sign
  # --- Transport run with signal events, using run_tra_file.C
  set(testname run_${sname}_tra_sign)
  set(input ${CBMROOT_SOURCE_DIR}/input/pluto.auau.8gev.omega.mpmm.0001.root)
  add_test(${testname} ${MACRO_DIR}/run_tra_file.sh
  	\"${input}\" ${nEvents} \"data/${sname}_sign\" \"${setup}\" kGeant3 ${randomSeed} kTRUE)
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
	FAIL_REGULAR_EXPRESSION "segmentation violation"
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
  	FIXTURES_REQUIRED run_cleanup
  	FIXTURES_SETUP fixt_tra_sign_${setup}
  	RESOURCE_LOCK signParDb_${setup}
  )

  # --- Test run_tra_beam
  # --- Transport run with beam events, using run_tra_beam.C
  set(testname run_${sname}_tra_beam)
  add_test(${testname} ${MACRO_DIR}/run_tra_beam.sh
  	${nBeam} \"Au\" 10 -1 \"data/${sname}_beam\" \"${setup}\" kGeant3 ${randomSeed} kTRUE)
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
	FAIL_REGULAR_EXPRESSION "segmentation violation"
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
	FAIL_REGULAR_EXPRESSION "segmentation violation"
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
	FAIL_REGULAR_EXPRESSION "segmentation violation"
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
 	FIXTURES_REQUIRED "fixt_tra_coll_${setup};fixt_tra_sign_${setup};fixt_tra_beam_${setup}"
  	FIXTURES_SETUP fixt_digi_ts_${setup}
 	RESOURCE_LOCK collParDb_${setup}
  )

  # --- Test run_reco_ev_ideal
  # --- Event-by-event reconstruction from event-based simulation
  # --- Ideal raw event builder
  set(testname run_${sname}_reco_ev_ideal)
  add_test(${testname}	 ${MACRODIR}/run_reco.sh
  	\"data/${sname}_ev\" -1 0 \"data/${sname}_eb_eb_ideal\" \"Ideal\" \"${setup}\" \"data/${sname}_coll\")
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
	FAIL_REGULAR_EXPRESSION "segmentation violation"
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
    FAIL_REGULAR_EXPRESSION "segmentation violation"
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
	FAIL_REGULAR_EXPRESSION "segmentation violation"
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
	FAIL_REGULAR_EXPRESSION "segmentation violation"
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
	\"data/${sname}_sign\" \"data/${sname}_beam\" \"qa_config.cbm.yaml\")
  set_tests_properties(${testname} PROPERTIES
  	TIMEOUT ${timeOutTime}
	FAIL_REGULAR_EXPRESSION "segmentation violation"
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
	FAIL_REGULAR_EXPRESSION "segmentation violation"
  	PASS_REGULAR_EXPRESSION "Macro finished successfully"
	FIXTURES_REQUIRED fixt_digi_ts_${setup}
	FIXTURES_SETUP fixt_reco_ts_tb_${setup}
 	RESOURCE_LOCK collParDb_${setup}
  )


endforeach(setup IN LISTS cbm_setup)
# ============================================================================


# =====   mCBM 2021 tests   ==================================================
# ====> only if test TSA files for "typical runs" present
If(DEFINED ENV{RAW_DATA_PATH} )
  SET( RAW_DATA_PATH $ENV{RAW_DATA_PATH} )

  Message( STATUS "Raw data path provided => Performing mCBM 2021 tests using tsa files in ${RAW_DATA_PATH}" )

  ## Generate scripts
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/create_mcbm_geo_setup.C )  # Geofile creation
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/run/run_unpack_tsa.C )  # Unpackers
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2021/check_timing_any.C )  # Check Timing mCBM 2021
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2022/mcbm_event.C )  # Event building mCBM 2022
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2022/mcbm_reco.C )  # Reconstruction + tracking mCBM 2022

  ## Run only if raw data files present: 2021
  # =====>   Set the timeouts depending on test model (profiling = slow)
  SET( RAW_DATA_UNPACK_TO 500) # Nightly mode on run4: 298 for run 1588
  SET( RAW_DATA_CHECK_TO  600) # Nightly mode on run4: 562 for run 1588
  if(${CBM_TEST_MODEL} MATCHES Weekly OR ${CBM_TEST_MODEL} MATCHES Profile )
    Message( STATUS "Profiling model detected, increasing timeout for mCBM 2021 tests with coverage." )
    SET( RAW_DATA_UNPACK_TO 1600) # Coverage mode: 716 for run 1588 on run4
    SET( RAW_DATA_CHECK_TO  1000) # Coverage mode: 654 for run 1588 on run4
  endif()
  # ====================================================================

  ForEach(RUN 1588 )
    Message( STATUS "Checking if raw data file exists: ${RAW_DATA_PATH}/${RUN}_first20Ts.tsa" )
    If(EXISTS ${RAW_DATA_PATH}/${RUN}_first20Ts.tsa)
      Message( STATUS "Raw data file found, tests will be performed for mCBM run ${RUN}" )

      ## Define tests + dependencies
      ### Raw data analysis tests: unpacking
      Set(testname mcbm_unpack_2021_${RUN})
      Set(fixture_unpacked_${RUN} fixture_done_${testname})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_unpack_tsa.sh \"${RAW_DATA_PATH}/${RUN}_first20Ts.tsa\" ${RUN})
      Set_Tests_Properties(${testname} PROPERTIES
            TIMEOUT ${RAW_DATA_UNPACK_TO}
            FAIL_REGULAR_EXPRESSION "segmentation violation"
            PASS_REGULAR_EXPRESSION "Macro finished successfully"
            FIXTURES_REQUIRED run_cleanup
            FIXTURES_SETUP ${fixture_unpacked_${RUN}}
            )

      ### Raw data analysis tests: time offsets check
      Set(testname mcbm_chktime_2021_${RUN})
      Set(fixture_chktime_${RUN} fixture_done_${testname})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2021/check_timing_any.sh \"${CBMROOT_BINARY_DIR}/macro/run/data/${RUN}_first20Ts.digi.root\" ${RUN})
      Set_Tests_Properties(${testname} PROPERTIES
            TIMEOUT ${RAW_DATA_CHECK_TO}
            FAIL_REGULAR_EXPRESSION "segmentation violation"
            PASS_REGULAR_EXPRESSION "Macro finished successfully"
            FIXTURES_REQUIRED ${fixture_unpacked_${RUN}}
            FIXTURES_SETUP ${fixture_chktime_${RUN}}
            )
    EndIf() # If(EXISTS ${RAW_DATA_PATH}/${RUN}_first20Ts.tsa))
  EndForEach() # ForEach(RUN 1588 )

  ## Run only if raw data files present: 2022
  # =====>   Set the timeouts and data range depending on test model (profiling = slow)
  # SET( EVT_BUILD_TRIG 4) # Default mCBM 22 Lambda analysis trigger, but too long runtime for L1 and Lambda CI tests
  SET( EVT_BUILD_TRIG 15) # Default mCBM 22 Lambda trigger + TRD1D and TRD2D to reduce evt nb and combinatorics
  if(${CBM_TEST_MODEL} MATCHES MergeRequest OR ${CBM_TEST_MODEL} MATCHES Continuous )
    Message( STATUS "MR or Continuous model detected, decreasing event nb for mCBM 2022 reco tests to single one." )
    SET( RECO_EVNTS_NB 1)
    SET( RAW_DATA_UNPACK_TO 700) # MR mode on run4: 525 for 2391, 601 for 2488
    SET( RAW_DATA_BUILD_TO  600) # MR mode on run4: 60-70 for 2391, 10-120 for 2488
    SET( RAW_DATA_RECO_TO 1800)  # MR mode on run4: for 1 TS + L1, ~750 in 2391 and ~1600 in 2488 (TOFtr: ~200 and ~550)
    List(APPEND mcbm_2022_runs 2391)  # Run CI only on 2391 until way to start at TS 5 of run 2488 found
  elseif(${CBM_TEST_MODEL} MATCHES Weekly OR ${CBM_TEST_MODEL} MATCHES Profile )
    Message( STATUS "Profiling model detected, increasing timeout and event nb for mCBM 2022 tests with coverage." )
    SET( RECO_EVNTS_NB 20)
    SET( RAW_DATA_UNPACK_TO 4000) # Coverage mode: 1669 for run 2391, 2085 for run 2488
    SET( RAW_DATA_BUILD_TO   600) # Coverage mode:  171 for run 2391,  213 for run 2488
    SET( RAW_DATA_RECO_TO 7200)   # Coverage mode: ~380 for 1 TS in 2391 and ~500 in 2488 on run4
    List(APPEND mcbm_2022_runs 2391 2488)  # Run CI only on 2391 until way to start at TS 5 of run 2488 found
  else()
    SET( RECO_EVNTS_NB 3)
    SET( RAW_DATA_UNPACK_TO 700) # Nightly mode on run4: 525 for 2391, 601 for 2488
    SET( RAW_DATA_BUILD_TO  600) # Nightly mode on run4: 155 for 2391, 190 for 2488
    SET( RAW_DATA_RECO_TO 2600)  # Nightly mode on run4: 140 for 1 TS in 2391 and 2488
    List(APPEND mcbm_2022_runs 2391 2488)  # Run CI only on 2391 until way to start at TS 5 of run 2488 found
  endif()

  # ====================================================================
  ForEach(RUN IN LISTS mcbm_2022_runs)
    Message( STATUS "Checking if raw data file exists: ${RAW_DATA_PATH}/${RUN}_first20Ts.tsa" )
    If(EXISTS ${RAW_DATA_PATH}/${RUN}_first20Ts.tsa)
      Message( STATUS "Raw data file found, tests will be performed for mCBM run ${RUN}" )

      ### Transport run to create geo file
      Set(testname mcbm_geo_${RUN})
      Set(fixture_geo_${RUN} fixture_done_${testname})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/create_mcbm_geo_setup.sh ${RUN})
      Set_Tests_Properties(${testname} PROPERTIES
        TIMEOUT "200"
        PASS_REGULAR_EXPRESSION "Test Passed;All ok"
        FIXTURES_REQUIRED run_cleanup
        FIXTURES_SETUP ${fixture_geo_${RUN}}
        )

      ## Define tests + dependencies
      ### Raw data analysis tests: unpacking
      Set(testname mcbm_unpack_${RUN})
      Set(fixture_unpacked_${RUN} fixture_done_${testname})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/run/run_unpack_tsa.sh \"${RAW_DATA_PATH}/${RUN}_first20Ts.tsa\"
            ${RUN} \"\" ${RECO_EVNTS_NB}
            )
      Set_Tests_Properties(${testname} PROPERTIES
            TIMEOUT ${RAW_DATA_UNPACK_TO}
            FAIL_REGULAR_EXPRESSION "segmentation violation"
            PASS_REGULAR_EXPRESSION "Macro finished successfully"
            FIXTURES_REQUIRED run_cleanup
            FIXTURES_SETUP ${fixture_unpacked_${RUN}}
            )

      ### Raw data analysis tests: event building and reconstruction => need calibration files!!
      Set(testname mcbm_event_2022_${RUN})
      Set(fixture_event_${RUN} fixture_done_${testname})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2022/mcbm_event.sh
            \"${CBMROOT_BINARY_DIR}/macro/run/data/${RUN}_first20Ts.digi.root\" ${RUN} ${EVT_BUILD_TRIG}
            ${RECO_EVNTS_NB} false
            \"${CBMROOT_BINARY_DIR}/macro/run/data/\"
            )
      Set_Tests_Properties(${testname} PROPERTIES
            TIMEOUT ${RAW_DATA_BUILD_TO}
            FAIL_REGULAR_EXPRESSION "segmentation violation"
            PASS_REGULAR_EXPRESSION "Macro finished successfully"
            FIXTURES_REQUIRED ${fixture_unpacked_${RUN}}
            FIXTURES_SETUP ${fixture_event_${RUN}}
            )

      ### Raw analysis tests: (event building and) reconstruction => need calibration files!!
      Set(testname mcbm_reco_toftr_2022_${RUN})
      Set(fixture_reco_toftr_${RUN} fixture_done_${testname})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2022/mcbm_reco.sh ${RUN} ${RECO_EVNTS_NB}
            \"${CBMROOT_BINARY_DIR}/macro/run/data/\" \"${CBMROOT_BINARY_DIR}/macro/run/data/\"
            -1 kFALSE kTRUE kTRUE kTRUE kTRUE kFALSE kTRUE kTRUE kFALSE kTRUE kFALSE kFALSE
            \"${CBMROOT_BINARY_DIR}/macro/run/data/${RUN}_${EVT_BUILD_TRIG}.events.root\" false
            \"${CBMROOT_BINARY_DIR}/macro/run/data/${RUN}_first20Ts.digi.root\"
            )
      Set_Tests_Properties(${testname} PROPERTIES
            TIMEOUT ${RAW_DATA_RECO_TO}
            FAIL_REGULAR_EXPRESSION "segmentation violation"
            PASS_REGULAR_EXPRESSION "Macro finished successfully"
            FIXTURES_REQUIRED "${fixture_geo_${RUN}};${fixture_event_${RUN}}"
            FIXTURES_SETUP ${fixture_reco_toftr_${RUN}}
            )

      ### Raw analysis tests: (event building and) reconstruction => need calibration files!!
      ### => L1 needs around 2000 seconds to complete 1 TS (mostly in GlobalFindTracksEvents) with trigger 4
      Set(testname mcbm_reco_l1tr_2022_${RUN})
      Set(fixture_reco_l1tr_${RUN} fixture_done_${testname})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2022/mcbm_reco.sh ${RUN} ${RECO_EVNTS_NB}
            \"${CBMROOT_BINARY_DIR}/macro/run/data/\" \"${CBMROOT_BINARY_DIR}/macro/run/data/\"
            -1 kFALSE kTRUE kTRUE kTRUE kTRUE kFALSE kTRUE kFALSE kFALSE kTRUE kTRUE kFALSE
            \"${CBMROOT_BINARY_DIR}/macro/run/data/${RUN}_${EVT_BUILD_TRIG}.events.root\" false
            \"${CBMROOT_BINARY_DIR}/macro/run/data/${RUN}_first20Ts.digi.root\" # For CbmTimeslice object
            )
      Set_Tests_Properties(${testname} PROPERTIES
            TIMEOUT ${RAW_DATA_RECO_TO}
            FAIL_REGULAR_EXPRESSION "segmentation violation"
            PASS_REGULAR_EXPRESSION "Macro finished successfully"
            FIXTURES_REQUIRED "${fixture_geo_${RUN}};${fixture_reco_toftr_${RUN}}"
            FIXTURES_SETUP ${fixture_reco_l1tr_${RUN}}
            )

      ### Raw data analysis tests: event building and reconstruction => need calibration files!!
      Set(testname mcbm_digievent_2022_${RUN})
      Set(fixture_digievent_${RUN} fixture_done_${testname})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2022/mcbm_event.sh
            \"${CBMROOT_BINARY_DIR}/macro/run/data/${RUN}_first20Ts.digi.root\" ${RUN} ${EVT_BUILD_TRIG}
            ${RECO_EVNTS_NB} true
            \"${CBMROOT_BINARY_DIR}/macro/run/data/\"
            )
      Set_Tests_Properties(${testname} PROPERTIES
            TIMEOUT ${RAW_DATA_BUILD_TO}
            FAIL_REGULAR_EXPRESSION "segmentation violation"
            PASS_REGULAR_EXPRESSION "Macro finished successfully"
            FIXTURES_REQUIRED ${fixture_event_${RUN}}
            FIXTURES_SETUP ${fixture_digievent_${RUN}}
            )

      ### Raw analysis tests: reconstruction  from digievent => need calibration files!!
      ### => Last kFALSE disables the HadronAnalysis for Lambda as it takes too long: more than 30 minutes for first TS
      Set(testname mcbm_reco_toftr_digievent_2022_${RUN})
      Set(fixture_reco_toftr_digievent_${RUN} fixture_done_${testname})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2022/mcbm_reco.sh ${RUN} ${RECO_EVNTS_NB}
            \"${CBMROOT_BINARY_DIR}/macro/run/data/\" \"${CBMROOT_BINARY_DIR}/macro/run/data/\"
            -1 kFALSE kTRUE kTRUE kTRUE kTRUE kFALSE kTRUE kTRUE kFALSE kTRUE kFALSE kFALSE
            \"${CBMROOT_BINARY_DIR}/macro/run/data/${RUN}_${EVT_BUILD_TRIG}.digievents.root\" true
            )
      Set_Tests_Properties(${testname} PROPERTIES
            TIMEOUT ${RAW_DATA_RECO_TO}
            FAIL_REGULAR_EXPRESSION "segmentation violation"
            PASS_REGULAR_EXPRESSION "Macro finished successfully"
            FIXTURES_REQUIRED "${fixture_geo_${RUN}};${fixture_digievent_${RUN}}"
            FIXTURES_SETUP ${fixture_reco_toftr_digievent_${RUN}}
            )

      ### Raw analysis tests: reconstruction  from digievent => need calibration files!!
      ### => L1 needs around 2000 seconds to complete 1 TS (mostly in GlobalFindTracksEvents) with trigger 4
      Set(testname mcbm_reco_l1tr_digievent_2022_${RUN})
      Set(fixture_reco_l1tr_digievent_${RUN} fixture_done_${testname})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2022/mcbm_reco.sh ${RUN} ${RECO_EVNTS_NB}
            \"${CBMROOT_BINARY_DIR}/macro/run/data/\" \"${CBMROOT_BINARY_DIR}/macro/run/data/\"
            -1 kFALSE kTRUE kTRUE kTRUE kTRUE kFALSE kTRUE kFALSE kFALSE kTRUE kTRUE kFALSE
            \"${CBMROOT_BINARY_DIR}/macro/run/data/${RUN}_${EVT_BUILD_TRIG}.digievents.root\" true
            \"${CBMROOT_BINARY_DIR}/macro/run/data/${RUN}_first20Ts.digi.root\" # For CbmTimeslice object
            )
      Set_Tests_Properties(${testname} PROPERTIES
            TIMEOUT ${RAW_DATA_RECO_TO}
            FAIL_REGULAR_EXPRESSION "segmentation violation"
            PASS_REGULAR_EXPRESSION "Macro finished successfully"
            FIXTURES_REQUIRED "${fixture_geo_${RUN}};${fixture_reco_toftr_digievent_${RUN}}"
            FIXTURES_SETUP ${fixture_reco_l1tr_digievent_${RUN}}
            )

    EndIf() # If(EXISTS ${RAW_DATA_PATH}/${RUN}_first20Ts.tsa))
  EndForEach() # ForEach(RUN 2391 2488 )
EndIf() # If(DEFINED ENV{RAW_DATA_PATH} )





#####################
# ============================================================================

Install(FILES .rootrc run_tra_file.C run_tra_beam.C run_digi.C run_reco.C run_unpack_online.C run_unpack_tsa.C qa_config.cbm.yaml
        DESTINATION share/cbmroot/macro/run
       )
Install(PROGRAMS run_tests.sh
        DESTINATION share/cbmroot/macro/run
       )
Install(CODE "FILE(MAKE_DIRECTORY \${CMAKE_INSTALL_PREFIX}/share/cbmroot/macro/run/data)")
