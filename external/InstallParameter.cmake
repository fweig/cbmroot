set(PARAMETER_VERSION 9f7459f375104d6b0a70a804f8a1044b474242ab)


set(PARAMETER_SRC_URL "https://git.cbm.gsi.de/CbmSoft/cbmroot_parameter.git")

download_project_if_needed(PROJECT         Parameter_source
                           GIT_REPOSITORY  ${PARAMETER_SRC_URL}
                           GIT_TAG         ${PARAMETER_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/parameters
                           TEST_FILE       sts/LandauWidthTable.txt
                          )

