set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmTrdGas.cxx
  CbmTrdContFact.cxx
  CbmTrdParManager.cxx
  CbmTrdModuleAbstract.cxx
  CbmMcbm2020TrdTshiftPar.cxx
  CbmTrdParSet.cxx
  CbmTrdParSetAsic.cxx
  CbmTrdParSetGas.cxx
  CbmTrdParSetGain.cxx
  CbmTrdParSetGeo.cxx
  CbmTrdParSetDigi.cxx
  CbmTrdParMod.cxx
  CbmTrdParAsic.cxx
  CbmTrdParSpadic.cxx
  CbmTrdSpadic.cxx
  CbmTrdFASP.cxx
  CbmTrdParFasp.cxx
  CbmTrdParModDigi.cxx
  CbmTrdParModGas.cxx
  CbmTrdParModGain.cxx
  CbmTrdParModGeo.cxx
  CbmTrdTrackingInterface.cxx
  #tools
  CbmTrdGeoHandler.cxx
  CbmTrdUtils.cxx
  CbmTrddEdxUtils.cxx
  CbmTrdHardwareSetupR.cxx
  CbmTrdRadiator.cxx
  )


IF (SSE_FOUND)
  ADD_DEFINITIONS(-DHAVE_SSE)
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS
    "-msse -O3")
  Message(STATUS "TRD BASE will be compiled with SSE support")
ELSE (SSE_FOUND)
  Message(STATUS "TRD BASE will be compiled without SSE support")
  SET_SOURCE_FILES_PROPERTIES(${SRCS} PROPERTIES COMPILE_FLAGS
    "-O3")
ENDIF (SSE_FOUND)


set(LIBRARY_NAME CbmTrdBase)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
#set(DEPENDENCIES CbmBase CbmData)
set(PUBLIC_DEPENDENCIES 
  CbmData
  FairRoot::Base
  FairRoot::ParBase
  ROOT::Core 
  ROOT::Hist 
  ROOT::MathCore 
  ROOT::Physics 
  ROOT::RIO
  )

set(PRIVATE_DEPENDENCIES   
  FairLogger::FairLogger 
  CbmBase
  ROOT::Geom 
  ROOT::Gpad 
  ROOT::Graf
  )


generate_cbm_library()

# Install file which has no corresponding source file
install(FILES
        CbmTrdDefs.h
        DESTINATION include
       )
