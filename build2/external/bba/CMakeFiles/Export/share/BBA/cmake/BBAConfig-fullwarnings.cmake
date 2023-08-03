#----------------------------------------------------------------
# Generated CMake target import file for configuration "FullWarnings".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "BBA::bba_library" for configuration "FullWarnings"
set_property(TARGET BBA::bba_library APPEND PROPERTY IMPORTED_CONFIGURATIONS FULLWARNINGS)
set_target_properties(BBA::bba_library PROPERTIES
  IMPORTED_LOCATION_FULLWARNINGS "${_IMPORT_PREFIX}/lib/libbba_library.so"
  IMPORTED_SONAME_FULLWARNINGS "libbba_library.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS BBA::bba_library )
list(APPEND _IMPORT_CHECK_FILES_FOR_BBA::bba_library "${_IMPORT_PREFIX}/lib/libbba_library.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
