Set(INCLUDE_DIRECTORIES
  ${CBMBASE_DIR}

  ${CBMDATA_DIR}/raw

  ${CBMROOT_SOURCE_DIR}/beamtime/base
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
  unpacker/StsXyterRawMessage.cxx
  unpacker/StsXyterBetaMessage.cxx

  unpacker/CbmCern2017MonitorRawSts.cxx
  unpacker/CbmCern2017MonitorBetaSts.cxx
  unpacker/CbmCern2017MonitorSts.cxx
  unpacker/CbmCern2017MonitorHodo.cxx

  param/CbmCern2017ContFact.cxx
  param/CbmCern2017UnpackParSts.cxx
  param/CbmCern2017UnpackParHodo.cxx
)

If(_UINT8_T_EXIST)
  Add_Definitions(-DHAVE_UINT8_T_HEADER_FILE)
  Set(DEFINITIONS HAVE_UINT8_T_HEADER_FILE)
EndIf()

Set(LINKDEF CbmBeamtimeCern2017LinkDef.h )
Set(LIBRARY_NAME CbmBeamtimeCern2017)
Set(DEPENDENCIES
    CbmData CbmBase CbmBeamtimeBase fles_ipc Eve Base
)

GENERATE_LIBRARY()
