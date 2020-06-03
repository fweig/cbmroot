# CMakeList file for library libStsSim
# Last update: V. Friese, 13.03.2020


# -----   Specify library name   --------------------------
Set(LIBRARY_NAME CbmStsSim)
# ---------------------------------------------------------


# -----  Specify sources for compilation   ----------------
set(SRCS
CbmStsDigitize.cxx
CbmStsDigitizeQa.cxx
CbmStsDigitizeQaReport.cxx
CbmStsMC.cxx
CbmStsSensorPoint.cxx
CbmStsSignal.cxx
CbmStsSimModule.cxx
CbmStsSimSensor.cxx
CbmStsSimSensorDssd.cxx
CbmStsSimSensorDssdOrtho.cxx
CbmStsSimSensorDssdStereo.cxx
CbmStsSimSensorFactory.cxx
)
# -----  End of sources   ---------------------------------



# ----  Specify include directories -----------------------
set(INCLUDE_DIRECTORIES

# --- this directory
${CBMROOT_SOURCE_DIR}/sim/detectors/sts

# --- sts base
${CBMDETECTORBASE_DIR}/sts

# --- core/base
${CBMBASE_DIR}
${CBMBASE_DIR}/draw
${CBMBASE_DIR}/report
${CBMBASE_DIR}/utils

# --- core/data
${CBMDATA_DIR}
${CBMDATA_DIR}/base
${CBMDATA_DIR}/sts
${CBMDATA_DIR}/global

# --- sim
${CBMROOT_SOURCE_DIR}/sim/transport/base
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
Set(DEPENDENCIES
    CbmBase CbmData CbmStsBase
)
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


# -----  Create the library  ------------------------------
include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
link_directories( ${LINK_DIRECTORIES})
GENERATE_LIBRARY()
# ---------------------------------------------------------


# -----   Install headers   -------------------------------
Install(
FILES CbmStsDigitize.h
	  CbmStsDigitizeQa.h
	  CbmStsDigitizeQaReport.h
	  CbmStsMC.h
	  CbmStsSensorPoint.h
	  CbmStsSignal.h
	  CbmStsSimModule.h
	  CbmStsSimSensor.h
	  CbmStsSimSensorDssd.h
	  CbmStsSimSensorDssdStereo.h
	  CbmStsSimSensorDssdOrtho.h
	  CbmStsSimSensorFactory.h
	  CbmStsTrackStatus.h
DESTINATION include
)
# ---------------------------------------------------------

