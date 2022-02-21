set(INPUT_VERSION 35c1dc7dd893b2da93c9404c60b82e84e8969d95)

set(INPUT_SRC_URL "https://git.cbm.gsi.de/CbmSoft/cbmroot_input.git")

download_project_if_needed(PROJECT         Input_source
                           GIT_REPOSITORY  ${INPUT_SRC_URL}
                           GIT_TAG         ${INPUT_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/input
                           TEST_FILE       urqmd_pdg.dat
                          )
