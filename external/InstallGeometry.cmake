set(GEOMETRY_VERSION 83f5ffc1a886a350de6960437cdd85026ee5c666)

set(GEOMETRY_SRC_URL "https://git.cbm.gsi.de/CbmSoft/cbmroot_geometry.git")

download_project_if_needed(PROJECT         Geometry_source
                           GIT_REPOSITORY  ${GEOMETRY_SRC_URL}
                           GIT_TAG         ${GEOMETRY_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/geometry
                           TEST_FILE       media.geo
                          )


