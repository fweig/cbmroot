Set(INCLUDE_DIRECTORIES
  ${CBMBASE_DIR}

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/beamtime
  ${CBMDATA_DIR}/beamtime/fhodo
  ${CBMDATA_DIR}/raw
  ${CBMDATA_DIR}/sts
  ${CBMDATA_DIR}/much

  ${CBMROOT_SOURCE_DIR}/beamtime/base

  ${CBMROOT_SOURCE_DIR}/beamtime/cern2017/param

  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURREN_SOURCE_DIR}/unpacker
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
  unpacker/CbmCosy2018MonitorSts.cxx
  unpacker/CbmCosy2018MonitorHodo.cxx
  unpacker/CbmCosy2018MonitorSetup.cxx
  unpacker/CbmCosy2018MonitorDupli.cxx
  unpacker/CbmCosy2018MonitorSetupGood.cxx
  unpacker/CbmCosy2018MonitorEfficiency.cxx
  unpacker/CbmCosy2018MonitorPulser.cxx
  unpacker/CbmCosy2018MonitorScan.cxx
)

If(_UINT8_T_EXIST)
  Add_Definitions(-DHAVE_UINT8_T_HEADER_FILE)
  Set(DEFINITIONS HAVE_UINT8_T_HEADER_FILE)
EndIf()

Set(LINKDEF CbmBeamtimeCosy2018LinkDef.h )
Set(LIBRARY_NAME CbmBeamtimeCosy2018)
Set(DEPENDENCIES
    CbmBeamtimeCern2017 CbmBase CbmBeamtimeBase fles_ipc Eve Base
)

GENERATE_LIBRARY()
