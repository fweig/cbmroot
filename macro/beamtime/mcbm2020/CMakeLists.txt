
### Cleanup of old root files
add_test(run_cleanup_mcbm2020
	${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/scripts/cleanmacrodir.cmake)
set_tests_properties(run_cleanup_mcbm2020 PROPERTIES
	TIMEOUT 30
	FIXTURES_SETUP cleanup_mcbm2020
)
#####################

##### mCBM 2020 tests: only if test TSA files for "typical runs" present
If(DEFINED ENV{RAW_DATA_PATH} )
  SET( RAW_DATA_PATH $ENV{RAW_DATA_PATH} )

  Message( STATUS "Raw data path provided => Performing mCBM 2020 tests using tsa files in ${RAW_DATA_PATH}" )

  ## Generate scripts
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/unpack_tsa_mcbm.C )  # Unpackers
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/check_timing.C )     # CbmCheckTiming.h
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/check_pulser.C )     # CbmMcbm2019CheckPulser.h
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/build_events.C )     # CbmMcbm2018EventBuilder.h
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/check_events.C )     # CbmCheckEvents.h
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/build_event_win.C )  # CbmMcbm2019TimeWinEventBuilderTask.h + CbmMcbm2019TimeWinEventBuilderAlgo.h
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/mcbm_reco.C )        # Cluster Finding + Hit reconstruction, time based (no events)
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/mcbm_build_and_reco.C ) # TW event building + Cluster Finding + Hit reconstruction, Event based
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/mcbm_event_reco.C )  # Digi dt event building + Cluster Finding + Hit reconstruction, Event based
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/mcbm_event_ana.C )   # TOF Track extension toward other detectors
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/mcbm_tof_tracking.C ) # TOF Tracking, Event based
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/MonitorT0.C )        # CbmMcbm2018MonitorTaskT0 + CbmMcbm2018MonitorAlgoT0
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/MonitorSts.C )       # CbmMcbm2018MonitorSts.h
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/MonitorMuch.C )      # CbmMcbm2018MonitorMuchLite.h
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/MonitorTrd.C )       # CbmMcbm2018UnpackerTaskTrd.h in monitor mode
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/MonitorTof.C )       # CbmMcbm2018MonitorTaskTof + CbmMcbm2018MonitorAlgoTof
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/MonitorRich.C )      # CbmMcbm2018UnpackerTaskRich.h in monitor mode
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/MonitorPsd.C )       # CbmMcbm2018MonitorTaskPsd + CbmMcbm2018MonitorAlgoPsd
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/unpack_pulser_mcbm.C ) # Unpackers + Pulser check

  ## Run only if raw data files present
  ForEach(RUN 759 760 761 762 811 812 819 824 831 856 )
    Message( STATUS "Checking if raw data file exists: ${RAW_DATA_PATH}/${RUN}_first20Ts.tsa" )
    If(EXISTS ${RAW_DATA_PATH}/${RUN}_first20Ts.tsa)
      Message( STATUS "Raw data file found, tests will be performed for mCBM run ${RUN}" )

      ## Define tests + dependencies
      ### Raw data analysis tests
      Set(testname mcbm_unpack_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/unpack_tsa_mcbm.sh \"${RAW_DATA_PATH}/${RUN}_first20Ts.tsa\" ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES DEPENDS run_cleanup_mcbm2020)
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED run_cleanup_mcbm2020)
      Set(fixture_unpacked_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_unpacked_${RUN}})

      Set(testname mcbm_check_timing_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/check_timing.sh \"data/unp_mcbm_${RUN}.root\" ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
  #    Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Total number of T0 digis: 18")
  #    Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Total number of Sts digis: 1061912")
  #    Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Total number of Much digis: 319732")
  #    Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Total number of Tof digis: 246711")
  #    Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Total number of Rich digis: 5359")
  #    Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Total number of Psd digis: 0")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked_${RUN}})
      #### Make this test run alone to try to avoid memory errors on test machines
      set_tests_properties(${testname} PROPERTIES RUN_SERIAL true )

#      Set(testname mcbm_check_pulser_2020_${RUN})
#      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/check_pulser.sh ${RUN} )
#      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
#      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
#      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked_${RUN}})
#      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_check_timing_2020_${RUN})

      Set(testname mcbm_build_event_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/build_events.sh \"data/unp_mcbm_${RUN}.root\" ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked_${RUN}})
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_check_timing_2020_${RUN})
      Set(fixture_built_evt_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_built_evt_${RUN}})

      Set(testname mcbm_check_event_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/check_events.sh 99999 ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_built_evt_${RUN}})
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_build_event_2020_${RUN})

      Set(testname mcbm_build_event_win_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/build_event_win.sh ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked_${RUN}})
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_check_event_2020_${RUN})
      Set(fixture_built_evt_win_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_built_evt_win_${RUN}})

      Set(testname mcbm_reco_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/mcbm_reco.sh ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked_${RUN}})
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_build_event_win_2020_${RUN})
      Set(fixture_mcbm_reco_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_mcbm_reco_${RUN}})

      Set(testname mcbm_build_reco_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/mcbm_build_and_reco.sh ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked_${RUN}})
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_reco_2020_${RUN})
      Set(fixture_mcbm_build_reco_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_mcbm_build_reco_${RUN}})

      Set(testname mcbm_event_reco_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/mcbm_event_reco.sh ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked_${RUN}})
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_build_reco_2020_${RUN})
      Set(fixture_mcbm_event_reco_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_mcbm_event_reco_${RUN}})

      Set(testname mcbm_track_evt_win_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/mcbm_tof_tracking.sh ${RUN} "kTRUE" )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_mcbm_build_reco_${RUN}})
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_build_reco_2020_${RUN})
      Set(fixture_mcbm_track_evt_win_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_mcbm_track_evt_win_${RUN}})

      Set(testname mcbm_track_event_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/mcbm_tof_tracking.sh ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_mcbm_event_reco_${RUN}})
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_event_reco_2020_${RUN})
      Set(fixture_mcbm_track_event_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_mcbm_track_event_${RUN}})

      Set(testname mcbm_evt_win_ana_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/mcbm_event_ana.sh ${RUN} 1000 "kTRUE" )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_mcbm_track_evt_win_${RUN}})
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_track_evt_win_2020_${RUN})
      Set(fixture_mcbm_evt_win_ana_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_mcbm_evt_win_ana_${RUN}})

      Set(testname mcbm_event_ana_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/mcbm_event_ana.sh ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_mcbm_track_event_${RUN}})
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_track_event_2020_${RUN})
      Set(fixture_mcbm_event_ana_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_mcbm_event_ana_${RUN}})

      ### Monitors tests => Should run 1 by 1 after unpacker test to avoid concurrent access to tsa file
      Set(testname mcbm_monit0_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/MonitorT0.sh \"${RAW_DATA_PATH}/${RUN}_first20Ts.tsa\" )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_unpack_2020_${RUN})

      Set(testname mcbm_monists_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/MonitorSts.sh \"${RAW_DATA_PATH}/${RUN}_first20Ts.tsa\" \"\" ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_monit0_2020_${RUN})

      Set(testname mcbm_monimuch_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/MonitorMuch.sh \"${RAW_DATA_PATH}/${RUN}_first20Ts.tsa\" \"\" ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_monists_2020_${RUN})

      Set(testname mcbm_monitrd_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/MonitorTrd.sh \"${RAW_DATA_PATH}/${RUN}_first20Ts.tsa\" )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_monimuch_2020_${RUN})

      Set(testname mcbm_monitof_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/MonitorTof.sh \"${RAW_DATA_PATH}/${RUN}_first20Ts.tsa\" )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_monitrd_2020_${RUN})

      Set(testname mcbm_monirich_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/MonitorRich.sh \"${RAW_DATA_PATH}/${RUN}_first20Ts.tsa\" )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_monitof_2020_${RUN})

      Set(testname mcbm_monipsd_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/MonitorPsd.sh \"${RAW_DATA_PATH}/${RUN}_first20Ts.tsa\" )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_monirich_2020_${RUN})

      ### Unpack + pulser check, needed as STS unpacker now needs a special flag to store the pulser digis in the standard out
      Set(testname mcbm_unpack_pulser_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/unpack_pulser_mcbm.sh \"${RAW_DATA_PATH}/${RUN}_first20Ts.tsa\" ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_monirich_2020_)
    EndIf() # If(EXISTS ${RAW_DATA_PATH}/${RUN}_first20Ts.tsa))
  EndForEach() # ForEach(RUN 759 760 761 762 811 812 819 824 856 )
EndIf() # If(DEFINED ENV{RAW_DATA_PATH} )
#####################
