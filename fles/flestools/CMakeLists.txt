# Create a library called "libCbmFlibReader" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.
set(INCLUDE_DIRECTORIES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

set(SRCS
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


set(LIBRARY_NAME CbmFlibFlesTools)
set(PRIVATE_DEPENDENCIES
  ROOT::Gpad
  ROOT::Hist
  external::fles_ipc
  )

generate_cbm_library()
