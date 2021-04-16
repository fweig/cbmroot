set(GEOMETRY_VERSION 53574909ed0120471a9be47dfc8c0d669e0f021b)

set(GEOMETRY_SRC_URL "https://git.cbm.gsi.de/CbmSoft/cbmroot_geometry.git")

download_project_if_needed(PROJECT         Geometry_source
                           GIT_REPOSITORY  ${GEOMETRY_SRC_URL}
                           GIT_TAG         ${GEOMETRY_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/geometry
                           TEST_FILE       media.geo
                          )


