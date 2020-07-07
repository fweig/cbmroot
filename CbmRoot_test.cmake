Set(CTEST_SOURCE_DIRECTORY $ENV{SOURCEDIR})
Set(CTEST_BINARY_DIRECTORY $ENV{BUILDDIR})
Set(CTEST_SITE $ENV{SITE})
Set(CTEST_BUILD_NAME $ENV{LABEL})
Set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
Set(CTEST_PROJECT_NAME "CBMROOT")
Set(EXTRA_FLAGS $ENV{EXTRA_FLAGS})

Set(CTEST_UPDATE_COMMAND "git")

#If($ENV{ctest_model} MATCHES Continuous)
#  Set(CTEST_SVN_UPDATE_OPTIONS "$ENV{REVISION}")
#EndIf($ENV{ctest_model} MATCHES Continuous)

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

If($ENV{ctest_model} MATCHES Weekly)
  Set(_Model PROFILE)
ElseIf($ENV{ctest_model} MATCHES MergeRequest)
  Set(_Model CONTINUOUS)
Else()
  String(TOUPPER $ENV{ctest_model} _Model)
EndIf()

If(EXTRA_FLAGS)
  Set(CTEST_CONFIGURE_COMMAND " \"${CMAKE_EXECUTABLE_NAME}\" \"-G${CTEST_CMAKE_GENERATOR}\" \"-DCMAKE_BUILD_TYPE=${_Model}\" \"-DCTEST_USE_LAUNCHERS=${CTEST_USE_LAUNCHERS}\" \"${EXTRA_FLAGS}\" \"${CTEST_SOURCE_DIRECTORY}\" ")
Else()
  Set(CTEST_CONFIGURE_COMMAND " \"${CMAKE_EXECUTABLE_NAME}\" \"-G${CTEST_CMAKE_GENERATOR}\" \"-DCMAKE_BUILD_TYPE=${_Model}\" \"-DCTEST_USE_LAUNCHERS=${CTEST_USE_LAUNCHERS}\" \"${CTEST_SOURCE_DIRECTORY}\" ")
EndIf()

If($ENV{ctest_model} MATCHES Nightly OR $ENV{ctest_model} MATCHES Weekly OR $ENV{ctest_model} MATCHES Profile)

  Find_Program(GCOV_COMMAND gcov)
  If(GCOV_COMMAND)
    Message("Found GCOV: ${GCOV_COMMAND}")
    Set(CTEST_COVERAGE_COMMAND ${GCOV_COMMAND})
  EndIf(GCOV_COMMAND)

  If(NOT $ENV{ctest_model} MATCHES Weekly)
    Set(ENV{ctest_model} Nightly)
  EndIf()

  # get the information about conflicting or localy modified files
  # from svn, extract the relavant information about the file name
  # and put the result in the output variable
  If(EXISTS ${CTEST_SOURCE_DIRECTORY}/.svn)
    Execute_Process(COMMAND svn stat -u  
                    COMMAND grep ^[CM]
                    COMMAND cut -c21- 
                    OUTPUT_VARIABLE FILELIST
                    )

    # create out of the output a cmake list. This step is done to convert the
    # stream into seperated filenames.
    # The trick is to exchange an "\n" by an ";" which is the separartor in
    # a list created by cmake 
    String(REGEX REPLACE "\n" ";" _result "${FILELIST}")

    ForEach(_file ${_result})
      String(STRIP "${_file}" _file1)
      Set(CTEST_NOTES_FILES ${CTEST_NOTES_FILES} "${CTEST_SOURCE_DIRECTORY}/${_file1}")
    EndForEach(_file ${_result})
  EndIf()

  CTEST_EMPTY_BINARY_DIRECTORY(${CTEST_BINARY_DIRECTORY})

EndIf()

Configure_File(${CTEST_SOURCE_DIRECTORY}/CTestCustom.cmake
               ${CTEST_BINARY_DIRECTORY}/CTestCustom.cmake
              )
Ctest_Read_Custom_Files("${CTEST_BINARY_DIRECTORY}")

Ctest_Start($ENV{ctest_model})

If($ENV{ctest_model} MATCHES Continuous OR $ENV{ctest_model} MATCHES MergeRequest)
  set(ENV{ctest_model} Nightly)
EndIf()

If(NOT $ENV{ctest_model} MATCHES Experimental)
  Ctest_Update(SOURCE "${CTEST_SOURCE_DIRECTORY}")
EndIf()

Ctest_Configure(BUILD "${CTEST_BINARY_DIRECTORY}"
                RETURN_VALUE _RETVAL
)
If(NOT _RETVAL)
  Ctest_Build(BUILD "${CTEST_BINARY_DIRECTORY}")
  If($ENV{ctest_model} MATCHES Continuous)
    CTest_Submit(PARTS Update Configure Build)
  EndIf()

  Ctest_Test(BUILD "${CTEST_BINARY_DIRECTORY}" 
             PARALLEL_LEVEL $ENV{number_of_processors}
             RETURN_VALUE _ctest_test_ret_val
            )

  If($ENV{ctest_model} MATCHES Continuous)
    CTest_Submit(PARTS Test)
  EndIf()

  If(GCOV_COMMAND)
    Ctest_Coverage(BUILD "${CTEST_BINARY_DIRECTORY}")
    If($ENV{ctest_model} MATCHES Continuous)
      CTest_Submit(PARTS Coverage)
    EndIf()
  EndIf()

  Ctest_Upload(FILES ${CTEST_NOTES_FILES})
  If(NOT $ENV{ctest_model} MATCHES Continuous)
    Ctest_Submit()
  EndIf()

  # Pipeline should fail also in case of failed tests
  if (_ctest_test_ret_val)
    Message(FATAL_ERROR "Some tests failed.")
  endif()

Else()
  CTest_Submit()
EndIf()
