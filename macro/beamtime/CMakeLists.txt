# Put the .rootrc file into the directory from which root is executed.
# Otherwise the rootalias file is not loaded
File(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc DESTINATION ${CBMROOT_BINARY_DIR}/macro/beamtime/tests)

# Define the number of events to run
Set(NumEvents 2)

If(EXISTS /lustre/cbm/prod/beamtime/2014/12/cosy/run106_production_sts3exttrig_0000.lmd)
  # STS COSY December 2014 beamtime => LMD
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/tests/StsCosyDec14.C)
  add_test(StsCosyDec14 ${CBMROOT_BINARY_DIR}/macro/beamtime/tests/StsCosyDec14.sh ${NumEvents} )
  SET_TESTS_PROPERTIES(StsCosyDec14 PROPERTIES TIMEOUT "600")
  SET_TESTS_PROPERTIES(StsCosyDec14 PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
EndIf()

If(EXISTS ${CBMROOT_SOURCE_DIR}/input/hodoTop_source_1000ts_20160422.tsa)
  # STS Lab Setup => TSA
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/tests/FHodoLabSetup.C)
  add_test(StsLabSetup ${CBMROOT_BINARY_DIR}/macro/beamtime/tests/FHodoLabSetup.sh)
  SET_TESTS_PROPERTIES(StsLabSetup PROPERTIES TIMEOUT "60")
  SET_TESTS_PROPERTIES(StsLabSetup PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
EndIf()

##### mCBM 2019 tests

## Generate scripts
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/convert_tsa_mcbm.C ) # CbmMcbm2018RawConverterXXX
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/unpack_tsa_mcbm.C )  # Unpackers
# CbmCheckDigisNbCorr.h => missing script?
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/check_timing.C )     # CbmCheckTiming.h
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/check_much_digis.C ) # CbmMcbm2019CheckDigisMuch.h
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/check_sts_digis.C )  # CbmMcbm2019CheckDigisSts.h
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/check_pulser.C )     # CbmMcbm2019CheckPulser.h
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/build_events.C )     # CbmMcbm2018EventBuilder.h
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/check_events.C )     # CbmCheckEvents.h
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/MonitorT0.C )        # CbmMcbm2018MonitorTaskT0 + CbmMcbm2018MonitorAlgoT
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/MonitorSts.C )       # CbmMcbm2018MonitorSts.h
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/MonitorMuch.C )      # CbmMcbm2018MonitorMuchLite.h
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/MonitorTof.C )       # CbmMcbm2018MonitorTaskTof + CbmMcbm2018MonitorAlgoTof
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/MonitorRich.C )      # CbmMcbm2018UnpackerTaskRich.h in monitor mode
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/MonitorPsd.C )       # CbmMcbm2018MonitorTaskPsd + CbmMcbm2018MonitorAlgoPsd

## Define tests + dependencies
Set(testname mcbm_convert_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/convert_tsa_mcbm.sh \"${CBMROOT_SOURCE_DIR}/input/mcbm_run399_first20Ts.tsa\" )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "60")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")

Set(testname mcbm_unpack_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/unpack_tsa_mcbm.sh \"${CBMROOT_SOURCE_DIR}/input/mcbm_run399_first20Ts.tsa\" 399 )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "60")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
Set(fixture_unpacked fixture_done_${testname})
set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_unpacked})
set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_convert_2019)

Set(testname mcbm_check_timing_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/check_timing.sh \"data/unp_mcbm_399.root\" 399 )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Total number of T0 digis: 18")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Total number of Sts digis: 1061912")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Total number of Much digis: 319732")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Total number of Tof digis: 246711")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Total number of Rich digis: 5359")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Total number of Psd digis: 0")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked})

Set(testname mcbm_check_stsdigis_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/check_sts_digis.sh 399 1 200.0 )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked})
set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_check_timing_2019)

Set(testname mcbm_check_muchdigis_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/check_much_digis.sh 399 1 200.0 )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked})
set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_check_stsdigis_2019)

Set(testname mcbm_check_pulser_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/check_pulser.sh 399 )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked})
set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_check_muchdigis_2019)

Set(testname mcbm_build_event_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/build_events.sh \"data/unp_mcbm_399.root\" 399 )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked})
set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_check_pulser_2019)
Set(fixture_built_evt fixture_done_${testname})
set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_built_evt})

Set(testname mcbm_check_event_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/check_events.sh 99999 399 )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_built_evt})
set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_build_event_2019)

Set(testname mcbm_monit0_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/MonitorT0.sh \"${CBMROOT_SOURCE_DIR}/input/mcbm_run399_first20Ts.tsa\" )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_check_event_2019)

Set(testname mcbm_monists_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/MonitorSts.sh \"${CBMROOT_SOURCE_DIR}/input/mcbm_run399_first20Ts.tsa\" )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_monit0_2019)

Set(testname mcbm_monimuch_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/MonitorMuch.sh \"${CBMROOT_SOURCE_DIR}/input/mcbm_run399_first20Ts.tsa\" )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_monists_2019)

Set(testname mcbm_monitof_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/MonitorTof.sh \"${CBMROOT_SOURCE_DIR}/input/mcbm_run399_first20Ts.tsa\" )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_monimuch_2019)

Set(testname mcbm_monirich_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/MonitorRich.sh \"${CBMROOT_SOURCE_DIR}/input/mcbm_run399_first20Ts.tsa\" )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_monitof_2019)

Set(testname mcbm_monipsd_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/MonitorPsd.sh \"${CBMROOT_SOURCE_DIR}/input/mcbm_run399_first20Ts.tsa\" )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_monirich_2019)

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
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/MonitorT0.C )        # CbmMcbm2018MonitorTaskT0 + CbmMcbm2018MonitorAlgoT0
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/MonitorSts.C )       # CbmMcbm2018MonitorSts.h
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/MonitorMuch.C )      # CbmMcbm2018MonitorMuchLite.h
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/MonitorTrd.C )       # CbmMcbm2018UnpackerTaskTrd.h in monitor mode
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/MonitorTof.C )       # CbmMcbm2018MonitorTaskTof + CbmMcbm2018MonitorAlgoTof
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/MonitorRich.C )      # CbmMcbm2018UnpackerTaskRich.h in monitor mode
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2020/MonitorPsd.C )       # CbmMcbm2018MonitorTaskPsd + CbmMcbm2018MonitorAlgoPsd

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

      Set(testname mcbm_check_pulser_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/check_pulser.sh ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked_${RUN}})
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_check_timing_2020_${RUN})

      Set(testname mcbm_build_event_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/build_events.sh \"data/unp_mcbm_${RUN}.root\" ${RUN} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked_${RUN}})
      set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_check_pulser_2020_${RUN})
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
      set_tests_properties(${testname} PROPERTIES DEPENDS fixture_built_evt_win_${RUN})
      Set(fixture_mcbm_reco_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_mcbm_reco_${RUN}})

      ### Monitors tests
      Set(testname mcbm_monit0_2020_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2020/MonitorT0.sh \"${RAW_DATA_PATH}/${RUN}_first20Ts.tsa\" )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES DEPENDS fixture_mcbm_reco_${RUN})

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
    EndIf() # If(EXISTS ${RAW_DATA_PATH}/${RUN}_first20Ts.tsa))
  EndForEach() # ForEach(RUN 759 760 761 762 811 812 819 824 856 )
EndIf() # If(DEFINED ENV{RAW_DATA_PATH} )
#####################

### Copy beamtime folder to allow execution of tasks on batch farm after installation
Install(DIRECTORY cern2016 DESTINATION share/cbmroot/macro/beamtime)
