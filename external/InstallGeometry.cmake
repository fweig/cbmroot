set(GEOMETRY_VERSION 34f302c071bda80da0bb8d562937b66a31bc27cd)

set(GEOMETRY_SRC_URL "https://git.cbm.gsi.de/CbmSoft/cbmroot_geometry.git")

download_project_if_needed(PROJECT         Geometry_source
                           GIT_REPOSITORY  ${GEOMETRY_SRC_URL}
                           GIT_TAG         ${GEOMETRY_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/geometry
                           TEST_FILE       media.geo
                          )
