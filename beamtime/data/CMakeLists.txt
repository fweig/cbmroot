# Create a library called "libCbmBeamtime" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${CBMDATA_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/fhodo
  ${CMAKE_CURRENT_SOURCE_DIR}/raw
)


Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${Boost_INCLUDE_DIR}
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
  CbmAuxDigi.cxx
  CbmTbEvent.cxx

  fhodo/CbmFiberHodoDigi.cxx
  fhodo/CbmFiberHodoCluster.cxx
  fhodo/CbmFiberHodoAddress.cxx

  raw/CbmRawMessage.cxx
  raw/CbmSpadicRawMessage.cxx
  raw/CbmTrdRawMessage.cxx
  raw/CbmNxyterRawMessage.cxx
  raw/CbmNxyterRawSyncMessage.cxx
  raw/CbmStsxyterRawHitMessage.cxx
  raw/CbmTrbRawMessage.cxx
)

Set(NO_DICT_SRCS
  raw/rocMess_wGet4v1.cxx
  CbmTofStarData.cxx
)

Set(LINKDEF  CbmBeamtimeDataLinkDef.h)
Set(LIBRARY_NAME CbmBeamtimeData)
Set(DEPENDENCIES
CbmData
)

GENERATE_LIBRARY()
