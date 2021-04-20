set(GEOMETRY_VERSION fec96a5edc68763373d81c5eb2b1ff9dcf3d4208)

set(GEOMETRY_SRC_URL "https://git.cbm.gsi.de/CbmSoft/cbmroot_geometry.git")

download_project_if_needed(PROJECT         Geometry_source
                           GIT_REPOSITORY  ${GEOMETRY_SRC_URL}
                           GIT_TAG         ${GEOMETRY_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/geometry
                           TEST_FILE       media.geo
                          )


