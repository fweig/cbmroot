set(GEOMETRY_VERSION 808403df53c15d144e261a0b7ccffdc7fd719d2e)

set(GEOMETRY_SRC_URL "https://git.cbm.gsi.de/e.clerkin/cbmroot_geometry.git")

download_project_if_needed(PROJECT         Geometry_source
                           GIT_REPOSITORY  ${GEOMETRY_SRC_URL}
                           GIT_TAG         ${GEOMETRY_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/geometry
                           TEST_FILE       media.geo
                          )
