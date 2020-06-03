# Create a library called "libBase" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}

  ${CBMBASE_DIR}/utils
)

Include_Directories( ${INCLUDE_DIRECTORIES})

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
)

Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
${FAIRROOT_LIBRARY_DIR}
${Boost_LIBRARY_DIRS}
)
 
link_directories( ${LINK_DIRECTORIES})

set(SRCS
CbmCave.cxx            
CbmGeoCave.cxx         
#CbmGeoMagnet.cxx       
CbmGeoPassivePar.cxx   
#CbmGeoPipe.cxx         
CbmGeoPlatform.cxx
CbmMagnet.cxx          
CbmPassiveContFact.cxx 
CbmPipe.cxx
CbmPlatform.cxx
#CbmShield.cxx
#CbmShieldGeo.cxx         
)

set(LINKDEF CbmPassiveLinkDef.h)
Set(LIBRARY_NAME CbmPassive)
Set(DEPENDENCIES
    CbmBase Base 
)

GENERATE_LIBRARY()
