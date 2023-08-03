#----------------------------------------------------------------
# Generated CMake target import file for configuration "FullWarnings".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "AnalysisTreeQA" for configuration "FullWarnings"
set_property(TARGET AnalysisTreeQA APPEND PROPERTY IMPORTED_CONFIGURATIONS FULLWARNINGS)
set_target_properties(AnalysisTreeQA PROPERTIES
  IMPORTED_LOCATION_FULLWARNINGS "${_IMPORT_PREFIX}/lib/libAnalysisTreeQA.so"
  IMPORTED_SONAME_FULLWARNINGS "libAnalysisTreeQA.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS AnalysisTreeQA )
list(APPEND _IMPORT_CHECK_FILES_FOR_AnalysisTreeQA "${_IMPORT_PREFIX}/lib/libAnalysisTreeQA.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
