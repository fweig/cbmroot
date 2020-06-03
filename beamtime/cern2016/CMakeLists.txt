# Create a library called "libCbmFlibReader" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${CBMBASE_DIR}

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/tof
  ${CBMDATA_DIR}/much

  ${CBMROOT_SOURCE_DIR}/beamtime/base
  ${CBMROOT_SOURCE_DIR}/beamtime/param
  ${CBMROOT_SOURCE_DIR}/beamtime/data/fhodo
  ${CBMROOT_SOURCE_DIR}/beamtime/data/raw
  ${CBMROOT_SOURCE_DIR}/beamtime/data

  ${CBMROOT_SOURCE_DIR}/fles/flestools

  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/unpacker
  ${CMAKE_CURRENT_SOURCE_DIR}/param
)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${Boost_INCLUDE_DIR}
  ${ROOT_INCLUDE_DIR}
  ${IPC_INCLUDE_DIRECTORY}
  ${CBMROOT_SOURCE_DIR}/external/flib_dpb/flib_dpb
  ${CMAKE_SOURCE_DIR}/external/cppzmq
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})


Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

Link_Directories(
  ${LINK_DIRECTORIES}
)

Set(SRCS
#  CbmFlibTestSource.cxx
  CbmFlibCern2016Source.cxx

  unpacker/CbmTSUnpackFHodo.cxx
  unpacker/CbmTSUnpackMuch.cxx
  unpacker/CbmTSUnpackTof.cxx

  unpacker/CbmTSMonitorTofLegacy.cxx
  unpacker/CbmTSMonitorTof.cxx
  unpacker/CbmTSMonitorMuch.cxx
)

If(_UINT8_T_EXIST)
  Add_Definitions(-DHAVE_UINT8_T_HEADER_FILE)
  Set(DEFINITIONS HAVE_UINT8_T_HEADER_FILE)
EndIf()

Set(LINKDEF CbmBeamtimeCern2016LinkDef.h )
Set(LIBRARY_NAME CbmBeamtimeCern2016)
Set(DEPENDENCIES
    CbmFlibFlesTools CbmBase CbmBeamtimeBase CbmBeamtimeData CbmBeamtimeParam fles_ipc Eve Base
)

GENERATE_LIBRARY()
