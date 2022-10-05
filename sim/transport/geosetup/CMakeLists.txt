set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmGeoSetup.cxx
  CbmGeoSetupField.cxx
  CbmGeoSetupModule.cxx
  CbmGeoSetupMedia.cxx
  CbmGeoSetupProvider.cxx
  CbmGeoSetupRepoProvider.cxx
  CbmGeoSetupDbProvider.cxx
  )

set(INSTALL_HEADERS
  CbmGeoSetup.h
  CbmGeoSetupField.h
  CbmGeoSetupModule.h
  CbmGeoSetupMedia.h
  CbmGeoSetupProvider.h
  CbmGeoSetupRepoProvider.h
  CbmGeoSetupDbProvider.h
  )

set(LIBRARY_NAME CbmGeoSetup)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmData
  ROOT::Core
  )

set(PRIVATE_DEPENDENCIES
  CbmMuchSim
  CbmMvd
  CbmPassive
  CbmPsdSim
  CbmRichSim
  CbmSimBase
  CbmStsSim
  CbmTofSim
  CbmTrdSim
  FairRoot::Base
  ROOT::Net
  ROOT::RSQLite
  
  )

set(INTERFACE_DEPENDENCIES
  ROOT::Geom
  )



generate_cbm_library()


install(FILES 
       ${INSTALL_HEADERS} 
       DESTINATION include/geosetup
       )
