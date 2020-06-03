# Create a library called "libCbmBeamtime" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${CBMBASE_DIR}
  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/mvd
  ${CBMDATA_DIR}/sts
  ${CBMDATA_DIR}/rich
  ${CBMDATA_DIR}/much
  ${CBMDATA_DIR}/trd
  ${CBMDATA_DIR}/tof
  ${CBMDATA_DIR}/psd
  ${CMAKE_SOURCE_DIR}/beamtime/data/
  ${CBMROOT_SOURCE_DIR}/beamtime/base
  ${CMAKE_SOURCE_DIR}/beamtime/data/fhodo
  ${CMAKE_SOURCE_DIR}/beamtime/roclight
)


Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
  ${Boost_INCLUDE_DIR}
  ${IPC_INCLUDE_DIRECTORY}
)

Include_Directories( 
  ${INCLUDE_DIRECTORIES}
)

Include_Directories( 
  SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES}
)

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
  ${FAIRROOT_LIBRARY_DIR}
)
 
Link_Directories( 
  ${LINK_DIRECTORIES}
)

Set(SRCS
  CbmROCUnpack.cxx
  CbmTbDaqBuffer.cxx
  CbmTbEventBuilder.cxx
  CbmTbEventBuilderFixedTimeWindow.cxx
  CbmTSUnpack.cxx
)

If(_UINT8_T_EXIST)
  Add_Definitions(-DHAVE_UINT8_T_HEADER_FILE)
  Set(DEFINITIONS HAVE_UINT8_T_HEADER_FILE)
EndIf()

Set(LINKDEF  CbmBeamtimeBaseLinkDef.h)
Set(LIBRARY_NAME CbmBeamtimeBase)
Set(DEPENDENCIES
    CbmBeamtimeData
    CbmRocLight
    CbmBase
    Base 
)

GENERATE_LIBRARY()
