
### Cleanup of old root files
add_test(run_cleanup_mcbm2019
	${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/scripts/cleanmacrodir.cmake)
set_tests_properties(run_cleanup_mcbm2019 PROPERTIES
	TIMEOUT 30
	FIXTURES_SETUP cleanup_mcbm2019
)
#####################

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
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/mcbm2019/unpack_pulser_mcbm.C ) # Unpackers + Pulser check

## Define tests + dependencies
Set(testname mcbm_convert_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/convert_tsa_mcbm.sh \"${CBMROOT_SOURCE_DIR}/input/mcbm_run399_first20Ts.tsa\" )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "60")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
Set_tests_properties(${testname} PROPERTIES DEPENDS run_cleanup_mcbm2019)
Set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED run_cleanup_mcbm2019)

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

#Set(testname mcbm_check_pulser_2019)
#Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/check_pulser.sh 399 )
#Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
#Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
#set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked})
#set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_check_muchdigis_2019)

Set(testname mcbm_build_event_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/build_events.sh \"data/unp_mcbm_399.root\" 399 )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_unpacked})
set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_check_muchdigis_2019)
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

### Unpack + pulser check, needed as STS unpacker now needs a special flag to store the pulser digis in the standard out
Set(testname mcbm_unpack_pulser_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/mcbm2019/unpack_pulser_mcbm.sh \"${CBMROOT_SOURCE_DIR}/input/mcbm_run399_first20Ts.tsa\" 399 )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES DEPENDS mcbm_monipsd_2019)
#####################
