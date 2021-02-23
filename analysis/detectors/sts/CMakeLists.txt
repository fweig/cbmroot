# CMakeList file for library libStsAna
# Last update: V. Friese, 09.04.2020


# -----   Specify library name   --------------------------
Set(LIBRARY_NAME CbmStsAna)
# ---------------------------------------------------------


# -----  Specify sources for compilation   ----------------
set(SRCS
CbmStsWkn.cxx
)
# -----  End of sources   ---------------------------------



# ----  Specify include directories -----------------------
set(INCLUDE_DIRECTORIES
${CBMROOT_SOURCE_DIR}/analysis/detectors/sts

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


# -----  Let cmake do the job   ---------------------------
include_directories( ${INCLUDE_DIRECTORIES})
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
link_directories( ${LINK_DIRECTORIES})
GENERATE_LIBRARY()
# ---------------------------------------------------------


# -----  Install headers   --------------------------------
Install(
FILES CbmStsWkn.h
DESTINATION include
)
# ---------------------------------------------------------
