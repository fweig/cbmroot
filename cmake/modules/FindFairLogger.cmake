# Find FairLogger installation 

FIND_PATH(FAIRLOGGER_INCLUDE_DIR NAMES Logger.h HINTS
  ${SIMPATH}/include/fairlogger
  NO_DEFAULT_PATH
)

FIND_PATH(FAIRLOGGER_LIBRARY_DIR NAMES libFairLogger.so libFairLogger.dylib HINTS
   ${SIMPATH}/lib
  NO_DEFAULT_PATH
)

if(FAIRLOGGER_INCLUDE_DIR AND FAIRLOGGER_LIBRARY_DIR)
   set(FAIRLOGGER_FOUND TRUE)
   MESSAGE(STATUS "FairLogger ... - found at ${FAIRLOGGER_LIBRARY_DIR}")
else()
   set(FAIRMQ_FOUND FALSE)
   MESSAGE(STATUS "FairLogger not found")
endif ()

