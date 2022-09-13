set(INPUT_VERSION e7571e3af56ba81fa586efc7fa676b2b8e9beea4)

set(INPUT_SRC_URL "https://git.cbm.gsi.de/CbmSoft/cbmroot_input.git")

download_project_if_needed(PROJECT         Input_source
                           GIT_REPOSITORY  ${INPUT_SRC_URL}
                           GIT_TAG         ${INPUT_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/input
                           TEST_FILE       urqmd_pdg.dat
                          )
