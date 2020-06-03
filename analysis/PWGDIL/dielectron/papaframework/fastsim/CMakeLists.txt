Set(INCLUDE_DIRECTORIES
${CBMROOT_SOURCE_DIR}/analysis
${CBMROOT_SOURCE_DIR}/analysis/papa
${CBMROOT_SOURCE_DIR}/analysis/papa/fastsim
${CBMROOT_SOURCE_DIR}/generators
${CBMBASE_DIR}
${CBMDATA_DIR}
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${Boost_INCLUDE_DIR}
  ${VC_INCLUDE_DIRS}
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)

#Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Weffc++ -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare -Wno-ignored-qualifiers -Wno-overloaded-virtual")

link_directories( ${LINK_DIRECTORIES})

set(SRCS
CbmFastSim.cxx
CbmFastDecayer.cxx
)


Set(HEADERS )

Set(LINKDEF CbmFastSimLinkDef.h)
Set(LIBRARY_NAME CbmFastSim)
Set(DEPENDENCIES
    CbmPapa CbmBase CbmData CbmSimGenerators Base VMC Vc.a
)

GENERATE_LIBRARY()



