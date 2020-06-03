# Find FairMQ installation 

FIND_PATH(FAIRMQ_INCLUDE_DIR NAMES FairMQDevice.h HINTS
  ${FAIRROOTPATH}/include/fairmq
  ${SIMPATH}/include/fairmq
  NO_DEFAULT_PATH
)

FIND_PATH(FAIRMQ_LIBRARY_DIR NAMES libFairMQ.so libFairMQ.dylib HINTS
   ${FAIRROOTPATH}/lib
   ${SIMPATH}/lib
  NO_DEFAULT_PATH
)

# look for exported FairMQ targets and include them
find_file(_fairroot_fairmq_cmake
    NAMES FairMQ.cmake
    HINTS ${FAIRROOTPATH}/include/cmake
)
if(_fairroot_fairmq_cmake)
    include(${_fairroot_fairmq_cmake})
endif()

if(FAIRMQ_INCLUDE_DIR AND FAIRMQ_LIBRARY_DIR)
   set(FAIRMQ_FOUND TRUE)
   MESSAGE(STATUS "FairMQ ... - found at ${FAIRMQ_LIBRARY_DIR}")
else()
   set(FAIRMQ_FOUND FALSE)
   MESSAGE(STATUS "FairMQ not found")
endif ()

