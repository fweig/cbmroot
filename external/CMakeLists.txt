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

  Include(InstallFlesnet.cmake)
  Set(FLES_IPC_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/flesnet/lib/fles_ipc PARENT_SCOPE)
  Set(FLES_LOGGING_INCLUDE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/flesnet/lib/logging PARENT_SCOPE)

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

  download_project_if_needed(PROJECT           xpu
    GIT_REPOSITORY    "https://git.cbm.gsi.de/fweig/xpu.git"
    GIT_TAG           "v0.6.4"
    SOURCE_DIR        ${CMAKE_CURRENT_SOURCE_DIR}/xpu
    CONFIGURE_COMMAND ""
    BUILD_COMMAND     ""
    INSTALL_COMMAND   ""
  )
  Add_Subdirectory(xpu)

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
  add_library(external::yaml-cpp STATIC IMPORTED GLOBAL)
  set_target_properties(external::yaml-cpp PROPERTIES
                        IMPORTED_LOCATION external::yaml-cpp-NOTFOUND
             )
  add_library(external::fles_ipc STATIC IMPORTED GLOBAL)
  set_target_properties(external::fles_ipc PROPERTIES
                        IMPORTED_LOCATION external::fles_ipc-NOTFOUND
                       )
  add_library(external::fles_logging STATIC IMPORTED GLOBAL)
  set_target_properties(external::fles_logging PROPERTIES
                        IMPORTED_LOCATION external::fles_logging-NOTFOUND
             )

  # Define an empty macro such that ctest is happy when no externals are
  # available
  # This is needed since to speed up the execution of the code format check 
  # no externals are downloaded
  macro(xpu_attach)
  endmacro()
endif()
