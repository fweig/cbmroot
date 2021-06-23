set(PARAMETER_VERSION 5f15643f51a44453c595f67f9570d173bf417b4f)


set(PARAMETER_SRC_URL "https://git.cbm.gsi.de/CbmSoft/cbmroot_parameter.git")

download_project_if_needed(PROJECT         Parameter_source
                           GIT_REPOSITORY  ${PARAMETER_SRC_URL}
                           GIT_TAG         ${PARAMETER_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/parameters
                           TEST_FILE       sts/LandauWidthTable.txt
                          )

