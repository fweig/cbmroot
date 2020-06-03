set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories(${INCLUDE_DIRECTORIES})

set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
)

include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
)
 
link_directories(${LINK_DIRECTORIES})

set(SRCS
  CbmFieldConst.cxx     
  CbmFieldContFact.cxx    
  CbmFieldMap.cxx
  CbmFieldMapCreator.cxx         
  CbmFieldMapData.cxx     
  CbmFieldMapSym2.cxx     
  CbmFieldMapSym3.cxx     
  CbmFieldPar.cxx         
  CbmBsField.cxx 
  CbmFieldCreator.cxx 
  CbmFieldMapDistorted.cxx       
  CbmFieldMapSym1.cxx     
)

set(LINKDEF CbmFieldLinkDef.h)
Set(LIBRARY_NAME CbmField)
Set(DEPENDENCIES
    Base 
)

GENERATE_LIBRARY()

