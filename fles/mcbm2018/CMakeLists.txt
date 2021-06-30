# Create a library called "libCbmFlibReader" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${CBMBASE_DIR}

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/base
  ${CBMDATA_DIR}/raw
  ${CBMDATA_DIR}/sts
  ${CBMDATA_DIR}/much
  ${CBMDATA_DIR}/global
  ${CBMDATA_DIR}/rich
  ${CBMDATA_DIR}/tof
  ${CBMDATA_DIR}/psd
  ${CBMDATA_DIR}/trd

  ${CBMROOT_SOURCE_DIR}/fles/flestools

  ${CBMROOT_SOURCE_DIR}/fles/mcbm2018
  ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/dataforma
  ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/commonMQ
#  ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/parameter
  ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/unpacker
  ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/monitor
  ${CBMROOT_SOURCE_DIR}/fles/mcbm2018/tasks

  ${CBMDETECTORBASE_DIR}/trd # required for parameter handling of the trd
  ${CBMDETECTORBASE_DIR}/psd
  ${CBMDETECTORBASE_DIR}/sts
  ${CBMDETECTORBASE_DIR}/tof
  ${CBMDETECTORBASE_DIR}/much
  ${CBMDETECTORBASE_DIR}/rich
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
   CbmMcbmUnpack.cxx
   CbmMcbm2018Source.cxx
   CbmMcbm2018MsCrcCheck.cxx

   unpacker/CbmMcbm2018RawConverterSdpb.cxx
   unpacker/CbmMcbm2018RawConverterGdpb.cxx

   unpacker/CbmMcbm2018UnpackerAlgoSts.cxx
   unpacker/CbmMcbm2018UnpackerTaskSts.cxx
   unpacker/CbmMcbm2018UnpackerAlgoMuch.cxx
   unpacker/CbmMcbm2018UnpackerTaskMuch.cxx
   unpacker/CbmMcbm2018UnpackerAlgoTof.cxx
   unpacker/CbmMcbm2018UnpackerTaskTof.cxx
   unpacker/CbmMcbm2018UnpackerAlgoRich.cxx
   unpacker/CbmMcbm2018UnpackerTaskRich.cxx
   unpacker/CbmMcbm2018UnpackerTaskRich2020.cxx
   unpacker/CbmMcbm2018UnpackerAlgoRich2020.cxx
   unpacker/CbmMcbm2018UnpackerUtilRich2020.cxx
   unpacker/CbmMcbm2018UnpackerAlgoPsd.cxx
   unpacker/CbmMcbm2018UnpackerTaskPsd.cxx
   unpacker/CbmMcbm2018UnpackerAlgoTrdR.cxx
   unpacker/CbmMcbm2018UnpackerTaskTrdR.cxx

   monitor/CbmCheckDataFormatGdpb2018.cxx
   monitor/CbmMcbm2018MonitorDataRates.cxx
   monitor/CbmMcbm2018MonitorAlgoT0.cxx
   monitor/CbmMcbm2018MonitorTaskT0.cxx
   monitor/CbmMcbm2018MonitorMcbmRate.cxx
   monitor/CbmMcbm2018MonitorMcbmSync.cxx
   monitor/CbmMcbm2018MonitorMcbmPulser.cxx
#   monitor/CbmMcbm2018MonitorStsSync.cxx => Deprecated due to dependence on CERN 2017 HODO par
#   monitor/CbmMcbm2018MonitorStsPulser.cxx
   monitor/CbmMcbm2018MonitorSts.cxx
   monitor/CbmMcbm2018MonitorMuch.cxx
   monitor/CbmMcbm2018MonitorMuchLite.cxx
   monitor/CbmMcbm2018MonitorAlgoMuchLite.cxx
   monitor/CbmMcbm2018MonitorTaskMuchLite.cxx
#   monitor/CbmMcbm2018MonitorTrd.cxx
   monitor/CbmMcbm2018MonitorTof.cxx
   monitor/CbmMcbm2018MonitorTofPulser.cxx
   monitor/CbmMcbm2018MonitorAlgoTof.cxx
   monitor/CbmMcbm2018MonitorTaskTof.cxx
   monitor/CbmMcbm2018MonitorAlgoTofPulser.cxx
   monitor/CbmMcbm2018MonitorTaskTofPulser.cxx
   monitor/CbmMcbm2018TofTestFee.cxx
   monitor/CbmMcbm2018TofFeeThr.cxx
   monitor/CbmMcbm2018MonitorAlgoPsd.cxx
   monitor/CbmMcbm2018MonitorTaskPsd.cxx

   tasks/CbmCheckDigisNbCorr.cxx
   tasks/CbmCheckTiming.cxx
   tasks/CbmCheckEvents.cxx
   tasks/CbmMcbm2018EventBuilder.cxx
   tasks/CbmMcbm2019CheckPulser.cxx
   tasks/CbmMcbm2019CheckDigisMuch.cxx
   tasks/CbmMcbm2019CheckDigisSts.cxx
   tasks/CbmMcbm2019CheckDtInDet.cxx
   tasks/CbmMcbm2019CheckTimingPairs.cxx
   tasks/CbmMcbm2019TimeWinEventBuilderAlgo.cxx
   tasks/CbmMcbm2019TimeWinEventBuilderTask.cxx

   tasks/CbmMcbmCheckTimingAlgo.cxx
   tasks/CbmMcbmCheckTimingTask.cxx

   tasks/CbmMcbmSpillFindAlgo.cxx
   tasks/CbmMcbmSpillFindTask.cxx
)

If(_UINT8_T_EXIST)
  Add_Definitions(-DHAVE_UINT8_T_HEADER_FILE)
  Set(DEFINITIONS HAVE_UINT8_T_HEADER_FILE)
EndIf()

Set(LINKDEF CbmFlibMcbm2018LinkDef.h )
Set(LIBRARY_NAME CbmFlibMcbm2018)
Set(DEPENDENCIES
    CbmFlibFlesTools CbmStsBase CbmTofBase
    CbmRichBase CbmMuchBase CbmTrdBase CbmPsdBase
    CbmData CbmBase fles_ipc Eve Base
)

GENERATE_LIBRARY()

# Install file which has no corresponding source file
install(FILES commonMQ/CbmStar2019Algo.h DESTINATION include)
