set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/report
  ${CMAKE_CURRENT_SOURCE_DIR}/draw
  ${CMAKE_CURRENT_SOURCE_DIR}/utils
  )

set(SRCS
  CbmDigiManager.cxx
  CbmDigitizeBase.cxx
  CbmDaq.cxx
  CbmMCDataArray.cxx
  CbmMCDataManager.cxx
  CbmMCDataObject.cxx
  CbmRadDamage.cxx
  CbmHistManager.cxx
  CbmMatchRecoToMC.cxx
  CbmTrackingDetectorInterfaceBase.cxx
  draw/CbmDrawHist.cxx
  report/CbmReport.cxx
  report/CbmStudyReport.cxx
  report/CbmSimulationReport.cxx
  report/CbmTextReportElement.cxx
  report/CbmHtmlReportElement.cxx
  report/CbmLatexReportElement.cxx
  utils/CbmUtils.cxx
  utils/CbmGeometryUtils.cxx
  utils/CbmMcbmUtils.cxx
  utils/CbmMediaList.cxx
  utils/CbmFileUtils.cxx
  )

set(LIBRARY_NAME CbmBase)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)

set(PUBLIC_DEPENDENCIES
  CbmData
  FairRoot::Base
  GSL
  ROOT::Core
  ROOT::Gpad
  ROOT::Hist
  )

set(PRIVATE_DEPENDENCIES
  FairLogger::FairLogger
  FairRoot::GeoBase
  Boost::filesystem
  ROOT::Geom
  ROOT::Graf
  ROOT::MathCore
  ROOT::RIO
  ROOT::Tree
  )

generate_cbm_library()

Install(FILES CbmDigitize.h CbmTrackingDetectorInterfaceBase.h report/CbmReportElement.h
        DESTINATION include
       )
