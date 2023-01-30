# Check if Python is installed and excute test only if python is available

find_package(Python)

if (Python_FOUND)
  # Define the number of events to run
  if(LARGE_TEST_STATISTIC OR ${CBM_TEST_MODEL} MATCHES Weekly)
    Set(production_macro_events 40)
  else()
    Set(production_macro_events 2)
  endif()
  math(EXPR timeOutTime "300 + (${production_macro_events} * 150)")

  GENERATE_TEST_SCRIPT(${CBMROOT_SOURCE_DIR}/macro/PWG/common/production/run_json.sh)

  # copy the configuration to the build directory and replace variables
  configure_file(${CBMROOT_SOURCE_DIR}/macro/PWG/common/production/config_ci.json.in
                 ${CBMROOT_BINARY_DIR}/macro/PWG/common/production/config_ci.json
                 @ONLY
                )

  File(COPY ${CBMROOT_SOURCE_DIR}/macro/include/.rootrc DESTINATION ${CBMROOT_BINARY_DIR}/macro/PWG/common/production)

  Set(testname PWG_common_production_run_json)

  Add_Test(${testname} ${CMAKE_CURRENT_BINARY_DIR}/run_json.sh ${CMAKE_CURRENT_BINARY_DIR}/config_ci.json)
  Set_Tests_Properties(${testname} PROPERTIES TIMEOUT ${timeOutTime})
endif()
