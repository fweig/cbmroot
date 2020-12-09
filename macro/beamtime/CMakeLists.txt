
##### Old beamtime tests
##### TODO: check if still running and cleanup if needed
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
#####################

##### COSY 2019 tests
add_subdirectory(cosy2019)
#####################


##### mCBM 2019 tests
add_subdirectory(mcbm2019)
#####################

##### mCBM 2020 tests: only if test TSA files for "typical runs" present
add_subdirectory(mcbm2020)
#####################

### Copy beamtime folder to allow execution of tasks on batch farm after installation
Install(DIRECTORY cern2016 DESTINATION share/cbmroot/macro/beamtime)
