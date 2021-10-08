# Create a library called "libCbmOnlineData" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
   ${CMAKE_SOURCE_DIR}/core/data
   ${CMAKE_SOURCE_DIR}/core/data/sts
)

include_directories(${INCLUDE_DIRECTORIES})

set(SYSTEM_INCLUDE_DIRECTORIES
  ${Boost_INCLUDE_DIR}
)

include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

set(SRCS
  ${CMAKE_SOURCE_DIR}/core/data/sts/CbmStsDigi.cxx
)


ADD_DEFINITIONS(-DNO_ROOT)

#---Create a shared library with geneated dictionary
add_library(OnlineData SHARED ${SRCS})
