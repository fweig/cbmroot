set(GEOMETRY_VERSION d3b5bf6617fcb4c402afe5473b3bb251d1369c98)

set(GEOMETRY_SRC_URL "https://git.cbm.gsi.de/e.clerkin/cbmroot_geometry.git")

download_project_if_needed(PROJECT         Geometry_source
                           GIT_REPOSITORY  ${GEOMETRY_SRC_URL}
                           GIT_TAG         ${GEOMETRY_VERSION}
                           SOURCE_DIR      ${CMAKE_SOURCE_DIR}/geometry
                           TEST_FILE       media.geo
                          )
