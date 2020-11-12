# Build all libraries which come from external sources like other subversion or git repositories

# Exclude dowload of external packages. The external packages are not needed
# to run the code format checker and slows down the execution.
# Without the externals it isn't possible to build CbmRoot so don't switch
# it off execept for the checker.
Option(DOWNLOAD_EXTERNALS "Download the code from the external repositories." ON)

if(DOWNLOAD_EXTERNALS)
  download_project_if_needed(PROJECT         cppzmq
                             GIT_REPOSITORY  "https://github.com/zeromq/cppzmq/"
                             GIT_TAG         "05a0256d0eeea8063690fde6a156e14b70ed2280"
                             SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/cppzmq
                             TEST_FILE       zmq.hpp
                            )

  download_project_if_needed(PROJECT         flib_dpb_20
                             GIT_REPOSITORY  "https://git.cbm.gsi.de/f.uhlig/flib_dpb_20.git"
                             GIT_TAG         "9ce7c7c0ccc7c9ea2f8c396c35d7fe39ac9aafb6"
                             SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/flib_dpb_20
                             TEST_FILE       CMakeLists.txt
                            )

  Set(IPC_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ipc/ipc/lib/fles_ipc PARENT_SCOPE)
  Set(IPCLOG_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ipc/ipc/lib/logging PARENT_SCOPE)

  Add_Subdirectory(ipc)
  Add_Subdirectory(ipc_legacy)
  Add_Subdirectory(flib_dpb)
  Add_Subdirectory(flib_dpb_20)
  Add_Subdirectory(spadic)

  Include(InstallVC.cmake)
  Include(InstallKFParticle.cmake)
  Include(InstallNicaFemto.cmake)
  Include(InstallAnalysisTree.cmake)
#  Include(InstallAnalysisTreeQA.cmake)

  Include(InstallParameter.cmake)
  Include(InstallInput.cmake)
  Include(InstallGeometry.cmake)
else()
  # Define targets which are needed by CbmRoot but are not available
  # whithout the external packages
  add_library(ANALYSISTREE SHARED IMPORTED GLOBAL)
  add_library(NICAFEMTO SHARED IMPORTED GLOBAL)
  add_library(KFPARTICLE SHARED IMPORTED GLOBAL)
endif()
