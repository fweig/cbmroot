cmake_host_system_information(RESULT fqdn QUERY FQDN)

set(CTEST_SOURCE_DIRECTORY .)
set(CTEST_BINARY_DIRECTORY build)
Set(CTEST_PROJECT_NAME "CbmRoot")
set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
set(CTEST_USE_LAUNCHERS ON)

if ("$ENV{CTEST_SITE}" STREQUAL "")
  set(CTEST_SITE "${fqdn}")
else()
  set(CTEST_SITE $ENV{CTEST_SITE})
endif()

if ("$ENV{LABEL}" STREQUAL "")
  set(CTEST_BUILD_NAME "format-check")
else()
  set(CTEST_BUILD_NAME $ENV{LABEL})
endif()

ctest_start(Experimental)

ctest_configure(BUILD "${CTEST_BINARY_DIRECTORY}")

ctest_build(TARGET FormatCheck FLAGS "")

ctest_submit()
