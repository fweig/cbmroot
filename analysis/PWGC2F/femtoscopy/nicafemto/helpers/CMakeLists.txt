# Create a library called "" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
Set(INCLUDE_DIRECTORIES
  ${CMAKE_SOURCE_FAIRMCSTACK}
  ${NICAFEMTO_FEATURES}
  ${NICAFEMTO_DATA}
  ${KF_HEADERS}
  ${CBMDATA_DIR}
  ${CBMDATA_DIR}/global
  ${CBMDATA_DIR}/sts
  ${NICA_CBM}/helpers
)
Set(SYSTEM_INCLUDE_DIRECTORIES
  ${ROOT_INCLUDE_DIR} 
  ${BASE_INCLUDE_DIRECTORIES}
  ${CLHEP_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
)

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
  ${Vc_LIB_DIR}
  ${KFParticle_LIB_DIR}
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})
 
Link_Directories(${LINK_DIRECTORIES})

# List of source files
set(SRCS
CbmDetectorID.cxx
NicaCbmFirstTask.cxx
CbmHelix.cxx
)

Set(HEADERS )
Set(LINKDEF NicaCbmHelpersLinkDef.h)
Set(LIBRARY_NAME NicaCbmHelper)
Set(DEPENDENCIES NicaFeatures NicaDataFormat Base NicaCut NicaAna KF KFParticle)
GENERATE_LIBRARY()

Add_Dependencies(NicaCbmHelper NICAFEMTO)

