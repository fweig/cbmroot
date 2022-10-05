set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/qa
  )


set(SRCS
  CbmStsDigitize.cxx
  CbmStsMC.cxx
  CbmStsSensorPoint.cxx
  CbmStsSignal.cxx
  CbmStsSimModule.cxx
  CbmStsSimSensor.cxx
  CbmStsSimSensorDssd.cxx
  CbmStsSimSensorDssdOrtho.cxx
  CbmStsSimSensorDssdStereo.cxx
  CbmStsSimSensorFactory.cxx
  
  CbmStsDigitizePixel.cxx
  
  qa/CbmStsDigitizeQa.cxx
  qa/CbmStsDigitizeQaReport.cxx
  )


set(LIBRARY_NAME CbmStsSim)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  CbmBase
  CbmData
  CbmStsBase
  FairRoot::Base
  ROOT::Core
  ROOT::Hist
  ROOT::MathCore
  )

set(PRIVATE_DEPENDENCIES
  CbmSimBase
  FairRoot::ParBase
  ROOT::EG
  ROOT::Geom
  ROOT::Gpad
  ROOT::Physics
  ROOT::RIO
  )

generate_cbm_library()


# Install file which has no corresponding source file
install(FILES CbmStsTrackStatus.h
        DESTINATION include
        )

