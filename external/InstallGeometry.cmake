set(GEOMETRY_VERSION 151fcb9356610da62d91eeb7d5f4ef3d418b0cae)

set(GEOMETRY_SRC_URL "https://git.cbm.gsi.de/e.clerkin/cbmroot_geometry.git")

download_project_if_needed(PROJECT         Geometry_source
                           GIT_REPOSITORY  ${GEOMETRY_SRC_URL}
                           GIT_TAG         ${GEOMETRY_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/geometry
                           TEST_FILE       media.geo
                          )
