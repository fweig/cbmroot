set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}/checker
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
  CbmQaCanvas.cxx
  CbmQaEff.cxx
  CbmQaPie.cxx
  CbmQaHist.cxx
  CbmQaTable.cxx
  CbmQaTask.cxx
  CbmQaIO.cxx
  checker/CbmQaCheckerCore.cxx
  checker/CbmQaCheckerFileHandler.cxx
  checker/CbmQaCheckerHist1DHandler.cxx
  checker/CbmQaCheckerHist2DHandler.cxx
  checker/CbmQaCheckerObjectHandler.cxx
  checker/CbmQaCheckerProfile1DHandler.cxx
  checker/CbmQaCheckerObjectDB.cxx
  )

set(HEADERS
  CbmQaConstants.h
  )

set(LIBRARY_NAME CbmQaBase)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  FairRoot::Base
  FairLogger::FairLogger
  external::yaml-cpp
  ROOT::Core
  ROOT::Gpad
  ROOT::Hist
  )

set(PRIVATE_DEPENDENCIES
  external::yaml-cpp
  )

set(INTERFACE_DEPENDENCIES
  ROOT::Graf
  )

generate_cbm_library()

Install(FILES CbmQaTask.h CbmQaCanvas.h CbmQaTable.h CbmQaHist.h CbmQaEff.h CbmQaPie.h CbmQaConstants.h CbmQaCmpDrawer.h checker/CbmQaCheckerTypedefs.h
        DESTINATION include
        )

