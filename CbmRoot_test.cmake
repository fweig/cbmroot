Set(CTEST_SOURCE_DIRECTORY $ENV{SOURCEDIR})
Set(CTEST_BINARY_DIRECTORY $ENV{BUILDDIR})
Set(CTEST_SITE $ENV{SITE})
Set(CTEST_BUILD_NAME $ENV{LABEL})
Set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
Set(CTEST_PROJECT_NAME "CBMROOT")
Set(EXTRA_FLAGS $ENV{EXTRA_FLAGS})

if(NOT CBM_TEST_MODEL)
  set(CBM_TEST_MODEL NIGHTLY)
endif()

include(${CTEST_SOURCE_DIRECTORY}/CTestConfig.cmake)
Ctest_Read_Custom_Files("${CTEST_SOURCE_DIRECTORY}")

Set(CTEST_UPDATE_COMMAND "git")

Set(BUILD_COMMAND "make")
Set(CTEST_BUILD_COMMAND "${BUILD_COMMAND} -i -k -j$ENV{number_of_processors}")

# Extract the FairRoot version from fairroot-config
# The version info is of the form Major.Minor.Patch e.g. 15.11.1 and
# is stored in the variable FairRoot_VERSION
Set(CMAKE_MODULE_PATH "${CTEST_SOURCE_DIRECTORY}/cmake/modules" ${CMAKE_MODULE_PATH})
Include(CbmMacros)
FairRootVersion()

If(${FairRoot_VERSION} VERSION_LESS 16.0.0)
  Set(CTEST_USE_LAUNCHERS 0)
Else()
  Set(CTEST_USE_LAUNCHERS 1)
EndIf()


If(${CBM_TEST_MODEL} MATCHES MergeRequest OR ${CBM_TEST_MODEL} MATCHES Continuous)
  Set(_BuildType NIGHTLY)
  Set(_CMakeModel Continuous)
elseIf(${CBM_TEST_MODEL} MATCHES Weekly OR ${CBM_TEST_MODEL} MATCHES Profile)
  Set(_BuildType PROFILE)
  Set(_CMakeModel Nightly)
Else()
  String(TOUPPER ${CBM_TEST_MODEL} _BuildType)
  set(_CMakeModel ${CBM_TEST_MODEL})
EndIf()

If(EXTRA_FLAGS)
  Set(CTEST_CONFIGURE_COMMAND " \"${CMAKE_EXECUTABLE_NAME}\" \"-G${CTEST_CMAKE_GENERATOR}\" \"-DCBM_TEST_MODEL=${CBM_TEST_MODEL}\" \"-DCMAKE_BUILD_TYPE=${_BuildType}\" \"-DCTEST_USE_LAUNCHERS=${CTEST_USE_LAUNCHERS}\" \"${EXTRA_FLAGS}\" \"${CTEST_SOURCE_DIRECTORY}\" ")
Else()
  Set(CTEST_CONFIGURE_COMMAND " \"${CMAKE_EXECUTABLE_NAME}\" \"-G${CTEST_CMAKE_GENERATOR}\" \"-DCBM_TEST_MODEL=${CBM_TEST_MODEL}\" \"-DCMAKE_BUILD_TYPE=${_BuildType}\" \"-DCTEST_USE_LAUNCHERS=${CTEST_USE_LAUNCHERS}\" \"${CTEST_SOURCE_DIRECTORY}\" ")
EndIf()

#If(${CBM_TEST_MODEL} MATCHES Nightly OR ${CBM_TEST_MODEL} MATCHES Weekly OR ${CBM_TEST_MODEL} MATCHES Profile)
If(NOT ${_BuildType} MATCHES Experimental)

  Find_Program(GCOV_COMMAND gcov)
  If(GCOV_COMMAND)
    Message("Found GCOV: ${GCOV_COMMAND}")
    Set(CTEST_COVERAGE_COMMAND ${GCOV_COMMAND})
  EndIf(GCOV_COMMAND)

  CTEST_EMPTY_BINARY_DIRECTORY(${CTEST_BINARY_DIRECTORY})

EndIf()

Ctest_Start(${_CMakeModel})

unset(repeat)
If(${CBM_TEST_MODEL} MATCHES MergeRequest OR ${CBM_TEST_MODEL} MATCHES Continuous)
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.17)
    set(repeat REPEAT UNTIL_PASS:2)
  endif()        
EndIf()


If(NOT ${_BuildType} MATCHES Experimental)
  Ctest_Update(SOURCE "${CTEST_SOURCE_DIRECTORY}")
EndIf()

Ctest_Configure(BUILD "${CTEST_BINARY_DIRECTORY}"
                RETURN_VALUE _RETVAL
)

