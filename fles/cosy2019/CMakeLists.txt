# Create a library called "libCbmFlibReader" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/parameter
  ${CMAKE_CURRENT_SOURCE_DIR}/unpacker
  ${CMAKE_CURRENT_SOURCE_DIR}/monitor

  ${CBMBASE_DIR}

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/base
  ${CBMDATA_DIR}/raw
  ${CBMDATA_DIR}/sts

  ${CBMROOT_SOURCE_DIR}/fles/flestools

  ${CBMROOT_SOURCE_DIR}/fles/mcbm2018
  ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/unpacker
  ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/commonMQ

  ${CBMDETECTORBASE_DIR}/sts
)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${Boost_INCLUDE_DIR}
  ${ROOT_INCLUDE_DIR}
  ${IPC_INCLUDE_DIRECTORY}
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

   parameter/CbmCosy2019HodoPar.cxx
   parameter/CbmCosy2019ContFact.cxx

   unpacker/CbmCosy2019UnpackerAlgoHodo.cxx
   unpacker/CbmCosy2019UnpackerTaskHodo.cxx

   monitor/CbmCosy2019MonitorAlgoHodo.cxx
   monitor/CbmCosy2019MonitorTaskHodo.cxx
)

If(_UINT8_T_EXIST)
  Add_Definitions(-DHAVE_UINT8_T_HEADER_FILE)
  Set(DEFINITIONS HAVE_UINT8_T_HEADER_FILE)
EndIf()

Set(LINKDEF CbmBeamtimeCosy2019LinkDef.h )
Set(LIBRARY_NAME CbmBeamtimeCosy2019)
Set(DEPENDENCIES
    CbmFlibFlesTools CbmFlibMcbm2018
    CbmStsBase
    CbmData CbmBase fles_ipc Eve Base
)

GENERATE_LIBRARY()
