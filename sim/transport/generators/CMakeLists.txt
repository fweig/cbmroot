set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/unigen
  ${CMAKE_CURRENT_SOURCE_DIR}/pluto
  )

set(SRCS
  CbmBeamGenerator.cxx
  CbmPhsdGenerator.cxx
  CbmPlutoGenerator.cxx
  CbmShieldGenerator.cxx
  CbmUnigenGenerator.cxx
  unigen/URun.cxx
  unigen/UEvent.cxx
  unigen/UParticle.cxx
  pluto/PDataBase.cxx
  pluto/PMesh.cxx
  pluto/PParticle.cxx
  pluto/PStaticData.cxx
  pluto/PStdData.cxx
  pluto/PValues.cxx
  )


set(LIBRARY_NAME CbmSimGenerators)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  FairRoot::Base
  ROOT::Core
  ROOT::Hist
  ROOT::MathCore
  ROOT::Physics
  )

set(PRIVATE_DEPENDENCIES
  CbmBase
  CbmSimBase
  FairLogger::FairLogger
  ROOT::EG
  ROOT::RIO
  ROOT::Tree
  )



generate_cbm_library()

# Install file which has no corresponding source file
install(FILES
        pluto/Pdefines.h
        DESTINATION include
       )
