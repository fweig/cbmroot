# Create a library called "libCbmFlibReader" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${CBMBASE_DIR}

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/tof
  ${CBMDATA_DIR}/tof/etof # For the star_rhicf.c interface file

  ${CBMROOT_SOURCE_DIR}/beamtime/base
  ${CBMROOT_SOURCE_DIR}/beamtime/param
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
  CbmTofStar2018Source.cxx

  param/CbmTofStar2018Par.cxx
  param/CbmTofStar2018ContFact.cxx

  unpacker/rocMess_wGet4v2.cxx
  unpacker/CbmTofStarData2018.cxx
  unpacker/CbmTofStarMonitor2018.cxx
  unpacker/CbmTofStarMonitorShift2018.cxx
  unpacker/CbmTofStarEventBuilder2018.cxx
#  unpacker/CbmUnpackTofStar2018.cxx
)

If(_UINT8_T_EXIST)
  Add_Definitions(-DHAVE_UINT8_T_HEADER_FILE)
  Set(DEFINITIONS HAVE_UINT8_T_HEADER_FILE)
EndIf()

Set(LINKDEF CbmBeamtimeStar2018LinkDef.h )
Set(LIBRARY_NAME CbmBeamtimeStar2018)
Set(DEPENDENCIES
    CbmBase CbmBeamtimeBase CbmBeamtimeData fles_ipc Eve Base
)

GENERATE_LIBRARY()
