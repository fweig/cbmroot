set(PARAMETER_VERSION 4aea00fc0ff47ae7a79e9b60bcd6d3ddd1f9a766)

set(PARAMETER_SRC_URL "https://git.cbm.gsi.de/e.clerkin/cbmroot_parameter.git")

download_project_if_needed(PROJECT         Parameter_source
                           GIT_REPOSITORY  ${PARAMETER_SRC_URL}
                           GIT_TAG         ${PARAMETER_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/parameters
                           TEST_FILE       sts/LandauWidthTable.txt
                          )
