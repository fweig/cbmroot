# Build all libraries which come from external sources like other subversion or git repositories

# Exclude dowload of external packages. The external packages are not needed
# to run the code format checker and slows down the execution.
# Without the externals it isn't possible to build CbmRoot so don't switch
# it off execept for the checker.
Option(DOWNLOAD_EXTERNALS "Download the code from the external repositories." ON)

if(DOWNLOAD_EXTERNALS)
  download_project_if_needed(PROJECT         cppzmq
                             GIT_REPOSITORY  "https://github.com/zeromq/cppzmq/"
                             GIT_TAG         "4f111562e7ce23d53bda53748d934ca523d650d7"
                             SOURCE_DIR      ${CMAKE_CURRENT_SOURCE_DIR}/cppzmq
                             TEST_FILE       zmq.hpp
                            )
  # Install header files
  install(FILES cppzmq/zmq.hpp DESTINATION include)


  Set(IPC_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ipc/ipc/lib/fles_ipc PARENT_SCOPE)
  Set(IPCLOG_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ipc/ipc/lib/logging PARENT_SCOPE)

  Add_Subdirectory(ipc)
  Add_Subdirectory(flib_dpb)

  find_package(Vc 1.4.1)
  if(NOT Vc_FOUND)
    Message("Vc not found in FairSoft installation, Build included version.")
    Include(InstallVC.cmake)
    set(KF_DEPENDS_ON "Vc")
  else()
    # Missing from VcConfig.cmake
    set(VC_INCLUDE_DIRS ${Vc_INCLUDE_DIR} PARENT_SCOPE)
    set(Vc_LIB_DIR ${Vc_ROOTDIR}/${_LIBDIR_DEFAULT} PARENT_SCOPE)
    set(KF_DEPENDS_ON "")
  endif()

  Include(InstallKFParticle.cmake)
  Include(InstallNicaFemto.cmake)
  Include(InstallAnalysisTree.cmake)
  Include(InstallAnalysisTreeQA.cmake)

  Include(InstallParameter.cmake)
  Include(InstallInput.cmake)
  Include(InstallGeometry.cmake)

  Include(InstallYamlCpp.cmake)
else()
  # Define targets which are needed by CbmRoot but are not available
  # whithout the external packages
  add_library(ANALYSISTREE SHARED IMPORTED GLOBAL)
  add_library(ANALYSISTREEQA SHARED IMPORTED GLOBAL)
  add_library(NICAFEMTO SHARED IMPORTED GLOBAL)
  add_library(KFPARTICLE SHARED IMPORTED GLOBAL)
endif()
