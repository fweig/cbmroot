set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


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


set(LIBRARY_NAME CbmField)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES  
  FairRoot::Base
  FairRoot::ParBase
  ROOT::Core  
  )

set(PRIVATE_DEPENDENCIES 
  ROOT::MathCore 
  ROOT::RIO 
  ROOT::Hist
  ${VMCLIB}
  )

generate_cbm_library()
