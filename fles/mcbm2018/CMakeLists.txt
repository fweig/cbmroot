# Create a library called "libCbmFlibReader" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.
set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/dataformat
  ${CMAKE_CURRENT_SOURCE_DIR}/commonMQ
  ${CMAKE_CURRENT_SOURCE_DIR}/unpacker
  ${CMAKE_CURRENT_SOURCE_DIR}/tasks
  ${CMAKE_CURRENT_SOURCE_DIR}/monitor
  )

set(SRCS
   CbmMcbmUnpack.cxx
   CbmMcbm2018Source.cxx
   CbmMcbm2018MsCrcCheck.cxx

   unpacker/CbmMcbm2018RawConverterSdpb.cxx
   unpacker/CbmMcbm2018RawConverterGdpb.cxx
   unpacker/CbmCriGet4RawPrint.cxx

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


set(LIBRARY_NAME CbmFlibMcbm2018)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h )
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  CbmTrdBase
  CbmPsdBase
  FairRoot::Base
  external::fles_ipc
  flib_dpb
  ROOT::Core
  ROOT::Hist
  )

set(PRIVATE_DEPENDENCIES
  CbmFlibFlesTools
  CbmMuchBase
  CbmRichBase
  CbmStsBase
  CbmTofBase
  FairLogger::FairLogger
  FairRoot::ParBase
  cppzmq
  external::fles_logging
  ROOT::Gpad
  ROOT::Graf
  ROOT::MathCore
  ROOT::Physics
  ROOT::RHTTP
  ROOT::RIO  
  )

generate_cbm_library()

# Install file which has no corresponding source file
install(FILES commonMQ/CbmStar2019Algo.h 
        DESTINATION include
       )

