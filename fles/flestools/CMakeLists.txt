# Create a library called "libCbmFlibReader" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

Set(INCLUDE_DIRECTORIES
)

Set(SYSTEM_INCLUDE_DIRECTORIES
  ${BASE_INCLUDE_DIRECTORIES}
  ${Boost_INCLUDE_DIR}
  ${ROOT_INCLUDE_DIR}
  ${IPC_INCLUDE_DIRECTORY}
)

Include_Directories(${INCLUDE_DIRECTORIES})
Include_Directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})


Set(LINK_DIRECTORIES
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  ${Boost_LIBRARY_DIRS}
)

Link_Directories(
  ${LINK_DIRECTORIES}
)

Set(SRCS
   CbmFormatDecHexPrintout.cxx
   CbmFormatMsBufferPrintout.cxx
   CbmFormatMsHeaderPrintout.cxx
   CbmFormatTsPrintout.cxx
   CbmFlesCanvasTools.cxx
   CbmFlesHistosTools.cxx
)

If(_UINT8_T_EXIST)
  Add_Definitions(-DHAVE_UINT8_T_HEADER_FILE)
  Set(DEFINITIONS HAVE_UINT8_T_HEADER_FILE)
EndIf()

#Set(LINKDEF CbmFlibMcbm2018LinkDef.h )
Set(LIBRARY_NAME CbmFlibFlesTools)
Set(DEPENDENCIES
    fles_ipc Hist
)

GENERATE_LIBRARY()
