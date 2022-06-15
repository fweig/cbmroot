# CMakeList file for library libStsBase
# Last update: V. Friese, 16.03.2020


# -----   Specify library name   --------------------------
Set(LIBRARY_NAME CbmStsBase)
# ---------------------------------------------------------


# -----  Specify sources for compilation   ----------------
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
# -----  End of sources   ---------------------------------



# ----  Specify include directories -----------------------
set(INCLUDE_DIRECTORIES
${CMAKE_CURRENT_SOURCE_DIR}

${CBMBASE_DIR}

${CBMDATA_DIR}
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/global
)

set(SYSTEM_INCLUDE_DIRECTORIES
${BASE_INCLUDE_DIRECTORIES}
)
# ----  End of include directories ------------------------


# ----  Specify link directories --------------------------
set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)
# -----  End of link directories   ------------------------


# -----   Specify library dependences   -------------------
Set(DEPENDENCIES CbmBase CbmData)
# ---------------------------------------------------------


# -----  Specify LinkDef file   ---------------------------
set(LINKDEF ${LIBRARY_NAME}LinkDef.h)
# ---------------------------------------------------------


# ---- Enable OpenMP  -------------------------------------
find_package(OpenMP)
if (OPENMP_FOUND)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
# ---------------------------------------------------------


# -----  Let cmake do the job   ---------------------------
include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
link_directories( ${LINK_DIRECTORIES})
GENERATE_LIBRARY()
# ---------------------------------------------------------


# -----   Install headers   -------------------------------
Install(
FILES CbmStsContFact.h
	  CbmStsDefs.h
	  CbmStsElement.h
	  CbmStsModule.h
	  CbmStsParAsic.h
	  CbmStsParModule.h
	  CbmStsParSensor.h
	  CbmStsParSensorCond.h
	  CbmStsParSetModule.h
	  CbmStsParSetSensor.h
	  CbmStsParSetSensorCond.h
	  CbmStsParSim.h
	  CbmStsPhysics.h
	  CbmStsSensor.h
	  CbmStsSetup.h
	  CbmStsStation.h
    CbmStsTrackingInterface.h
DESTINATION include
)
# ---------------------------------------------------------


