set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmGeoMuchPar.cxx
  CbmMcbm2018MuchPar.cxx
  CbmMuchUnpackPar.cxx
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
  
  CbmMuchTrackingInterface.cxx
  )

set(LIBRARY_NAME CbmMuchBase)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES 
  CbmData 
  FairRoot::Base
  FairRoot::ParBase
  ROOT::Core 
  ROOT::Geom 
  ROOT::Graf 
  ROOT::MathCore 
  ROOT::Physics
  CbmBase
  )

set(PRIVATE_DEPENDENCIES 
  FairLogger::FairLogger 
  ROOT::Gpad 
  ROOT::Hist 
  ROOT::RIO
  )

generate_cbm_library()