If(NOT _RETVAL)
  Ctest_Build(BUILD "${CTEST_BINARY_DIRECTORY}")
  If(${_CMakeModel} MATCHES Continuous)
    If(${CMAKE_VERSION} VERSION_LESS 3.14.0)
      CTest_Submit(PARTS Update Configure Build)
    Else()
      CTest_Submit(PARTS Update Configure Build
                   BUILD_ID cdash_build_id
                  )
    EndIf()
  EndIf()

  Ctest_Test(BUILD "${CTEST_BINARY_DIRECTORY}"
             PARALLEL_LEVEL $ENV{number_of_processors}
             ${repeat}
             RETURN_VALUE _ctest_test_ret_val
            )

  If(${_CMakeModel} MATCHES Continuous)
    If(${CMAKE_VERSION} VERSION_LESS 3.14.0)
      CTest_Submit(PARTS Test)
    Else()
      CTest_Submit(PARTS Test
                   BUILD_ID cdash_build_id
                  )
    EndIf()
  EndIf()

  If(GCOV_COMMAND)
    Ctest_Coverage(BUILD "${CTEST_BINARY_DIRECTORY}")
    If(${_CMakeModel} MATCHES Continuous)
      If(${CMAKE_VERSION} VERSION_LESS 3.14.0)
        CTest_Submit(PARTS Coverage)
      Else()
        CTest_Submit(PARTS Coverage
                     BUILD_ID cdash_build_id
                    )
      EndIf()
    EndIf()
  EndIf()

  Ctest_Upload(FILES ${CTEST_NOTES_FILES})
  If(NOT ${_CMakeModel} MATCHES Continuous)
    If(${CMAKE_VERSION} VERSION_LESS 3.14.0)
      Ctest_Submit()
    Else()
      Ctest_Submit(BUILD_ID cdash_build_id)
    EndIf()
  EndIf()

  If(EXTRA_FLAGS MATCHES "INSTALL_PREFIX" AND EXTRA_FLAGS MATCHES "CBM_TEST_INSTALL" AND NOT _ctest_test_ret_val)
    Message("Testing Installation")
    execute_process(COMMAND ${BUILD_COMMAND} install -j$ENV{number_of_processors} WORKING_DIRECTORY ${CTEST_BINARY_DIRECTORY}
                    RESULTS_VARIABLE _install_ret_value
                   )
    if (NOT _install_ret_value)
      execute_process(COMMAND ${CMAKE_EXECUTABLE_NAME} -E rm -R MQ algo analysis core external fles mvd reco sim
                      WORKING_DIRECTORY ${CTEST_SOURCE_DIRECTORY}
                     )
      message("executing test suite in ${CTEST_BINARY_DIRECTORY}/install")
      execute_process(COMMAND ${CTEST_SOURCE_DIRECTORY}/cmake/scripts/execute_installation_test.sh ${CTEST_BINARY_DIRECTORY}/install
                      RESULTS_VARIABLE _install_ret_value
                     )
    endif()
 Else()
   # if installation isn't tested the return value should be 0
   set(_install_ret_value false)
 EndIf()

  # Pipeline should fail also in case of failed tests
  if (_ctest_test_ret_val OR _install_ret_value)
    If(${CMAKE_VERSION} VERSION_LESS 3.14.0)
    Else()
      message(STATUS " ")
      message(STATUS " You can find the produced results on the CDash server")
      message(STATUS " ")
      message(STATUS " CDash Build Summary ..: "
              "${CTEST_DROP_METHOD}://${CTEST_DROP_SITE}/buildSummary.php?buildid=${cdash_build_id}"
             )
      message(STATUS " CDash Test List ......: "
              "${CTEST_DROP_METHOD}://${CTEST_DROP_SITE}/viewTest.php?buildid=${cdash_build_id}"
             )
      message(STATUS " ")
    EndIf()
    Message(FATAL_ERROR "Some tests failed.")
  endif()

Else()
  If(${CMAKE_VERSION} VERSION_LESS "3.14.0")
    Ctest_Submit()
  Else()
    CTest_Submit(BUILD_ID cdash_build_id)
  EndIf()
EndIf()

If(${CMAKE_VERSION} VERSION_LESS 3.14.0)
Else()
  message(STATUS " ")
  message(STATUS " You can find the produced results on the CDash server")
  message(STATUS " ")
  message(STATUS " CDash Build Summary ..: "
          "${CTEST_DROP_METHOD}://${CTEST_DROP_SITE}/buildSummary.php?buildid=${cdash_build_id}"
         )
  message(STATUS " CDash Test List ......: "
          "${CTEST_DROP_METHOD}://${CTEST_DROP_SITE}/viewTest.php?buildid=${cdash_build_id}"
         )
  message(STATUS " ")
EndIf()
