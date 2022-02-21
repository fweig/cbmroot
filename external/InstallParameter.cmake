set(PARAMETER_VERSION e4125bd3c05e40a1dc674567b43da85cc88d5639)

set(PARAMETER_SRC_URL "https://git.cbm.gsi.de/CbmSoft/cbmroot_parameter.git")

download_project_if_needed(PROJECT         Parameter_source
                           GIT_REPOSITORY  ${PARAMETER_SRC_URL}
                           GIT_TAG         ${PARAMETER_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/parameters
                           TEST_FILE       sts/LandauWidthTable.txt
                          )

