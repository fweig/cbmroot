# Create a library called "libCbmFlibReader" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/mbs
  ${CMAKE_CURRENT_SOURCE_DIR}/src_mbs
  ${CMAKE_CURRENT_SOURCE_DIR}/roc
  ${CMAKE_CURRENT_SOURCE_DIR}/src_roc
  ${CMAKE_CURRENT_SOURCE_DIR}/feet
)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES} 
  ${Boost_INCLUDE_DIR} 
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})

Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
)

Link_Directories(
  ${LINK_DIRECTORIES}
)

Set(SRCS
  src_mbs/MbsTypeDefs.cxx
  src_mbs/LmdFile.cxx
  src_mbs/LmdInput.cxx
  src_roc/Message.cxx
  src_roc/Board.cxx
  src_roc/Iterator.cxx
  src_roc/OperList.cxx
  src_roc/FileInput.cxx
  src_roc/Factory.cxx
)

Set(HEADERS 
  mbs/LmdTypeDefs.h
  mbs/MbsTypeDefs.h
  mbs/LmdFile.h
  mbs/LmdInput.h
  roc/Message.h
  roc/Board.h
  roc/Iterator.h
  roc/OperList.h
  roc/FileInput.h
  roc/Factory.h
)

Set(DEPENDENCIES
    Base
)

If(_UINT8_T_EXIST)
  Add_Definitions(-DHAVE_UINT8_T_HEADER_FILE)
EndIf()

Set(LIBRARY_NAME CbmRocLight)

GENERATE_LIBRARY()
