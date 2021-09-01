set(PARAMETER_VERSION 1d22ed182f9fa48fdf9178bea5373d257041e580)


set(PARAMETER_SRC_URL "https://git.cbm.gsi.de/CbmSoft/cbmroot_parameter.git")

download_project_if_needed(PROJECT         Parameter_source
                           GIT_REPOSITORY  ${PARAMETER_SRC_URL}
                           GIT_TAG         ${PARAMETER_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/parameters
                           TEST_FILE       sts/LandauWidthTable.txt
                          )

