set(INPUT_VERSION 21433d7ba368f516d3665fbb9381d5ec2624d39d)

set(INPUT_SRC_URL "https://git.cbm.gsi.de/CbmSoft/cbmroot_input.git")

download_project_if_needed(PROJECT         Input_source
                           GIT_REPOSITORY  ${INPUT_SRC_URL}
                           GIT_TAG         ${INPUT_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/input
                           TEST_FILE       urqmd_pdg.dat
                          )
