## Create a library called "libCbmGeoSetup" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

If(NOT EXISTS geodb.tar.gz)
  Message(STATUS "Download geometry database")
  FILE(DOWNLOAD http://cbmdb.jinr.ru/geometry/download_file.php?type=all&tag=all ${CMAKE_BINARY_DIR}/geodb.tar.gz SHOW_PROGRESS)

  File(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/geodb)
  File(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/geometry/db)

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xvzf ${CMAKE_BINARY_DIR}/geodb.tar.gz
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/geodb
    OUTPUT_VARIABLE TAR_OUTPUT
    ERROR_VARIABLE TAR_OUTPUT
    RESULT_VARIABLE TAR_RETURN_VALUE
  )

  if (NOT TAR_RETURN_VALUE EQUAL 0)
    message(FATAL_ERROR "Error: ${TAR_OUTPUT}")
  endif()

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/geodb/storage ${CMAKE_SOURCE_DIR}/geometry/db
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/geodb/local.db ${CMAKE_SOURCE_DIR}/geometry/db/
    OUTPUT_VARIABLE COPY_OUTPUT
    ERROR_VARIABLE COPY_OUTPUT
    RESULT_VARIABLE COPY_RETURN_VALUE
  )

  if (NOT COPY_RETURN_VALUE EQUAL 0)
    message("Error: ${COPY_OUTPUT}")
  endif()

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/geodb/storage 
  )

  Message(STATUS "Download and unpacking done")
EndIf()


Set(INCLUDE_DIRECTORIES
${CBMROOT_SOURCE_DIR}/sim/transport
${CBMROOT_SOURCE_DIR}/sim/transport/base
${CBMROOT_SOURCE_DIR}/sim/transport/geosetup
${CBMROOT_SOURCE_DIR}/sim/detectors/much
${CBMROOT_SOURCE_DIR}/sim/detectors/psd
${CBMROOT_SOURCE_DIR}/sim/detectors/rich
${CBMROOT_SOURCE_DIR}/sim/detectors/sts
${CBMROOT_SOURCE_DIR}/sim/detectors/tof
${CBMROOT_SOURCE_DIR}/sim/detectors/trd

${CBMBASE_DIR}
${CBMDATA_DIR}
${CBMROOT_SOURCE_DIR}/run
${CBMROOT_SOURCE_DIR}/mvd
#${CBMROOT_SOURCE_DIR}/sts
#${CBMROOT_SOURCE_DIR}/sts/mc
${CBMROOT_SOURCE_DIR}/rich/sim
${CBMROOT_SOURCE_DIR}/much/sim
${CBMROOT_SOURCE_DIR}/trd/sim
${CBMROOT_SOURCE_DIR}/tof/sim
${CBMROOT_SOURCE_DIR}/psd/sim
${CBMROOT_SOURCE_DIR}/sim/passive
${CBMROOT_SOURCE_DIR}/field
)

Include_Directories(${INCLUDE_DIRECTORIES})

set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${Vc_LIB_DIR}
${KFParticle_LIB_DIR}
${Boost_LIBRARY_DIRS}
)

link_directories(${LINK_DIRECTORIES})

set(SRCS
CbmGeoSetup.cxx
CbmGeoSetupField.cxx
CbmGeoSetupModule.cxx
CbmGeoSetupMedia.cxx
CbmGeoSetupProvider.cxx
CbmGeoSetupRepoProvider.cxx
CbmGeoSetupDbProvider.cxx
)

set(HEADERS

CbmGeoSetup.h
CbmGeoSetupField.h
CbmGeoSetupModule.h
CbmGeoSetupMedia.h
CbmGeoSetupProvider.h
CbmGeoSetupRepoProvider.h
CbmGeoSetupDbProvider.h

)

set(LINKDEF CbmGeoSetupLinkDef.h)
Set(LIBRARY_NAME CbmGeoSetup)
Set(DEPENDENCIES CbmData CbmBase CbmSimBase
CbmField CbmMvd CbmStsSim CbmRichSim CbmMuchSim CbmMuchBase CbmTrdBase
CbmTrdSim CbmTofSim CbmPsdSim CbmPassive
Net RSQLite)

GENERATE_LIBRARY()

install(FILES ${HEADERS} DESTINATION include/geosetup)
