Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
  ${Boost_INCLUDE_DIR}
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR} 
  ${CMAKE_CURRENT_SOURCE_DIR}/report
  ${CMAKE_CURRENT_SOURCE_DIR}/draw
  ${CMAKE_CURRENT_SOURCE_DIR}/utils

  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/base
  ${CBMDATA_DIR}/sts
  ${CBMDATA_DIR}/tof
  ${CBMDATA_DIR}/rich
  ${CBMDATA_DIR}/much
  ${CBMDATA_DIR}/mvd
  ${CBMDATA_DIR}/trd
  ${CBMDATA_DIR}/psd
)
Include_Directories( ${INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)
 
link_directories( ${LINK_DIRECTORIES})

Set(SRCS
CbmDigiManager.cxx

CbmDigitizeBase.cxx
CbmDaq.cxx

CbmMCDataArray.cxx
CbmMCDataManager.cxx
CbmMCDataObject.cxx
CbmRadDamage.cxx

CbmHistManager.cxx
CbmMatchRecoToMC.cxx

draw/CbmDrawHist.cxx

report/CbmReport.cxx      
report/CbmStudyReport.cxx
report/CbmSimulationReport.cxx
report/CbmTextReportElement.cxx
report/CbmHtmlReportElement.cxx       
report/CbmLatexReportElement.cxx

utils/CbmUtils.cxx
utils/CbmGeometryUtils.cxx
utils/CbmMediaList.cxx
utils/CbmFileUtils.cxx
)


Set(LINKDEF CbmBaseLinkDef.h)
Set(LIBRARY_NAME CbmBase)
Set(DEPENDENCIES
    CbmData Base boost_regex boost_filesystem
)

GENERATE_LIBRARY()

Install(FILES CbmDigitize.h report/CbmReportElement.h
        DESTINATION include
       )
