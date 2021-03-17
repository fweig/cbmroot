set(PARAMETER_VERSION cd3fe7c92514188ed56c78dab5ce1baff77b41a7)

set(PARAMETER_SRC_URL "https://git.cbm.gsi.de/e.clerkin/cbmroot_parameter.git")

download_project_if_needed(PROJECT         Parameter_source
                           GIT_REPOSITORY  ${PARAMETER_SRC_URL}
                           GIT_TAG         ${PARAMETER_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/parameters
                           TEST_FILE       sts/LandauWidthTable.txt
                          )
