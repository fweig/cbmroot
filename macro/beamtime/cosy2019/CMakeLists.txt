
### Cleanup of old root files
add_test(run_cleanup_cosy2019
	${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/scripts/cleanmacrodir.cmake)
set_tests_properties(run_cleanup_cosy2019 PROPERTIES
	TIMEOUT 30
	FIXTURES_SETUP cleanup_cosy2019
)
#####################

### Simulation: Run in any case
GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/cosy2019/cosy2019_transport.C )   # MC (simu)
#GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/cosy2019/cosy2019_digi.C )        # Digitization (simu)
#  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/cosy2019/cosy2019_reco.C )        # Cluster Finding + Hit reconstruction, time based (simu)

Set(testname cosy_transport_2019)
Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/cosy2019/cosy2019_transport.sh 10 )
Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "300")
Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
set_tests_properties(${testname} PROPERTIES DEPENDS run_cleanup_cosy2019)
set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED run_cleanup_cosy2019)
Set(fixture_cosy_transport fixture_done_${testname})
set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_cosy_transport})

## Digitization = broken as of 09/12/2020
#Set(testname cosy_digi_2019)
#Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/cosy2019/cosy2019_digi.sh 20 )
#Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
#Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
#set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_cosy_transport})
#set_tests_properties(${testname} PROPERTIES DEPENDS cosy_transport_2019)
#Set(fixture_cosy_digi fixture_done_${testname})
#set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_cosy_digi})
#####################

### COSY 2019 tests: only if test TSA files for "typical runs" present
If(DEFINED ENV{RAW_DATA_PATH} )
  SET( RAW_DATA_PATH $ENV{RAW_DATA_PATH} )

  Message( STATUS "Raw data path provided => Performing COSY 2019 tests using tsa files in ${RAW_DATA_PATH}" )

  ## Generate scripts
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/cosy2019/unpack_tsa_cosy2019.C )  # Unpackers (real)
  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/cosy2019/cosy2019_calib.C )       # Coarse time calibration (real)
#  GENERATE_ROOT_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/beamtime/cosy2019/cosy2019_process.C )     # Cluster Finding + Hit reconstruction, time based (real)

  ## Run only if raw data files present
  ForEach(RUN 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 )
    Message( STATUS "Checking if raw data file exists: ${RAW_DATA_PATH}/r00${RUN}_first20Ts.tsa" )
    If(EXISTS ${RAW_DATA_PATH}/r00${RUN}_first20Ts.tsa)
      Message( STATUS "Raw data file found, tests will be performed for COSY 2019 run ${RUN}" )

      If( 08 EQUAL ${RUN} )
        SET( RUN_INDEX 8 )
      ElseIf( 09 EQUAL ${RUN} )
        SET( RUN_INDEX 9 )
      Else()
        SET( RUN_INDEX ${RUN} )
      EndIf()

      ## Define tests + dependencies
      ### Raw data analysis tests
      Set(testname cosy_unpack_2019_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/cosy2019/unpack_tsa_cosy2019.sh \"${RAW_DATA_PATH}/r00${RUN}_first20Ts.tsa\" ${RUN_INDEX} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES DEPENDS run_cleanup_cosy2019)
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED run_cleanup_cosy2019)
      Set(fixture_cosy_unpacked_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_cosy_unpacked_${RUN}})

      Set(testname cosy_calib_2019_${RUN})
      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/cosy2019/cosy2019_calib.sh 20 ${RUN_INDEX} )
      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_cosy_unpacked_${RUN}})
      set_tests_properties(${testname} PROPERTIES DEPENDS cosy_unpack_2019_${RUN})
      Set(fixture_cosy_calib_${RUN} fixture_done_${testname})
      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_cosy_calib_${RUN}})

## Clustering = broken as of 09/12/2020
#      Set(testname cosy_process_2019_${RUN})
#      Add_Test(${testname} ${CBMROOT_BINARY_DIR}/macro/beamtime/cosy2019/cosy2019_process.sh ${RUN_INDEX} )
#      Set_Tests_Properties(${testname} PROPERTIES TIMEOUT "600")
#      Set_Tests_Properties(${testname} PROPERTIES PASS_REGULAR_EXPRESSION "Test Passed;All ok")
#      set_tests_properties(${testname} PROPERTIES FIXTURES_REQUIRED ${fixture_cosy_calib_${RUN}})
#      set_tests_properties(${testname} PROPERTIES DEPENDS cosy_calib_2019_${RUN})
#      Set(fixture_cosy_process_${RUN} fixture_done_${testname})
#      set_tests_properties(${testname} PROPERTIES FIXTURES_SETUP ${fixture_cosy_process_${RUN}})
    EndIf() # If(EXISTS ${RAW_DATA_PATH}/${RUN}_first20Ts.tsa))
  EndForEach() # ForEach(RUN 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 )
EndIf() # If(DEFINED ENV{RAW_DATA_PATH} )
#####################
