# Create a library called "libCbmFlibReader" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/parameter
  ${CMAKE_CURRENT_SOURCE_DIR}/unpacker
  ${CMAKE_CURRENT_SOURCE_DIR}/monitor
)


set(SRCS
   parameter/CbmCosy2019HodoPar.cxx
   parameter/CbmCosy2019ContFact.cxx

   unpacker/CbmCosy2019UnpackerAlgoHodo.cxx
   unpacker/CbmCosy2019UnpackerTaskHodo.cxx
   unpacker/CbmStsCoincHodo.cxx
   
   monitor/CbmCosy2019MonitorAlgoHodo.cxx
   monitor/CbmCosy2019MonitorTaskHodo.cxx
)


If(_UINT8_T_EXIST)
  Add_Definitions(-DHAVE_UINT8_T_HEADER_FILE)
  Set(DEFINITIONS HAVE_UINT8_T_HEADER_FILE)
EndIf()


set(LIBRARY_NAME CbmBeamtimeCosy2019)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h )
set(PUBLIC_DEPENDENCIES
  CbmData
  CbmFlibMcbm2018
  FairRoot::Base
  FairRoot::ParBase
  ROOT::Core
  ROOT::Gpad
  ROOT::Hist

  )

set(PRIVATE_DEPENDENCIES
  CbmFlibFlesTools
  CbmStsBase
  FairLogger::FairLogger
  FairRoot::Online
  ROOT::MathCore
  ROOT::RHTTP
  ROOT::RIO
  )

set(INTERFACE_DEPENDENCIES
  external::fles_ipc
  )


generate_cbm_library()
