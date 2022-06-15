# Create a library called "libTrd" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}
${CBMBASE_DIR}
${CBMDATA_DIR}
${CBMDATA_DIR}/trd
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${FAIRROOT_LIBRARY_DIR}
${ROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)

link_directories( ${LINK_DIRECTORIES})

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



set(LINKDEF CbmTrdBaseLinkDef.h)
Set(LIBRARY_NAME CbmTrdBase)
Set(DEPENDENCIES
    CbmBase Base
)

GENERATE_LIBRARY()

#Install files without a corresponding source file
Install(FILES CbmTrdDefs.h
  DESTINATION include
)
