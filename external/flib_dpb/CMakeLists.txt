download_project_if_needed(PROJECT         flib_dpb
                           GIT_REPOSITORY  "https://github.com/spadic/flib_dpb/"
                           GIT_TAG         "da380b29739b42b03dff24b2d04ed6a1ca35fe90"
                           SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/flib_dpb
                           TEST_FILE       CMakeLists.txt
                          )

Set(LIB_SOURCES flib_dpb/MicrosliceContents.cpp)
Set(LIB_HEADERS flib_dpb/MicrosliceContents.hpp)

Add_Library(flib_dpb SHARED ${LIB_SOURCES} ${LIB_HEADERS})
Set_Target_Properties(flib_dpb PROPERTIES  SUFFIX ".so")
