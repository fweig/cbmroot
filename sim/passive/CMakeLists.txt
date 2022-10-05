set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmCave.cxx            
  CbmGeoCave.cxx         
  #CbmGeoMagnet.cxx       
  CbmGeoPassivePar.cxx   
  #CbmGeoPipe.cxx         
  CbmGeoPlatform.cxx
  CbmMagnet.cxx          
  CbmPassiveContFact.cxx 
  CbmPipe.cxx
  CbmPlatform.cxx
  #CbmShield.cxx
  #CbmShieldGeo.cxx         
  )

set(LIBRARY_NAME CbmPassive)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  FairRoot::Base
  FairRoot::GeoBase
  FairRoot::ParBase
  ROOT::Core
  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  ROOT::Geom
  ROOT::RIO
  )


generate_cbm_library()
