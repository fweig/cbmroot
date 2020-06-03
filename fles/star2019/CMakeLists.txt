# Create a library called "libCbmFlibReader" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${CBMBASE_DIR}

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/raw
  ${CBMDATA_DIR}/tof
  ${CBMDATA_DIR}/tof/etof # For the star_rhicf.c interface file and the final data format classes


  ${CBMROOT_SOURCE_DIR}/fles/flestools

#  ${CBMROOT_SOURCE_DIR}/fles/star2017/unpacker # For the star_rhicf.c interface file

#  ${CBMROOT_SOURCE_DIR}/fles/star2018/unpacker # for the STAR2018 Event and Trigger format

  ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/
  ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/dataformat
  ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/commonMQ

  ${CBMROOT_SOURCE_DIR}/fles/star2019/
  ${CBMROOT_SOURCE_DIR}/fles/star2019/parameter
  ${CBMROOT_SOURCE_DIR}/fles/star2019/commonMQ
#  ${CBMROOT_SOURCE_DIR}/fles/star2019/unpacker
  ${CBMROOT_SOURCE_DIR}/fles/star2019/monitor
  ${CBMROOT_SOURCE_DIR}/fles/star2019/eventbuilder
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
   parameter/CbmStar2019TofPar.cxx
   parameter/CbmStar2019ContFact.cxx

   monitor/CbmStar2019MonitorAlgo.cxx
   monitor/CbmStar2019MonitorTask.cxx
   monitor/CbmStar2019MonitorPulserAlgo.cxx
   monitor/CbmStar2019MonitorPulserTask.cxx

   eventbuilder/CbmTofStarData2019.cxx
   eventbuilder/CbmStar2019EventBuilderEtofAlgo.cxx
   eventbuilder/CbmStar2019EventBuilderEtof.cxx
)

If(_UINT8_T_EXIST)
  Add_Definitions(-DHAVE_UINT8_T_HEADER_FILE)
  Set(DEFINITIONS HAVE_UINT8_T_HEADER_FILE)
EndIf()

Set(LINKDEF CbmFlibStar2019LinkDef.h )
Set(LIBRARY_NAME CbmFlibStar2019)
Set(DEPENDENCIES
    CbmFlibMcbm2018 CbmData CbmBase fles_ipc Eve Base
)

GENERATE_LIBRARY()
