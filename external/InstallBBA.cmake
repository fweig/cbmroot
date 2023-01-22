  download_project_if_needed(PROJECT           bba
                             GIT_REPOSITORY    "https://git.cbm.gsi.de/se.gorbunov/bba.git"
                             GIT_TAG           "183d0e39ceeeb0ab737960a9c984192f1ba93674"
                             SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/bba
                             CONFIGURE_COMMAND ""
                             BUILD_COMMAND     ""
                             INSTALL_COMMAND   ""
  )
  Add_Subdirectory(bba)
  Set(BBA_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bba/include PARENT_SCOPE)
