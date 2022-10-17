# Create a library called "libCbmFlibReader" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/parameter
 # ${CMAKE_CURRENT_SOURCE_DIR}/commonMQ
#  ${CBMROOT_SOURCE_DIR}/fles/star2019/unpacker
  ${CMAKE_CURRENT_SOURCE_DIR}/monitor
  ${CMAKE_CURRENT_SOURCE_DIR}/eventbuilder
)

set(SRCS
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

set(LIBRARY_NAME CbmFlibStar2019)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h )
set(PUBLIC_DEPENDENCIES
  CbmData
  FairRoot::ParBase
  ROOT::Core
  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  CbmFlibFlesTools
  CbmFlibMcbm2018
  FairRoot::Base
  ROOT::Gpad
  ROOT::Graf
  ROOT::Hist
  ROOT::MathCore
  ROOT::RHTTP
  ROOT::RIO
  )

set(INTERFACE_DEPENDENCIES
  external::fles_ipc
  )

generate_cbm_library()

