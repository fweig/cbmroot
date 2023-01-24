set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )


set(SRCS
  CbmStsContFact.cxx
  CbmStsElement.cxx
  CbmStsModule.cxx
  CbmStsParAsic.cxx
  CbmStsParModule.cxx
  CbmStsParSensor.cxx
  CbmStsParSensorCond.cxx
  CbmStsParSetModule.cxx
  CbmStsParSetSensor.cxx
  CbmStsParSetSensorCond.cxx
  CbmStsParSim.cxx
  CbmMcbm2018StsPar.cxx
  CbmStsPhysics.cxx
  CbmStsSensor.cxx
  CbmStsSetup.cxx
  CbmStsStation.cxx
  CbmStsTrackingInterface.cxx
  )


# ---- Enable OpenMP  -------------------------------------
find_package(OpenMP)
if (OPENMP_FOUND)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
# ---------------------------------------------------------

set(LIBRARY_NAME CbmStsBase)
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
set(PUBLIC_DEPENDENCIES
  FairRoot::Base
  FairRoot::ParBase
  CbmBase
  ROOT::Core
  ROOT::Geom
  ROOT::Gpad
  ROOT::Hist
  ROOT::MathCore
  ROOT::Physics
  )

set(PRIVATE_DEPENDENCIES
  FairLogger::FairLogger
  CbmData
  ROOT::Graf
  ROOT::MLP
  ROOT::Tree
  )

generate_cbm_library()

# Install file which has no corresponding source file
install(FILES
        CbmStsDefs.h
        DESTINATION include
       )

