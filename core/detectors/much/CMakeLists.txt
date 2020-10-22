# Create a library called "libBase" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed
# here.

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}

  ${CBMDATA_DIR} 
  ${CBMDATA_DIR}/much 
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
CbmGeoMuchPar.cxx 
CbmMuchContFact.cxx  
CbmMuchGeoScheme.cxx
CbmMuchLayer.cxx
CbmMuchLayerSide.cxx
CbmMuchModule.cxx
CbmMuchModuleGem.cxx
CbmMuchModuleGemRadial.cxx
CbmMuchModuleGemRectangular.cxx
CbmMuchPad.cxx
CbmMuchPadRadial.cxx
CbmMuchPadRectangular.cxx
CbmMuchPointInfo.cxx
CbmMuchSector.cxx
CbmMuchSectorRadial.cxx
CbmMuchSectorRectangular.cxx
CbmMuchStation.cxx

CbmMuchSegmentAuto.cxx
CbmMuchSegmentManual.cxx
CbmMuchSegmentSector.cxx
)

set(LINKDEF CbmMuchBaseLinkDef.h)
Set(LIBRARY_NAME CbmMuchBase)
Set(DEPENDENCIES
  CbmData Base 
)

GENERATE_LIBRARY()
