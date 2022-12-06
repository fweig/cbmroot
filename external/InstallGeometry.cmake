set(GEOMETRY_VERSION 5d243e6451f3206e7321e5b0d604d5b30af7d002)

set(GEOMETRY_SRC_URL "https://git.cbm.gsi.de/CbmSoft/cbmroot_geometry.git")

download_project_if_needed(PROJECT         Geometry_source
                           GIT_REPOSITORY  ${GEOMETRY_SRC_URL}
                           GIT_TAG         ${GEOMETRY_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/geometry
                           TEST_FILE       media.geo
                          )
